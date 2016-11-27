#ifndef PIPED_FOREACH_HPP
#define PIPED_FOREACH_HPP
namespace piped {

auto each = [](auto f) {
  return [=](auto push) {
    return [=](auto...x) mutable {
      f(x...);
      return push(x...);
    };
  };
};

}
#endif
