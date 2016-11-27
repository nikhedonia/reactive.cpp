#ifndef PIPED_DOWHILE_HPP
#define PIPED_DOWHILE_HPP
#include "pipe.hpp"

namespace piped {

auto doWhile = [] {
  return pipe([run=true](auto& push, auto...x) mutable {
    if(run) {
      run = push(x...);
    }
    return run;
  });
};

}

#endif
