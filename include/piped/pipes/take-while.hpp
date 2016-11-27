#ifndef PIPED_TAKE_WHILE_HPP
#define PIPED_TAKE_WHILE_HPP
#include "pipe.hpp"

namespace piped {

auto takeWhile = [](auto f) {
  return pipe([f, cond = true](auto& push, auto...x) mutable
    -> decltype(push(x...))  {
    if(cond && (cond=f(x...))) return push(x...);
    return false;
  });
};

}

#endif
