#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

# include "BaseRequest.hpp"
# include "../server/Server.hpp"
# include "../utils/utils.hpp"

int BaseRequest::cgi_pid = -1;

void BaseRequest::timeout_handler_CGI(int signum)
{
    (void)signum;
    std::cerr << "CGI script timed out! Terminating child process." << std::endl;
    if (cgi_pid > 0)
    {
        kill(cgi_pid, SIGKILL);
    }
}

void BaseRequest::handleCGI(std::string path, std::string method, char **methods)
{
    // path = /*"./Desktop/Webserv_11_07/" + */  path;
    cgi_pid = -1;
    std::string filename = "./ressources/CGI_bin/file.txt";
    int pipe_in[2];
    int k = 0;
    int pipe_out[2];
    char **str = new char*[2];
 
    // size_t i = 0;
    str[0] = strdup(path.c_str());
    //str[1] = strdup(".usr/bin/bash");
    str[1] = NULL;
    pipe(pipe_in);
    pipe(pipe_out);
    signal(SIGALRM, timeout_handler_CGI);
    alarm(5);
    cgi_pid = fork();
	if (cgi_pid == 0)
	{
        dup2(pipe_in[0], STDIN_FILENO);
	    dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_in[0]);
		close(pipe_out[1]);
		close(pipe_out[0]);
		close(pipe_in[1]);
        alarm(0);
		//dup2(pipe_in[0], STDIN_FILENO);
		// close(pipe_out[0]);
		// dup2(pipe_out[1], STDOUT_FILENO);
		//close(pipe_in[1]);
		int _exit_status;
		if(methods == NULL)
		{
			std::cout << "Methods not findeds" << std::endl;
			_exit_status = execve(path.c_str(), str, NULL);
		}
		else
		{
			std::cout << "Methods findeds" << std::endl;
			_exit_status = execve(path.c_str(), methods, NULL);
		}
		//close(pipe_out[1]);
        exit(_exit_status);
		//close(pipe_in[0]);
	}
    else
    {
        // dup2(pipe_in[0], STDIN_FILENO);
	    // dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_in[0]);
		close(pipe_out[1]);
        //dup2(pipe_out[0], STDIN_FILENO);
        int status;
        waitpid(cgi_pid, &status, 0);
        alarm(0);
		if(method == "POST")
		{
			std::ofstream out_file(filename.c_str());
			char buffer2[6000];
			ssize_t nbytes = read(pipe_out[0], buffer2, sizeof(buffer2) - 1);
			if (nbytes > 0) 
			{
				buffer2[nbytes] = '\0'; // Null-terminate le buffer
				std::string test_buff(buffer2);
			}
            ////////////////////////////////
            else
            {
                if (nbytes == 0)
                {
                    std::cout << "No data to read" << std::endl;
                    k = 0;
                    if(str != NULL)
                    {
                        while (str[k] != NULL)
                        {
                            free(str[k]);
                            k++;
                        }
                    delete[] str;
                    }
                    return;
                }
                else
                {
                    std::cerr << "Error : Failed to read data" << std::endl;
                    k = 0;
                    if(str != NULL)
                    {
                        while (str[k] != NULL)
                        {
                            free(str[k]);
                            k++;
                        }
                    delete[] str;
                    }
                    return;
                }
            }
			if (out_file.is_open())
			{
				out_file << buffer2;
				out_file.close();
				pageContent =  "CGI executed, file modified.";
			}
			else
				std::cout << "Error du open " << std::endl;
		}
		else if (method == "GET")
		{
			char buffer2[6000];
			ssize_t nbytes = read(pipe_out[0], buffer2, sizeof(buffer2) - 1);
			if (nbytes > 0) 
			{
				buffer2[nbytes] = '\0'; // Null-terminate le buffer
				std::string test_buff(buffer2);
			}
            else
            {
                if (nbytes == 0)
                {
                    std::cout << "No data to read" << std::endl;
                    k = 0;
                    if(str != NULL)
                    {
                        while (str[k] != NULL)
                        {
                            free(str[k]);
                            k++;
                        }
                    delete[] str;
                    }
                    return;
                }
                else
                {
                    std::cerr << "Error : Failed to read data" << std::endl;
                    k = 0;
                    if(str != NULL)
                    {
                        while (str[k] != NULL)
                        {
                            free(str[k]);
                            k++;
                        }
                        delete[] str;
                    }
                    return;
                }
            }
			pageContent += buffer2;
		}
		close(pipe_out[0]);
		close(pipe_in[1]);
        cgi_pid = -1;
    }
    k = 0;
    if(str != NULL)
    {
        while (str[k] != NULL)
        {
            free(str[k]);
            k++;
        }
        delete[] str;
	}
}

