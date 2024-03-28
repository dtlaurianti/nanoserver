//
// Created by dtlau on 3/27/2024.
//
#include<SPI.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include "http.h"

enum HTTPMethod {
    GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH, UNKNOWN
};

HTTPMethod stringToHTTPMethod(const std::string method) {
    static const std::map<std::string, HTTPMethod> httpTable {
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

    // search for
    auto httpMap = httpTable.find(method);
    if (httpMap == httpTable.end()) {
        return HTTPMethod::UNKNOWN;
    }
    return httpMap->second;
}

struct Request {
    HTTPMethod method;
    std::string target;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

Request parseRequest(const std::string str) {
    Request req;
    std::istringstream stream(str);
    std::string method;
    // parse the first line by splitting on whitespace
    stream >> method >> req.target >> req.version;
    req.method = stringToHTTPMethod(method);

    // parse headers
    std::string line;
    std::getline(stream, line);
    while (line != "") {
        std::string::size_type colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon+1);
            req.headers[key] = value;
        }
        std::getline(stream, line);
    }

    // get body
    if (req.headers.count("Transfer-Encoding")) {
        Serial.println("Error: Server does not support Transfer-Encoding");
        return req;
    }
    if (req.headers.count("Content-Length") <= 0) {
        return req;
    }
};