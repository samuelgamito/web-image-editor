#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include <iostream>

class HTTPHandler {
 public:
  static void handleClientRequest(int clientSocket);
};

#endif  // HTTPREQUESTHANDLER_H
