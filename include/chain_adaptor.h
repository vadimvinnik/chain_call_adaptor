#pragma once

#include <utility>

namespace call_utils
{

class chain_adaptor
{
private:
  template <typename F>
  class impl
  {
  public:
    impl(F func)
      : m_func(func)
    {}

    template <typename... Args>
    impl& operator() (Args&& ...args)
    {
      m_func(std::forward<Args>(args)...);
      return *this;
    }

    private:
      F m_func;
  };

public:
  template <typename F>
  static impl<F> make(F func)
  {
    return impl<F>(func);
  }
};

}
