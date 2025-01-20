//Status code file with short description 

#include "BaseRequest.hpp"

void BaseRequest::setStatusCode(int status)
{
    switch (status) 
    {
        // Informational Responses (100–199)
        case 100:
            statusCode = "100 Continue";
            break;
        case 101:
            statusCode = "101 Switching Protocols";
            break;
        case 102:
            statusCode = "102 Processing";
            break;
        case 103:
            statusCode = "103 Early Hints";
            break;

        // Successful Responses (200–299)
        case 200: //Cas de base qui s'applique si les autre conditions ne sont pas remplis
            statusCode = "200 OK";
            break;
        case 201: //Liée à la  réponse de la méthode POST
            statusCode = "201 Created";
            break;
        case 202:
            statusCode = "202 Accepted";
            break;
        case 203:
            statusCode = "203 Non-Authoritative Information";
            break;
        case 204: //Pour des requêtes test, par exemple vérifier qu’une ressource est bien disponible sur le serveur
            statusCode = "204 No Content";
            break;
        case 205:
            statusCode = "205 Reset Content";
            break;
        case 206:
            statusCode = "206 Partial Content";
            break;
        case 207:
            statusCode = "207 Multi-Status";
            break;
        case 208:
            statusCode = "208 Already Reported";
            break;
        case 226:
            statusCode = "226 IM Used";
            break;

        // Redirection Messages (300–399)
        case 300:
            statusCode = "300 Multiple Choices";
            break;
        case 301: //Faire un fichier dont le chemin est noté comme modifier de manière permanente.
            statusCode = "301 Moved Permanently";
            break;
        case 302: //Faire un fichier dont le chemin est noté comme modifier de manière temporaire.
            statusCode = "302 Found";
            break;
        case 303:
            statusCode = "303 See Other";
            break;
        case 304: //Voir comment faire une requête précise qui demande si le fichier a été modifié. Faire un qui a été modifié (par l’user ?) et un autre non.
            statusCode = "304 Not Modified";
            break;
        case 305:
            statusCode = "305 Use Proxy";
            break;
        case 307:
            statusCode = "307 Temporary Redirect";
            break;
        case 308:
            statusCode = "308 Permanent Redirect";
            break;

        // Client Error Responses (400–499)
        case 400:
            statusCode = "400 Bad Request";
            break;
        case 401:
            statusCode = "401 Unauthorized";
            break;
        case 402:
            statusCode = "402 Payment Required";
            break;
        case 403: //Faux fichier admin, le rendre inaccessible.
            statusCode = "403 Forbidden";
            break;
        case 404: //Le basique si on trouve rien d’autre.
            statusCode = "404 Not Found";
            break;
        case 405: //Si la demande sort du scope gérer de GET POST et DELETE.
            statusCode = "405 Method Not Allowed";
            break;
        case 406:
            statusCode = "406 Not Acceptable";
            break;
        case 407:
            statusCode = "407 Proxy Authentication Required";
            break;
        case 408: //Avec poll() ou mettre un timer au début de la requête. Si pas de réponse au bout de X temps renvoyer cette erreur
            statusCode = "408 Request Timeout";
            break;
        case 409:
            statusCode = "409 Conflict";
            break;
        case 410:
            statusCode = "410 Gone";
            break;
        case 411: //Vérifier si le content length est bien présent. Faire une fausse requête ou il y est pas pour tester.
            statusCode = "411 Length Required";
            break;
        case 412:
            statusCode = "412 Precondition Failed";
            break;
        case 413: //Mettre ce message quand il a trop de fichier a read (quand je teste avec 100000. Si ca marche pas tant pis laisse tomber)
            statusCode = "413 Payload Too Large";
            break;
        case 414: //Définir un URL trop long. Si atteint envoyer ce message d’erreur.
            statusCode = "414 URI Too Long";
            break;
        case 415: //Mettre ce message pour les ressources qu’on a choisi de ne pas gérer.
            statusCode = "415 Unsupported Media Type";
            break;
        case 416:
            statusCode = "416 Range Not Satisfiable";
            break;
        case 417:
            statusCode = "417 Expectation Failed";
            break;
        case 418:
            statusCode = "418 I'm a teapot";
            break;
        case 421:
            statusCode = "421 Misdirected Request";
            break;
        case 422:
            statusCode = "422 Unprocessable Entity";
            break;
        case 423:
            statusCode = "423 Locked";
            break;
        case 424:
            statusCode = "424 Failed Dependency";
            break;
        case 425:
            statusCode = "425 Too Early";
            break;
        case 426:
            statusCode = "426 Upgrade Required";
            break;
        case 428:
            statusCode = "428 Precondition Required";
            break;
        case 429:
            statusCode = "429 Too Many Requests";
            break;
        case 431:
            statusCode = "431 Request Header Fields Too Large";
            break;
        case 451:
            statusCode = "451 Unavailable For Legal Reasons";
            break;

        // Server Error Responses (500–599)
        case 500: //Mettre cette erreur si on rentre dans aucune de toutes les autres.
            statusCode = "500 Internal Server Error";
            break;
        case 501:
            statusCode = "501 Not Implemented";
            break;
        case 502:
            statusCode = "502 Bad Gateway";
            break;
        case 503:
            statusCode = "503 Service Unavailable";
            break;
        case 504:
            statusCode = "504 Gateway Timeout";
            break;
        case 505: //Vérifier si c’est bien de l’HTTP 1.1 sinon erreur
            statusCode = "505 HTTP Version Not Supported";
            break;
        case 506:
            statusCode = "506 Variant Also Negotiates";
            break;
        case 507:
            statusCode = "507 Insufficient Storage";
            break;
        case 508:
            statusCode = "508 Loop Detected";
            break;
        case 510:
            statusCode = "510 Not Extended";
            break;
        case 511:
            statusCode = "511 Network Authentication Required";
            break;

        // Default case for unrecognized status codes
        default:
            statusCode = "Unknown Status Code";
            break;
    }
}
