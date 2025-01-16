#include <core/Request.h>

Request::Request() { this->request = {}; }

const string& Request::getMethod() const { return this->request.method; }
const string& Request::getPath() const { return this->request.path; }
const unordered_map<string, string>& Request::getHeaders() const {
  return this->request.headers;
}

const string& Request::getBody() const { return this->request.body; }

const string& Request::getHttpVersion() const {
  return this->request.httpVersion;
}

void Request::setMethod(const string& method) { this->request.method = method; }

void Request::setPath(const string& path) { this->request.path = path; }

void Request::setHeader(const string& key, const string& value) {
  this->request.headers[key] = value;
}

void Request::setBody(const string& body) { this->request.body = body; }

void Request::setHttpVersion(const string& http_version) {
  this->request.httpVersion = http_version;
}

bool Request::hasHeader(const string& key) const {
  return this->request.headers.find(key) != this->request.headers.end();
}

const string& Request::getHeader(const string& key) const {
  return this->request.headers.at(key);
}