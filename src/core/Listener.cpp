#include <ThreadPool.h>
#include <core/HTTPHandler.h>
#include <core/Listener.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

Listener::Listener() : port(8080), nThreads(4), running(false) {}

int bindSocket(int port) {
  // Create socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == 0) {
    std::cerr << "Socket creation failed." << std::endl;
    return -1;
  }

  // Configure server address
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // Bind socket to the specified port
  if (bind(serverSocket, reinterpret_cast<sockaddr*>(&address),
           sizeof(address)) < 0) {
    std::cerr << "Bind failed." << std::endl;
    close(serverSocket);
    return -1;
  }

  // Start listening for connections
  if (listen(serverSocket, 10) < 0) {
    std::cerr << "Listen failed." << std::endl;
    close(serverSocket);
    return -1;
  }
  return serverSocket;
}

int Listener::start() {
  const auto serverSocket = bindSocket(this->port);

  if (serverSocket < 0) {
    std::cerr << "Socket creation failed." << std::endl;
    return -1;
  }

  std::cout << "HTTP Server is running on port " << this->port << std::endl;

  ThreadPool threadPool(this->nThreads);

  this->running = true;

  while (running) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
      std::cerr << "Failed to accept connection." << std::endl;
      continue;
    }

    threadPool.enqueue(
        [clientSocket]() { HTTPHandler::handleClientRequest(clientSocket); });
  }

  // Close the server socket
  close(serverSocket);
  return 0;
}

void Listener::stop() { this->running = false; }