std::string getTypeMultipart(const std::string& multi_body)
{
    // Find the position of the "Content-Disposition" header to identify the part
    std::size_t content_disp_start = multi_body.find("Content-Disposition:");
    if (content_disp_start == std::string::npos)
    {
        std::cerr << "No Content-Disposition header found!" << std::endl;
    }


    // Find the position of the "Content-Type" header within this part
    std::size_t content_type_start = multi_body.find("Content-Type:", content_disp_start);
    if (content_type_start == std::string::npos)
    {
        std::cerr << "No Content-Type header found!" << std::endl;
        return "txt";
    }

    // Extract the Content-Type value (skip the "Content-Type:" label and any leading spaces)
    std::size_t type_prestart = content_type_start + 14;
    std::size_t type_start = multi_body.find("/", type_prestart) + 1;
    std::size_t type_end = multi_body.find("\r\n", type_start);
    std::string type = multi_body.substr(type_start, type_end - type_start);
    if (type_end == std::string::npos)
    {
        std::cerr << "Content-Type header not properly terminated!" << std::endl;
        return "txt";
    }
    return type;
}

std::string getBodyMultipart(const std::string& multi_body) 
{
    std::size_t content_disp_start = multi_body.find("Content-Disposition:");
    if (content_disp_start == std::string::npos) {
        std::cerr << "No Content-Disposition header found!" << std::endl;
        return "";
    }

    std::size_t header_end = multi_body.find("\r\n\r\n", content_disp_start);
    if (header_end == std::string::npos) {
        std::cerr << "No end of headers found!" << std::endl;
        return "";
    }

    std::size_t content_start = header_end + 4;

    std::size_t boundary_pos = multi_body.find("\r\n--", content_start);
    if (boundary_pos == std::string::npos) {
        boundary_pos = multi_body.length();
    }

    std::string content = multi_body.substr(content_start, boundary_pos - content_start);

    std::size_t trim_pos = content.find_last_not_of("\r\n");
    if (trim_pos != std::string::npos) {
        content = content.substr(0, trim_pos + 1);
    }

    return content;
}

void BaseRequest::handleMultipartData(const std::string& body, std::string content_type)
{
    std::string content;
    std::size_t begin_found = content_type.find("boundary=") + 9;
    std::size_t end_found = content_type.find("\r", begin_found);
    std::string boundary = "--" + content_type.substr(begin_found, end_found - begin_found);
    std::string end_boundary = boundary + "--";
    std::size_t new_begin_part = 0;
    std::size_t begin_part = 0;
    std::size_t end_part = 0;
    std::size_t end_request = body.find(end_boundary);
    if (end_request == std::string::npos)
    {
        std::cerr << "End boundary not found in body!" << std::endl;
        return;
    }
    std::vector<std::string>  all_body_from_multipart;
    while(end_part < end_request)
    {
        new_begin_part = body.find(boundary, end_part);
        if (new_begin_part == std::string::npos)
            break;
        begin_part = new_begin_part + boundary.length() + 2;
        end_part = body.find(boundary, begin_part);
        all_body_from_multipart.push_back(body.substr(begin_part, end_part - begin_part - 2));
    }
    for (size_t i = 0; i < all_body_from_multipart.size(); i++)
    {
        std::string type = getTypeMultipart(all_body_from_multipart[i]);
        handleAnyType(getBodyMultipart(all_body_from_multipart[i]), type);
    }
    return ;
}

std::string trim(const std::string &str) {
    std::size_t first = str.find_first_not_of(" \t\r\n");
    std::size_t last = str.find_last_not_of(" \t\r\n");

    if (first == std::string::npos || last == std::string::npos) {
        return "";  // The string is composed entirely of whitespace
    }

    return str.substr(first, last - first + 1);
}

