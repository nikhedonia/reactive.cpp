#ifndef PIPED_WRAP_HPP
#define PIPED_WRAP_HPP
#include <type_traits>

namespace piped {

template<class F>
auto wrap(F f) {
  return [=](auto&&...x) mutable {
    using namespace std;
    using retType = decltype( f(x...) );
    if constexpr ( is_same<retType, void>{} ) {
      f(x...);
      return true;
    } else {
      return f(x...);
    }
  };
}

auto wrapResult = [](auto f) {
  return [f](auto&&...x) {
    return wrap( f(x...) );
  };
};

}

#endif
