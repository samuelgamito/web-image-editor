#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "../include/ThreadPool.h"
#include "../include/core/HTTPHandler.h"

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

int main() {
  // Create socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == 0) {
    std::cerr << "Socket creation failed." << std::endl;
    return 1;
  }

  // Configure server address
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket to the specified port
  if (bind(serverSocket, reinterpret_cast<sockaddr*>(&address),
           sizeof(address)) < 0) {
    std::cerr << "Bind failed." << std::endl;
    close(serverSocket);
    return 1;
  }

  // Start listening for connections
  if (listen(serverSocket, 10) < 0) {
    std::cerr << "Listen failed." << std::endl;
    close(serverSocket);
    return 1;
  }

  std::cout << "HTTP Server is running on port " << PORT << std::endl;

  // Create a thread pool with 4 worker threads
  ThreadPool threadPool(4);

  while (true) {
    // Accept a client connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
      std::cerr << "Failed to accept connection." << std::endl;
      continue;
    }

    // Enqueue a task to handle the client
    threadPool.enqueue(
        [clientSocket]() { HTTPHandler::handleClientRequest(clientSocket); });
  }

  // Close the server socket
  close(serverSocket);
  return 0;
}
