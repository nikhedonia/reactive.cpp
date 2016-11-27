#include <piped/pipes.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

int main() {
  using namespace std::chrono_literals;
  using namespace piped;

  std::vector<int> v = {1,2,3,4,5,6,7,8,9};

  auto log = [](auto name) {
    return each([=](auto x) { std::cout << name <<":" << x << std::endl; } );
  };


  auto composedPipe  = compose(
    drop(1),
    log("after drop"),
    filter([](auto x) { return x % 2; }),
    log("after filter"),
    map([](auto x){ return x*100; }),
    take(3)
  );

  auto exec = compose(
		range(v),
    log("after range"),
    composedPipe,
    sink([](auto x) {
      std::cout<< ("out: " + std::to_string( x )  ) << std::endl;
  	})
	).create();


  exec();


  return 0;
}
