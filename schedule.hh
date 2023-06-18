#pragma once

#include <ucontext.h>

#include <map>
#include <memory>

#include "coroutine.hh"
#include "singleton.hh"

#define _CLASS_REMOVE_COPY(class) \
  class(class&) = delete; \
  class& operator=(class&) = delete;

#define _CLASS_REMOVE_MOVE(class) \
  class(class&&) = delete; \
  class& operator=(class&&) = delete;


namespace coroutine
{
class Schedule {
public:
  friend class Coroutine;
  using co_map = std::map<co_handle, Coroutine*>;
  // using co_map = std::map<co_handle, std::unique_ptr<Coroutine>>;

  Schedule() = default;
  ~Schedule() = default;

  static void co_entry(void *vco);
  void coroutine_new(Coroutine* co);

  void resume(co_handle handle);
  void destroy(co_handle handle);

  int count() const { return m_co_pool.size(); }
  char *get_stack_bottom() { return main_stack + MAX_STACK_SIZE; }
  co_handle get_current_handle() const { return m_current_handle; }

  void free_coroutine();

  Schedule(Schedule&) = delete;
  Schedule& operator=(Schedule&) = delete;
protected:
  static constexpr int MAX_STACK_SIZE = 1024 * 1024;  // 1mb
public:
  ucontext_t main_ctx;
  char main_stack[MAX_STACK_SIZE];
private:
  co_map m_co_pool;
  co_handle m_current_handle;
};

using SingleSchedule = Singleton<Schedule>;

}  // namespace coroutine
