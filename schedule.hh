#pragma once

#include <ucontext.h>
#include <map>
#include "coroutine.hh"

namespace coroutine
{
class Schedule {
public:
  friend class Coroutine;
  using co_map = std::map<co_handle, Coroutine*>;

  static Schedule* instance() {
    static Schedule instance;
    return &instance;
  }

  static void co_entry(void *vco);
  void coroutine_new(Coroutine* co);

  void resume(co_handle handle);
  void destroy(co_handle handle);

  int count() const { return co_pool.size(); }
  char *get_stack_bottom() { return main_stack + MAX_STACK_SIZE; }
  co_handle get_current_handle() const { return current_handle; }

protected:
  static constexpr int MAX_STACK_SIZE = 1024 * 1024;  // 1mb
public:
  ucontext_t main_ctx;
  char main_stack[MAX_STACK_SIZE];
private:
  co_map co_pool;
  co_handle current_handle;
};

}  // namespace coroutine