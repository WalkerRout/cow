
#include <iostream>
#include <algorithm>
#include <ranges>

#include "app.h"

auto main(void) -> int {

  std::string s1{"etalpmet ppc"};
  std::string s2;
  
  std::transform(s1.rbegin(), s1.rend(), std::back_inserter(s2), [](auto c) { 
    return std::toupper(c); 
  });

  auto all_letters = std::ranges::all_of(s2.begin(), s2.end(), [](auto c) { 
    return 'A' <= c && c <= 'Z' || c == ' '; 
  });

  std::cout << all_letters << "\n";
  std::cout << s2 << "\n";

  return 0;
}
