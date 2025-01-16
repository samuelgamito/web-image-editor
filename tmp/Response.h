#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Response {
 public:
  Response();

  // Getters
  const unordered_map<string, string>& getHeaders() const;
  const string& getBody() const;

  // Setters
  void setHeader(const string& key, const string& value);
  void setBody(const string& body);

  // Utility Functions
  string createHTTPResponse();

 private:
  unordered_map<string, string> headers;
  int contentLength;
  int statusCode;
  string body;
};

#endif  // RESPONSE_H
