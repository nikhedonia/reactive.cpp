#include <reactive.hpp>
#include <thread-pool.hpp>
#include <iostream>
#include <chrono>
#include <string>

int main() {
  using namespace std::chrono_literals;
  using namespace reactive;
  ThreadPool Pool(4);

  auto p = pipe(
    repeat(4, [](auto x){ return x; } ),
    forEach( [](auto x){ std::cout << "in: " << x <<std::endl;}),
    scan(0, [](auto x, auto y) { return x+y; }),
    map([](auto x){ return x+1;}),
    filter([](auto x) { return x != 4; }),
    wait(1s),
    asyncify(Pool),
    [](auto x) {
      std::cout<< ("out: " + std::to_string( x )  ) << std::endl;
      return x%7;
    }
  ).create();


  p(1);
  p(2);
  //p(3);
  //p(4);

  //p.subscribe([](auto x, auto y) { std::cout << x <<"|" << y<< std::endl; });
 // p(1);
 // p(1);


  return 0;
}
