#include "chain_adaptor.h"

#include <catch.hpp>

#include <functional>
#include <iterator>
#include <list>

using call_utils::chain_adaptor;

TEST_CASE("Simple int variable with addition")
{
  int s = 0;
  auto const add_to_s = [&s](int x) { s += x; };

  auto chainable = chain_adaptor::make(add_to_s);
  
  chainable(1)(2)(4)(8)(16);

  REQUIRE(s == 31);
}

TEST_CASE("List with append operation")
{
  std::list<int> list;
  auto appender = std::back_inserter(list);

  auto chainable = chain_adaptor::make(
    [appender](int x) mutable { *(appender++) = x; });

  chainable(2)(3)(5)(7)(11)(13)(17);

  REQUIRE(list == std::list<int>{2, 3, 5, 7, 11, 13, 17});
}

TEST_CASE("A heterogeneously overloaded function onject")
{
  struct arg_type_counter_t
  {
    arg_type_counter_t()
      : ints{ 0 }
      , bools{ 0 }
      , strings{ 0 }
    {}

    void operator()(int) { ++ints; }
    void operator()(bool) { ++bools; }
    void operator()(char const*) { ++strings; }

    int ints;
    int bools;
    int strings;
  };

  arg_type_counter_t counter;

  auto chainable = chain_adaptor::make(std::ref(counter));

  chainable(0)(true)("")(1)(false)("a b c")(2)(true)(3);

  REQUIRE(counter.ints == 4);
  REQUIRE(counter.bools == 3);
  REQUIRE(counter.strings == 2);
}

TEST_CASE("More than one argument")
{
  struct arity_counter_t
  {
    arity_counter_t()
      : arity_0{ 0 }
      , arity_1{ 0 }
      , arity_3{ 0 }
    {}

    void operator()() { ++arity_0; }
    void operator()(int) { ++arity_1; }
    void operator()(int, int, int) { ++arity_3; }

    int arity_0;
    int arity_1;
    int arity_3;
  };

  arity_counter_t counter;

  auto chainable = chain_adaptor::make(std::ref(counter));

  chainable
    ()
    (0)
    (0, 0, 0)
    ()
    (0)
    (0, 0, 0)
    ()
    (0)
    ();

  REQUIRE(counter.arity_0 == 4);
  REQUIRE(counter.arity_1 == 3);
  REQUIRE(counter.arity_3 == 2);
}

