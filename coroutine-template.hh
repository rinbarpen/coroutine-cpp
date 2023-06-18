#pragma once

#include "coroutine.hh"
#include "schedule.hh"

using coroutine::Coroutine, coroutine::Schedule, coroutine::SingleSchedule;

#define BEGIN_COROUTINE(name) \
  class name : public Coroutine { \

#define END_COROUTINE(name) \
  };

// fundamental:
/// - normal function    ( run, {void()} )
/// - function           ( f, std::function<bool()> f = xxx; )
#define CO_TEMPLATE1(name, cf_func, ep_func) \
  BEGIN_COROUTINE(name) \
  using continueFuncType = std::remove_reference_t<decltype(cf_func)>; \
  using eventProcessFuncType = std::remove_reference_t<decltype(ep_func)>; \
  using resultType = std::invoke_result_t<decltype(continueFuncType())>; \
  static_assert(std::is_function_v<continueFuncType> || \
                std::is_same_v<continueFuncType, std::function<bool()>>, \
                "cf_func must be a function"); \
  static_assert(std::is_function_v<eventProcessFuncType> || \
                std::is_same_v<eventProcessFuncType, std::function<void()>>, \
                "ep_func must be a function"); \
  static_assert(std::is_same_v<resultType, bool>, \
                "the result type of cf_func must be 'bool'"); \
  void process() { while (__is_continue()) { __event_handler(); yield(); }} \
  bool __is_continue() { return cf_func(); }  \
  void __event_handler() { ep_func(); } \
  END_COROUTINE(name)

// advanced:
/// - lambda             ( [](){} )
/// - normal function    ( run, {void()} )
/// - temporary function ( std::function<bool()>{xxx} )
#define CO_TEMPLATE2(name, cf_lambda, ep_lambda) \
  BEGIN_COROUTINE(name) \
  void process() { while (__is_continue()) { __event_handler(); yield(); }} \
  bool __is_continue() { return cf_lambda(); }  \
  void __event_handler() { ep_lambda(); } \
  END_COROUTINE(name)
