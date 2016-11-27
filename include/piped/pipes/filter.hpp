#ifndef PIPED_FILTER_HPP
#define PIPED_FILTER_HPP
namespace piped {

auto filter = [](auto f) {
  return [=](auto push) {
    return [=, run=true](auto...x) mutable
    -> decltype(push(x...)) {
      if( run && f(x...) ) return (run=push(x...));
      return run;
    };
  };
};

}
#endif
