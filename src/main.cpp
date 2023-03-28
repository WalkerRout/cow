#include <memory>
#include <vector>
#include <iostream>
#include <functional>

template <typename T>
struct Cow {
  template<typename... Args>
  Cow(Args... args): ptr(std::make_shared<T>(args...)) {}
  Cow(std::shared_ptr<T> p): ptr(p) {}

  // Rule of Five
  Cow(const Cow &c): ptr(c.ptr) {}
  Cow(Cow&& c) noexcept: ptr(std::move(c.ptr)) {}
  auto operator=(const Cow &rhs) const { ptr = rhs.ptr; return *this; }
  auto operator=(Cow&& rhs) const noexcept { ptr = std::move(rhs.ptr); return *this; }
  virtual ~Cow() = default;

  // read
  operator T() const noexcept { return *ptr; }
  operator const T&() const noexcept { return *ptr; }
  operator T&&() = delete;

  auto operator*() const noexcept -> const T& { return *ptr; }
  // end read

  // write
  auto operator=(const T& rhs) {
    ptr = clone();
    *ptr = rhs;
    return *this;
  }

  auto operator=(T&& rhs) {
    ptr = clone();
    *ptr = std::move(rhs);
    return *this;
  }
  // end write

  auto clone() const {
    if (ptr.use_count() == 1)
      return ptr;

    return std::make_shared<T>(*ptr); // make clone by dereferencing
  }

  auto operator==(const Cow &rhs) const { return ptr == rhs.ptr; }
  auto compare(const Cow &rhs) const { return *ptr == *rhs.ptr; }

  auto pointer() const -> std::string {
    auto fmt = "%p";
    auto data = ptr.get();
    auto size = std::snprintf(nullptr, 0, fmt, data);
    std::vector<char> buf(size+1);
    
    if (std::snprintf(&buf[0], buf.size(), "%p", ptr.get()) < 0)
      exit(EXIT_FAILURE);
    
    return std::string{buf.begin(), buf.end()};
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

  b = 3;
  
  std::cout << "B now " << b << "\n";
  std::cout << "A addr " << a.pointer() << "\n";
  std::cout << "B addr " << b.pointer() << "\n";

  auto c = b;
  b = 4;

  std::cout << "B now " << b << "\n";
  std::cout << "B data " << b << ", address " << b.pointer() << "\n";
  std::cout << "C data " << c << ", address " << c.pointer() << "\n";

  return 0;
}
