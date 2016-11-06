#include <reactive.hpp>

#include <iostream>

int main() {
  using namespace reactive;


  auto p = publish<void(int,int)>(pipe()
    .pipe( repeat(4, [](auto x){ return x; } )) // push incoming value 4 times
    //.pipe( forEach( [](auto x){ std::cout << x;}))
    .pipe( scan(0, [](auto x, auto y) { return x+y; }) ) // accumulate values
    .pipe( map([](auto x){ return x+1;}) ) // add one to each value
    //.pipe( filter([](auto x) { return x != 4; }) )
    .pipe( buffer<int>() ) // add to queue and push the queue to next pipe
    .pipe( [](auto buffer, auto push) { // collect and push a pair of values
      auto& b = buffer.get();
      if(b.size() == 2) {
        push(b[0], b[1]);
        b.clear();
        return 0;
      }
      return 1;
    })
  );

  p.subscribe([](auto x, auto y) { std::cout << x <<" " << y<< std::endl; });
  p(1);
  p(1);
  p(1);
  p(1);


  return 0;
}
