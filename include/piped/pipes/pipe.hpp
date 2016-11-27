#ifndef PIPED_PIPE_HPP
#define PIPED_PIPE_HPP

namespace piped {
auto pipe = [](auto f) {
  return [=](auto...push) {
    return [=](auto...x) mutable {
      return f(push..., x...);
    };
  };
};
}

#endif
