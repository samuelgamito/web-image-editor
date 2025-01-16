#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <map>

#include "Request.h"
#include "Response.h"

using namespace std;

class Router {
 public:
  using Handler = function<void(const Request&, Response&)>;

  void addRoute(const string& method, const string& path, Handler handler);
  void handleRequest(const Request& request, Response& response) const;

 private:
  map<string, map<string, Handler>> routes;
};
#endif  // ROUTER_H
