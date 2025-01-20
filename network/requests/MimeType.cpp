#include "BaseRequest.hpp"

void BaseRequest::getMimeType()
{
    std::string extension = this->path.substr(path.find_last_of('.') + 1);

    if (extension == "html")
        this->mimeType = "text/html";
    else if (extension == "css")
        this->mimeType = "text/css";
    else if (extension == "js")
        this->mimeType = "application/javascript";
    else if (extension == "png")
        this->mimeType = "image/png";
    else if (extension == "jpg" || extension == "jpeg")
        this->mimeType = "image/jpeg";
    else if (extension == "gif")
        this->mimeType = "image/gif";
    else if (extension == "svg")
        this->mimeType = "image/svg+xml";
    else if (extension == "ico")
        this->mimeType = "image/x-icon";
    else if (extension == "pdf")
        this->mimeType = "application/pdf";
    else if (extension == "txt")
        this->mimeType = "text/plain";
    else if (extension == "xml")
        this->mimeType = "application/xml";
    else if (extension == "json")
        this->mimeType = "application/json";
    else if (extension == "zip")
        this->mimeType = "application/zip";
    else if (extension == "tar")
        this->mimeType = "application/x-tar";
    else if (extension == "gzip")
        this->mimeType = "application/gzip";
    else if (extension == "mp4")
        this->mimeType = "video/mp4";
    else if (extension == "webm")
        this->mimeType = "video/webm";
    else if (extension == "ogg")
        this->mimeType = "audio/ogg";
    else if (extension == "mp3")
        this->mimeType = "audio/mpeg";
    else if (extension == "wav")
        this->mimeType = "audio/wav";
    else if (extension == "csv")
        this->mimeType = "text/csv";
    else if (extension == "rtf")
        this->mimeType = "application/rtf";
    else if (extension == "doc")
        this->mimeType = "application/msword";
    else if (extension == "xls")
        this->mimeType = "application/vnd.ms-excel";
    else if (extension == "ppt")
        this->mimeType = "application/vnd.ms-powerpoint";
    else if (extension == "md")
        this->mimeType = "text/markdown";
    else if (extension == "woff")
        this->mimeType = "font/woff";
    else if (extension == "woff2")
        this->mimeType = "font/woff2";
    else if (extension == "eot")
        this->mimeType = "application/vnd.ms-fontobject";
    else if (extension == "ttf")
        this->mimeType = "font/ttf";
    else if (extension == "otf")
        this->mimeType = "font/otf";
    else
        this->mimeType = "application/octet-stream"; // Par défaut pour les types inconnus
}