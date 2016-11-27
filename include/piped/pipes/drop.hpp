#ifndef PIPED_DROP_HPP
#define PIPED_DROP_HPP
#include "pipe.hpp"

namespace piped {

auto drop = [](unsigned long i=0) {
  return pipe([i, cond = true](auto& push, auto...x) mutable
    -> decltype(push(x...)) {
    if(i) {
      --i;
      return true;
    }
    return push(x...);
  });
};

}

#endif
