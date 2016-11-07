#ifndef REACTIVE_HPP
#define REACTIVE_HPP

#include <memory>
#include <functional>
#include <vector>
#include <iostream>

namespace reactive {
namespace impl {
template<class F0>
auto compose(F0 f) {
  return [=](auto...x) {
    return f(x...);
  };
}


template<class F0, class...F>
auto compose(F0 f0, F...f) {
  return [=](auto...x) {
    return compose(f...)(f0(x...));
  };
}

template<class F0>
auto composeR(F0 f0) {
  return [=](auto...x) mutable {
    return f0(x...);
  };
}

template<class F0, class...F>
auto composeR(F0 f0, F...f) {
  return [=](auto...x) mutable {
    return f0(x..., composeR(f...) );
  };
}



}

struct NOP {
  template<class X>
  void operator()(X...)const {}
} nop;

auto id = [](auto x) { return x; };

template<class T, class F, class...X>
auto returnBool(T, F f, X...x) -> decltype( id(f(x...)) ) {
  return f(x...);
}

template<class F, class...X>
auto returnBool(bool R, F f, X...x) -> bool {
  f(x...);
  return R;
}


auto makeReturnBool = [](auto f) {
  return [=](auto...x) {
    return returnBool(true, f, x...);
  };
};


auto compose = [](auto...x) { return impl::compose(x...); };
auto composeR= [](auto...x) { return impl::composeR(x...); };

auto forkStream = [](auto...f) {
  return [=](auto...x) {
    nop(makeReturnBool(f)(x...)...);
  };
};

auto apply = [](auto...x) {
  return [=](auto f) {
    return f(x...);
  };
};

auto cat = [](auto...x) {
  return [=](auto...y) {
    return apply(x..., y...);
  };
};

template<class A=decltype(apply())>
struct Pipeline {
  A a = apply();

  template<class...F>
  auto pipe(F...f) {
    auto b = a(cat)(makeReturnBool(f)...); // combine both tuples
    return Pipeline<decltype(b)>{b};
  }

  auto create() {
    return a(composeR);
  }

};


auto pipe = [](auto...pipes) {
  auto a = apply(makeReturnBool(pipes)...);
  return Pipeline<decltype(a)>{a};
};


auto map = [](auto f){ return [f](auto x, auto push){ return push(f(x)); }; };

auto filter = [](auto f){ return [f](auto x, auto push){ if(f(x)) return push(x); return true; }; };

auto scan = [](auto init, auto f) {
  auto s = std::make_shared<decltype(init)>(init);
  auto& state = *s;
  return [&state, s, f](auto x, auto push) {
    return push(state = f(state, x));
  };
};

auto repeat = [](auto n, auto f) {
  return [f, n](auto x, auto push) {
    for(decltype(n) i=0; i < n; ++i) {
      if( !push( f(x) ) ) {
        return false;
      }
    }
    return true;
  };
};

auto enumerate = [](auto init=0) {
  auto s = std::make_shared<decltype(init)>(init);
  auto& i = *s;
  return [i,s](auto x, auto push){
    return push(x,i++);
  };
};

auto take = [](auto n) {
  auto s = std::make_shared<decltype(n)>(n);
  auto& i = *s;
  return [&i,s](auto x, auto push) {
    if(i) {
      --i;
      return push(x);
    }
    return false;
  };
};

auto drop = [](auto n) {
  auto s = std::make_shared<decltype(n)>(n);
  auto& i = *s;
  return [&i,s](auto x, auto push) {
    if(i) {
      i--;
    } else {
      return push(x);
    }
  };
};

auto takeUntil = [](auto f) {
  return [f](auto x, auto push) {
    if( f(x) ) { return push(x); }
    return false;
  };
};

auto dropUntil = [](auto f) {
  return [f](auto x, auto push) {
    if( f(x) ) { return true; }
    return push(x);
  };
};



auto forEach = [](auto f) {
  return [=](auto x, auto push) {
    f(x);
    return push(x);
  };
};




template<class T>
auto buffer(){
  auto vp = std::make_shared<std::vector<T>>();
  auto& v = *vp;
  return [vp, &v](auto x, auto push) {
    v.push_back(x);
    return push(std::ref(v));
  };
}


template<class N, class P>
struct Broker {
  N next;
  P pipe;

  template<class F>
  void subscribe(F f) {
    next = f;
  }

  template<class...X>
  void operator()(X...x){
    pipe(x...);
  }
};



template<class F, class P>
auto publish(P p) {
  auto uf = std::make_shared<std::function<F>>(nop);
  auto& f = *uf;

  auto pipe = p.pipe([uf](auto...x){ return (*uf)(x...); }).create();

  return Broker<decltype(f)&, decltype(pipe)>{f, pipe};
};


}

#endif
