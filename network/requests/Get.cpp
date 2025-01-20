#include "BaseRequest.hpp"

std::string BaseRequest::getPage(std::string path)
{
	std::ifstream file(path.c_str());
	if (!file)
		throw std::invalid_argument("File does not exist");
	std::stringstream buffer;
	buffer << file.rdbuf();
	return (buffer.str());
}

// Fonction pour lire un fichier en mode texte ou binaire
std::vector<char> BaseRequest::readFile(const std::string &path, bool isBinary)
{
    std::ifstream file(path.c_str(), isBinary ? std::ios::binary : std::ios::in);
    if (!file.is_open())
    {
        return std::vector<char>(); // Retourner un vecteur vide si le fichier ne peut pas être ouvert
    }

    std::vector<char> content;
    file.seekg(0, std::ios::end);
    std::ifstream::pos_type fileSize = file.tellg();
    content.resize(fileSize);

    file.seekg(0, std::ios::beg);
    file.read(&content[0], fileSize);
    if (!file)
	{
        std::cerr << "Error: Could not open the file" << std::endl;
		return content;
	}

    return content;
}

std::vector<char> BaseRequest::readBinaryFile(const std::string& path) {
    // Ouvre le fichier en mode binaire
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) 
	{
        throw std::runtime_error("Unable to open file: " + path); 
	}
    // Lis tout le fichier dans un vecteur de caractères
    std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

char** getArguments(std::string path)
{
	size_t count = 1;
	size_t pos = path.find("?");
	if(path.find("?") != std::string::npos)
	{
		while(path.find("&", pos) != std::string::npos)
		{
			count++;
			pos = path.find("&", pos + 1);
		}
	}
	else
	{
		return NULL;
	}
	char** arguments = new char*[count + 2];
	std::string firstArgument = path.substr(0, path.size() - (pos - 1));
	arguments[0] = new char[firstArgument.size() + 1];
	std::strcpy(arguments[0], firstArgument.c_str());
	count = 1;
	size_t posFirst = path.find("?") + 1;
	size_t posEnd;
	if(path.find("?") != std::string::npos)
	{
		while(path.find("&", posFirst) != std::string::npos)
		{
			posEnd = path.find("&", posFirst);
			std::string arg = path.substr(posFirst, posEnd - posFirst);
			arguments[count] = new char[arg.size() + 1];
			std::strcpy(arguments[count], arg.c_str());
			count++;
			posFirst = posEnd + 1;
		}
		std::string lastArg = path.substr(posFirst);
		arguments[count] = new char[lastArg.size() + 1];
		std::strcpy(arguments[count], lastArg.c_str());
	}
	arguments[count + 1] = NULL;
	return arguments;
}

void	BaseRequest::get(std::string path)
{
	// Empty path redirection
	// if (path == "/")
	// 	this->path = "ressources/pages/index.html";
	try
	{
		if(path.find(".sh") != std::string::npos)
		{
			std::string newPath = path.substr(0, (path.find(".sh") + 3));
			char **arguments = getArguments(path);
			handleCGI(newPath, "GET", arguments);
			int i = 0;
			if (arguments != NULL)
			{
				while (arguments[i] != NULL)
				{
					delete[]	arguments[i];
					i++;
				}
				delete[] arguments;
			}
			setStatusCode(200);
			return ;
		}
		if(path.find(".out") != std::string::npos)
		{
			std::string newPath = path.substr(0, (path.find(".out") + 4));
			std::cout << "newPath is " << newPath << std::endl;
			std::cout << "Path is " << path << std::endl;
			char **arguments = getArguments(path);
			handleCGI(newPath, "GET", arguments);
			int i = 0;
			if (arguments != NULL)
			{
				while (arguments[i] != NULL)
				{
					delete[]	arguments[i];
					i++;
				}
				delete[] arguments;
			}
			setStatusCode(200);
			return ;
		}
    	getMimeType();
		bool isBinary = mimeType.substr(0, 4) != "text"; //  || mimeType == "application/pdf"
		if (isBinary)
        {
            this->binaryContent = readFile(path, true); // Lire le fichier en mode binaire
			std::cout << RED << "Mime type BINARY" << WHITE << std::endl;

            if (binaryContent.empty())
            {
                // Fichier non trouvé, renvoyer une page 404
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
            }
            else
                setStatusCode(200);
        }
        else
        {
            this->pageContent = getPage(path); // Lire le fichier en mode texte
			std::cout << RED << "Mime type HTTP" << WHITE << std::endl;
            setStatusCode(200);
        }
		this->pageContent = getPage(path);
		setStatusCode(200);
    }
	catch (const std::exception &e)
	{
        //404PATH
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
	}
}