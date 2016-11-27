#ifndef PIPED_CHAIN_HPP
#define PIPED_CHAIN_HPP

namespace piped {

template<class F0, class...F>
auto chain(F0 f0, F&&...f) {
  if constexpr ( sizeof...(f) > 0 ) {
    return f0( chain(f...) );
  } else {
    return f0();
  }
}

}

#endif

