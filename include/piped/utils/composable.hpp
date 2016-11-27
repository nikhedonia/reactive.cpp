#ifndef PIPED_COMPOSABLE_HPP
#define PIPED_COMPOSABLE_HPP
#include "applicatives.hpp"
#include "chain.hpp"
#include "wrap.hpp"

namespace piped {

template<class A = decltype(apply())>
struct Composable {
  A a;

  template<class F>
  constexpr auto apply(F f)const {
    return a(f);
  }

  template<class F>
  constexpr auto map(F f)const {
    auto b = a.apply( a(amap(f)) );
    using B = decltype(b);
    return Composable<B>{b};
  }

  template<class T>
  constexpr auto operator()(Composable<T> c)const {
    auto b = c.apply( a(acat) );
    using B = decltype(b);
    return Composable<B>{b};
  }

  template<class P>
  constexpr auto operator()(P p)const {
    auto b = a(acat)(p);
    using B = decltype(b);
    return Composable<B>{b};
  }

  template<class P, class...Ps>
  constexpr auto operator()(P p, Ps...ps)const {
    return (*this)(p)(ps...);
  }

  template<class W>
  constexpr auto create(W w)const {
    return apply([=](auto&...f){
      return chain(w(f)...);
    });
  }

  constexpr auto create()const {
    return create(wrapResult);
  }

};

auto compose = [](auto...x) {
  return (Composable<>{apply()})(x...);
};

}

#endif
