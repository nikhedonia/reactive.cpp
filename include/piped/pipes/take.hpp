#ifndef PIPED_TAKE_HPP
#define PIPED_TAKE_HPP
#include "pipe.hpp"

namespace piped {

auto take = [](unsigned long i=0) {
  return pipe([i](auto& push, auto...x) mutable
    -> decltype(push(x...))  {
    if(i) {
      --i;
      return push(x...);
    }
    return false;
  });
};

}

#endif
