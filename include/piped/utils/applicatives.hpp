#ifndef PIPED_APPLICATIVES_HPP
#define PIPED_APPLICATIVES_HPP

namespace piped {

auto apply = [](auto...x) {
  return [=](auto f) {
    return f(x...);
  };
};

auto amap = [](auto f) {
  return [=](auto...x) {
    return apply(f(x)...);
  };
};

auto acat = [](auto...x) {
  return [=](auto...y) {
    return apply(x..., y...);
  };
};

}

#endif
