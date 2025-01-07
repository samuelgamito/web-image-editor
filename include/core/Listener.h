#ifndef LISTENER_H
#define LISTENER_H

class Listener {
 public:
  explicit Listener();
  int start();
  void stop();

 private:
  int port;
  int nThreads;
  bool running;
};
#endif  // LISTENER_H
