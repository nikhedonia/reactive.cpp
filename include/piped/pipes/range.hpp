#ifndef PIPED_RANGE_HPP
#define PIPED_RANGE_HPP

namespace piped {

auto range = [](auto range) {
  return [=](auto push) {
    return [=]() mutable {
      for(auto const& x : range) {
        if(!push(x)) return false;
      }
			return true;
    };
  };
};

}

#endif
