#ifndef PIPED_MERGE_HPP
#define PIPED_MERGE_HPP
#include "../utils/sapplicatives.hpp"
#include "sink.hpp"

namespace piped {

auto fakeSink = [](auto& push) {
  return sink([&push](auto...x) mutable {
    return push(x...);
  });
};

auto createWithFakeSink = [](auto& push) {
  return [&, f = fakeSink(push) ](auto c) {
    return c(f).create();
  };
};


template<class Push, class Composables, class Pipes>
struct Merged {
  Push push;
  Composables c;
  Pipes pipes;


  Merged(Merged const& M)
    : push(M.push)
    , c(M.c)
    , pipes(
        c( amap(createWithFakeSink(push)) )
      )
    {}

  Merged(Push const& p, Composables const& c)
    : push(p)
    , c(c)
    , pipes(
        c( amap(createWithFakeSink(push)) )
      )
  {}

  template<class...X>
  auto operator()(X const...x) {
    pipes(amap([=](auto& p) { return p(x...); }));
    return false;
  }

  template<class...X>
  auto operator()(X const...x)const {
    pipes(amap([=](auto& p) { return p(x...); }));
    return false;
  }


};

auto merge = [](auto...m) {
  return [=](auto push) {

    auto mergables = apply(compose(m)...);

    using Pipes = decltype(
      mergables(
        amap(createWithFakeSink(push))
      )
    );

    auto merged = Merged<
      decltype(push),
      decltype(mergables),
      Pipes
    > (push, mergables);

    return merged;
  };
};

}

#endif
