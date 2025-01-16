#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <unordered_map>

using namespace std;

struct RequestStruct {
  string method;
  string path;
  string httpVersion;
  unordered_map<string, string> headers;
  unordered_map<string, string> queryParams;
  string body;
};

class Request {
 public:
  Request();

  // Getters
  const string& getMethod() const;
  const string& getPath() const;
  const unordered_map<string, string>& getHeaders() const;
  const unordered_map<string, string>& getQueryParams() const;
  const string& getBody() const;
  const string& getHttpVersion() const;

  // Setters
  void setMethod(const string& method);
  void setPath(const string& path);
  void setHeader(const string& key, const string& value);
  void setQueryParam(const string& key, const string& value);
  void setBody(const string& body);
  void setHttpVersion(const string& http_version);

  // Utility Functions
  bool hasHeader(const string& key) const;
  const string& getHeader(const string& key) const;
  bool hasQueryParam(const string& key) const;
  const string& getQueryParam(const string& key) const;

 private:
  RequestStruct request;
};

#endif  // REQUEST_H
