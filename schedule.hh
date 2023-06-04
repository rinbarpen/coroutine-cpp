#pragma once

#include <map>
#include <memory>

#include <ucontext.h>

#include "coroutine.hh"

namespace coroutine
{
class Schedule {
public:
  friend class Coroutine;
  using co_map = std::map<co_handle, Coroutine*>;
  // using co_map = std::map<co_handle, std::unique_ptr<Coroutine>>;

  Schedule() = default;
  Schedule(uint32_t cap) : m_capacity(cap) {}
  ~Schedule() = default;

  static void co_entry(void *vco);
  void coroutine_new(Coroutine* co);

  void resume(co_handle handle);
  void destroy(co_handle handle);

  int count() const { return m_co_pool.size(); }
  char *get_stack_bottom() { return main_stack + MAX_STACK_SIZE; }
  co_handle get_current_handle() const { return m_current_handle; }

  void free_coroutine();
  void set_capacity(uint32_t capacity) { m_capacity = capacity; }
  int get_capacity() const { return m_capacity; }

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
  uint32_t m_capacity{0xffffffff};  // the schedule capacity
};

class SingleSchedule : public Schedule {
public:
  static SingleSchedule* instance() {
    static SingleSchedule *instance = new SingleSchedule();
    return instance;
  }

  SingleSchedule() = default;

  SingleSchedule(SingleSchedule&&) = delete;
  SingleSchedule& operator=(SingleSchedule&) = delete;
  SingleSchedule&& operator=(SingleSchedule&&) = delete;
};

}  // namespace coroutine
