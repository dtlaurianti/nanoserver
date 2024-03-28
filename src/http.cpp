//
// Created by dtlau on 3/27/2024.
//
#include<SPI.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include "http.h"


HTTPMethod stringToHTTPMethod(const std::string method) {
    auto httpMap = stringHTTPTable.find(method);
    if (httpMap == stringHTTPTable.end()) {
        return HTTPMethod::UNKNOWN;
    }
    return httpMap->second;
}

std::string httpMethodToString(const HTTPMethod method) {
    auto httpMap = httpStringTable.find(method);
    if (httpMap == httpStringTable.end()) {
        return "";
    }
    return httpMap->second;
}

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
    req.body = "";
    if (req.headers.count("Transfer-Encoding")) {
        Serial.println("Error: Server does not support Transfer-Encoding");
        return req;
    }
    if (req.headers.count("Content-Length") <= 0) {
        return req;
    }
    int bodyLen = std::stoi(req.headers["Content-Length"]);
    for (int i = 0; i < bodyLen; ++i) {
        req.body += stream.get();
    }
    return req;
};