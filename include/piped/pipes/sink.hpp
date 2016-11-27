#ifndef PIPED_SINK_HPP
#define PIPED_SINK_HPP

namespace piped {

auto sink = [](auto f) {
  return [=](auto...) {
    return [=](auto...x) mutable {
      return f(x...);
    };
  };
};

}

#endif
