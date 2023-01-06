
#include <iostream>
#include <memory>

template<typename T>
struct App {
  App() { }
  ~App() { }
  
  auto run() {
    init();
    loop();
    exit();
  }

private:
  auto init() { }
  auto loop() { }
  auto exit() { }

private:
  std::unique_ptr<T> context;
};

