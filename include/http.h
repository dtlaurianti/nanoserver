//
// Created by dtlau on 3/27/2024.
//

#ifndef NANOSERVER_HTTP_H
#define NANOSERVER_HTTP_H

#include <string>
#include <map>

enum HTTPMethod {
    GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH, UNKNOWN
};

struct Request {
    HTTPMethod method;
    std::string target;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

static const std::map<std::string, HTTPMethod> stringHTTPTable {
        {"GET", HTTPMethod::GET},
        {"HEAD", HTTPMethod::HEAD},
        {"POST", HTTPMethod::POST},
        {"PUT", HTTPMethod::PUT},
        {"DELETE", HTTPMethod::DELETE},
        {"CONNECT", HTTPMethod::CONNECT},
        {"OPTIONS", HTTPMethod::OPTIONS},
        {"TRACE", HTTPMethod::TRACE},
        {"PATCH", HTTPMethod::PATCH},
};

static const std::map<HTTPMethod, std::string> httpStringTable {
        {HTTPMethod::GET, "GET"},
        {HTTPMethod::HEAD, "HEAD"},
        {HTTPMethod::POST, "POST"},
        {HTTPMethod::PUT, "PUT"},
        {HTTPMethod::DELETE, "DELETE"},
        {HTTPMethod::CONNECT, "CONNECT"},
        {HTTPMethod::OPTIONS, "OPTIONS"},
        {HTTPMethod::TRACE, "TRACE"},
        {HTTPMethod::PATCH, "PATCH"},
};

HTTPMethod stringToHTTPMethod(const std::string method);
std::string httpMethodToString(const HTTPMethod method);

Request parseRequest(const std::string str);
void printRequest(Request req);

#endif //NANOSERVER_HTTP_H
