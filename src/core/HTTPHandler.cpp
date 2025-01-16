#include "../../include/core/HTTPHandler.h"

#include <core/Request.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <regex>

constexpr int BUFFER_SIZE = 8;

using namespace std;

const string ALLOWED_HTTP_VERBS_REGEX =
    "GET|PUT|POST|PATCH|DELETE|CONNECT|OPTIONS|TRACE";
const string FIRST_LINE_REGEX_STRING =
    "(" + ALLOWED_HTTP_VERBS_REGEX +
    R"()\s+([^\s?]+)(\?[^ ]*)?\s+(HTTP\/[0-9\.]+))";
const regex FIRST_LINE_REGEX(FIRST_LINE_REGEX_STRING);

const std::string HEADER_REGEX_STRING = R"(^([a-zA-Z0-9-_]+):\s*(.+)$)";
const std::regex HEADER_REGEX(HEADER_REGEX_STRING);

std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to) {
  auto &&pos = str.find(from, size_t{});
  while (pos != std::string::npos) {
    str.replace(pos, from.length(), to);
    // easy to forget to add to.length()
    pos = str.find(from, pos + to.length());
  }
  return str;
}

void processLine(const string &line, Request &request) {
  if (smatch matchFirstLine;
      regex_match(line, matchFirstLine, FIRST_LINE_REGEX)) {
    request.setMethod(matchFirstLine[1].str());
    request.setPath(matchFirstLine[2].str());
    ;
    request.setHttpVersion(matchFirstLine[4].str());
  } else if (smatch matchHeaders;
             regex_match(line, matchHeaders, HEADER_REGEX)) {
    request.setHeader(matchHeaders[1], matchHeaders[2]);
  }
}

string processBuffer(char buffer[], const ssize_t bytesRead, Request *request,
                     const string &leftOverLine) {
  cout << "Previous Left Over Data: "
       << replaceAll(leftOverLine, "\r\n", "\\r\\n") << endl;
  cout << "To Process: " << replaceAll(buffer, "\r\n", "\\r\\n") << endl;
  string line = leftOverLine;
  if (bytesRead <= 0) {
    return line;
  }

  for (int i = 0; i < bytesRead; i++) {
    if (buffer[i] == '\r' && i + 1 < bytesRead && buffer[i + 1] == '\n') {
      if (line.empty()) {  // this is last line before body
        return "";
      }
      processLine(line, *request);
      line = "";
      i++;  // jumping  \n character
    } else {
      line += buffer[i];
    }
  }

  cout << "New Left Over Data: " << replaceAll(line, "\r\n", "\\r\\n") << endl;

  return line;
}

void HTTPHandler::handleClientRequest(const int clientSocket) {
  char buffer[BUFFER_SIZE] = {};
  Request request;

  cout << "\nHTTP Request\n" << endl;

  string leftOverLine;
  while (true) {
    const ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    leftOverLine = processBuffer(buffer, bytesRead, &request, leftOverLine);

    memset(buffer, 0, BUFFER_SIZE);
    if (bytesRead < BUFFER_SIZE) {
      break;  // End processing if no more data is expected
    }
  }

  cout << "\n\n\nHTTP Request Processed\n" << endl;
  cout << "HTTP METHOD: " << request.getMethod() << endl;
  cout << "HTTP VERSION: " << request.getHttpVersion() << endl;
  cout << "PATH: " << request.getPath() << endl;
  for (const auto &[fst, snd] : request.getHeaders()) {
    cout << fst << " --> " << snd << endl;
  }

  string httpResponse =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: 13\r\n"
      "\r\n"
      "Hello, World!";

  send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);

  // Close the client socket
  close(clientSocket);
}