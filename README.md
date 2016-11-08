# reactive.cpp
functional reactive library for push based zero-overhead event streams

# Example

```
int main() {
  using namespace std::chrono_literals;
  using namespace reactive;
  ThreadPool Pool(4);

  auto p1 = pipe(
    repeat(4, [](auto x){ return x; } ),
    forEach([](auto x){ std::cout << "input: " << x <<std::endl;}),
    scan(0, [](auto x, auto y) { return x+y; }), // pushes sum
    map([](auto x){ return x*10;}), // push x*10
    filter([](auto x) { return x%4==0; }) filter-out if
  );

  auto p2 = p.pipe(
    asyncify(Pool), // make it async by pusing it to worker-queue
    wait(1s) // wait 1s before pushing it to next
  );

  auto sink = p2.pipe([](auto x) {
    std::cout<< ("out: " + std::to_string( x )  ) << std::endl;
  }).create();

  auto exec = pipe(
    just(1), // push just 1
    sink
  ).create();

  exec();

  return 0;
}
```
## GOALS
- zero overhead abstractions for static pipes
- supoprt for high level functions like scan, map, merge
- support for dynamic pipes and threads


