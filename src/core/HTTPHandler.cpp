#include "../../include/core/HTTPHandler.h"

#include <sys/socket.h>
#include <unistd.h>

constexpr int BUFFER_SIZE = 1024;

using namespace std;

void HTTPHandler::handleClientRequest(int clientSocket) {
  char buffer[BUFFER_SIZE] = {0};

  int offset = 0;
  cout << "\nHTTP Request\n" << endl;

  while (true) {
    const ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, offset);

    cout.write(buffer, bytesRead);
    cout.flush();

    offset += BUFFER_SIZE;

    memset(buffer, 0, BUFFER_SIZE);
    if (bytesRead < BUFFER_SIZE) {
      break;
    }
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