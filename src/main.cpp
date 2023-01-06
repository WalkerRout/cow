#include <memory>
#include <vector>
#include <iostream>
#include <functional>

template <typename T>
struct Cow {
  template<typename... Args>
  Cow(Args... args) noexcept: ptr(std::make_shared<T>(args...)) {}
  Cow(std::shared_ptr<T> p) noexcept: ptr(p) {}
  Cow(const Cow &c) noexcept: ptr(c.ptr) {}

  auto operator=(const Cow &rhs) const noexcept {
    ptr = rhs.ptr;
  }

  auto operator=(std::shared_ptr<T> p) const noexcept {
    ptr = p;
  }

  auto operator==(const Cow &rhs) const noexcept {
    return ptr == rhs.ptr;
  }

  auto operator*() const noexcept {
    return *ptr;
  }

  auto pointer() const {
    auto fmt = "%p";
    auto data = ptr.get();
    auto size = std::snprintf(nullptr, 0, fmt, data);
    std::vector<char> buf(size+1);
    
    if (std::snprintf(&buf[0], buf.size(), "%p", ptr.get()) < 0)
      exit(EXIT_FAILURE);
    
    return std::string{buf.begin(), buf.end()};
  }

  auto compare(const Cow &rhs) const noexcept {
    return *ptr == *rhs.ptr;
  }

  auto& write() noexcept {
    ptr = clone();
    return ptr;
  }

  auto clone() noexcept {
    if (ptr.use_count() == 1)
      return ptr;

    return std::make_shared<T>(*ptr); // make clone by dereferencing
  }

  friend auto& operator<<(std::ostream &os, const Cow &c) noexcept {
    os << *c.ptr;
    return os;
  }

private:
  std::shared_ptr<T> ptr;
};

auto main() -> int {
  auto a = Cow<int>(22);
  auto b = a;

  std::cout << "B is " << b << "\n";
  std::cout << "A addr " << a.pointer() << "\n";
  std::cout << "B addr " << b.pointer() << "\n";

  *b.write() = 3;
  
  std::cout << "B now " << b << "\n";
  std::cout << "A addr " << a.pointer() << "\n";
  std::cout << "B addr " << b.pointer() << "\n";

  auto c = b;
  *b.write() = 4;
  
  std::cout << "B now " << b << "\n";
  std::cout << "B data " << b << ", address " << b.pointer() << "\n";
  std::cout << "C data " << c << ", address " << c.pointer() << "\n";

  return 0;
}
