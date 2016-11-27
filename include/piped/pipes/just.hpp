#ifndef PIPED_JUST_HPP
#define PIPED_JUST_HPP

namespace piped {

template<class P, class X, class...Xs>
auto pushEach(P& push, X x, Xs...xs) {
  return push(x) && pushEach(push, xs...);
}

auto just = [](auto...x) {
  return [=](auto push) {
    return [=](auto...y) mutable {
      return eval(push, x..., y...);
    };
  };
};

}

#endif
