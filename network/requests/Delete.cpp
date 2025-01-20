#include "BaseRequest.hpp"
# include <sys/stat.h> // C function for path testing
#include <cstdio>  // std::remove

// bool fileExists(const std::string& path);

bool fileExists(const std::string& path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void	BaseRequest::del(std::string path)
{
	std::cout << BLUE << "DELETE REQUEST ON " << path << WHITE << std::endl;
    // std::ifstream file(path.c_str());

    if (!fileExists(path))
	{
        // File does not exist
        std::cerr << "File does not exist: " << path << std::endl;
        // 404PATH
        try
		{
			this->pageContent = getPage(_configServer->getErrorPath());
		}
		catch (const std::exception& e)
		{
			std::cerr << "404 default page not found: " << e.what() << std::endl;
			this->pageContent = "";
		}
        this->binaryContent = readFile(_configServer->getErrorPath(), false);
        this->mimeType = "text/html";
        setStatusCode(404);
        return;
    }
    // file.close();

    if (std::remove(path.c_str()) != 0)
	{
        std::cerr << "Failed to delete file: " << path << std::endl;
        setStatusCode(500);
    }
	else
	{
        std::cout << "File deleted successfully: " << path << std::endl;
        setStatusCode(200);
    }
}