void BaseRequest::handleAnyType(const std::string& body, std::string type)
{
    std::string filename = path + "uploaded_file." + type;
	// Récupération du nom de fichier depuis les headers
    if (headers.find("X-File-Name") != headers.end())
    {
        filename = path + trim(headers["X-File-Name"]);
		std::cout << "Name found : " << headers["X-File-Name"] << std::endl;

    }
	else
		std::cout << "No name found" << std::endl;
    std::ofstream out_file(filename.c_str(), std::ios::app);
    if (out_file.is_open())
    {
        out_file << body;
        out_file.close();
        pageContent = type + "file has been received.";
    } 
    else 
    {
        std::cerr << "Could not open file for writing." << std::endl;
    }
}

char** getArgumentsPost(std::string path, std::string body)
{
	size_t count = 1;
	if(body == "")
        return NULL;
	size_t pos = 0;
	while(body.find("&", pos) != std::string::npos)
	{
		count++;
		pos = body.find("&", pos + 1);
	}
	char** arguments = new char*[count + 2];
	std::string firstArgument = path;
	arguments[0] = new char[firstArgument.size() + 1];
	std::strcpy(arguments[0], firstArgument.c_str());
	count = 1;
	size_t posFirst = 0;
	size_t posEnd;
    while(body.find("&", posFirst) != std::string::npos)
    {
        posEnd = body.find("&", posFirst);
        std::string arg = body.substr(posFirst, posEnd - posFirst);
        arguments[count] = new char[arg.size() + 1];
        std::strcpy(arguments[count], arg.c_str());
        count++;
        posFirst = posEnd + 1;
    }
    std::string lastArg = body.substr(posFirst);
    arguments[count] = new char[lastArg.size() + 1];
    std::strcpy(arguments[count], lastArg.c_str());
	arguments[count + 1] = NULL;
	return arguments;
}

int BaseRequest::post(std::string path)
{
    std::cout << "POST REQUEST ON URL " << path << std::endl;

    std::string content_type;
    if (path.find("CGI_bin") != std::string::npos)
    {
		char **arguments = getArgumentsPost(path, body);
        handleCGI(path, "POST", arguments);
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
        return 0;
    }
    if (headers.find("Content-Type") != headers.end())
    {
        content_type = headers["Content-Type"];
    } 
    else
    {
        std::cerr << "Content-Type header not found" << std::endl;
        setStatusCode(400); // Bad Request
        pageContent = "Missing Content-Type header.";
        return 0;
    }
    if ((content_type.find("application/x-www-form-urlencoded") != std::string::npos) || 
        (content_type.find("application/json") != std::string::npos) || 
        (content_type.find("text/plain") != std::string::npos) || 
        (content_type.find("application/xml") != std::string::npos) ||
        (content_type.find("application/pdf") != std::string::npos) || 
        (content_type.find("image/jpeg") != std::string::npos) || 
        (content_type.find("image/png") != std::string::npos) || 
        (content_type.find("image/gif") != std::string::npos) || 
        (content_type.find("text/html") != std::string::npos))
    {
        std::size_t begin_found = content_type.find("/");
        // std::size_t end_found = content_type.find("\r\n", begin_found);
        std::size_t end_found = content_type.find_first_of(" ;\r\n", begin_found);

        // std::string type = content_type.substr(begin_found + 1, (end_found - begin_found) - 1);
        std::string type = content_type.substr(begin_found + 1, end_found - (begin_found + 1));
        type.erase(std::remove_if(type.begin(), type.end(), ::isspace), type.end());

        if (type == "x-www-form-urlencoded" || type == "plain")
            type = "txt";
        // if (type == "pdf" || type == "jpeg" || type == "png" || type == "gif" || type == "html")
        //     handleAnyType(body, type);
        // else 
        handleAnyType(body, type);
    } 
    else if (content_type.find("multipart/form-data") != std::string::npos)
    {
        handleMultipartData(body, content_type);
    }
    else
    {
        setStatusCode(415);
        pageContent = "Unsupported media type.";
        return 0;
    }
    // Flemme de le mettre, cree plusieurs bodys

    setStatusCode(200);
    return 0;
}