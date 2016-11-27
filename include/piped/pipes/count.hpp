#ifndef PIPED_COUNT_HPP
#define PIPED_COUNT_HPP
#include "pipe.hpp"
namespace piped {

auto count = [](unsigned long i=0) {
  return pipe([i](auto& push, auto...x) mutable {
    return push(i++, x...);
  });
};

}

#endif
