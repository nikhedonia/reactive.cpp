#ifndef PIPED_SCAN_HPP
#define PIPED_SCAN_HPP
#include "pipe.hpp"

namespace piped {

auto scan = [](auto state, auto f) {
  return pipe([state, f](auto& push, auto...x) mutable {
    state = f(state, x...);
    return push( state );
  });
};

}

#endif
