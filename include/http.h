//
// Created by dtlau on 3/27/2024.
//

#ifndef NANOSERVER_HTTP_H
#define NANOSERVER_HTTP_H

#include <string>
struct Request;
Request parseRequest(const std::string str);

#endif //NANOSERVER_HTTP_H
