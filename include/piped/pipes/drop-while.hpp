#ifndef PIPED_DROP_WHILE_HPP
#define PIPED_DROP_WHILE_HPP
#include "pipe.hpp"

namespace piped {

auto dropWhile = [](auto f) {
  return pipe([f, cond = true](auto& push, auto...x) mutable
    -> decltype(push(x...))  {
    if(cond && (cond=f(x...))) return true;
    return push(x...);
  });
};

}

#endif
