# reactive.cpp
functional reactive library for push based zero-overhead event streams

# Example

```
auto p1 = pipe(
  [](auto x, auto push) { push(x); push(x+1); } ), // push x and x+1
  map( [](auto x) { return x*2; }), // push doubled values 
  scan( 0 , [](auto x, auto y) { return x+y; } ), // accumulate values
  filter( [](auto x){ return x%4 == 0; } ), // only push to next pipe if x%4==0
  buffer<int>() // push to buffer
);

auto p2 = p1.push(
  [](auto buffer, auto push) {  //collect and push groups of two
    auto& b = buffer.get();
    if(b.size() == 2){ 
      push( b[0] , b[1] );
      b.clear();
    }
  })
);

auto observable = publish<void(int,int)>(p2);

observable.subscribe([](auto x, auto y) { std::cout << x << "|" << y << std::endl;  });
observable.push(1);
observable.push(2);
observable.push(3);
observable.push(4);


```
## GOALS
- zero overhead abstractions for static pipes
- supoprt for high level functions like scan, map, merge
- support for dynamic pipes and threads


