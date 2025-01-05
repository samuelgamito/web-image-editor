#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/ThreadPool.h"

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void handleClient(int clientSocket) {
  char buffer[BUFFER_SIZE] = {0};

  off_t offset = 0;
  while (pread(clientSocket, buffer, BUFFER_SIZE, offset) > 0) {
    std::cout << "HTTP Request:\n" << buffer << std::endl;
    offset += BUFFER_SIZE;
  }

  std::string httpResponse =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: 13\r\n"
      "\r\n"
      "Hello, World!";

  send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);

  // Close the client socket
  close(clientSocket);
}

int main() {
  // Create socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == 0) {
    std::cerr << "Socket creation failed." << std::endl;
    return 1;
  }

  // Configure server address
  sockaddr_in address;
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
    threadPool.enqueue([clientSocket]() { handleClient(clientSocket); });
  }

  // Close the server socket
  close(serverSocket);
  return 0;
}
