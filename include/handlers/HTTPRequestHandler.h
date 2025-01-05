#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include <string>

class HTTPRequestHandler {
 public:
  explicit HTTPRequestHandler(int clientSocket);

 private:
  std::string url;
  std::string host;
  std::string path;
  std::string method;
  std::string body;
  std::string contentType;
}

#endif  // HTTPREQUESTHANDLER_H
