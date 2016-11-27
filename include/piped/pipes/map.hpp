#ifndef PIPED_MAP_HPP
#define PIPED_MAP_HPP
namespace piped {

auto map = [](auto f) {
  return [=](auto push) {
    return [=](auto...x) mutable {
      return push(f(x...));
    };
  };
};

}
#endif
