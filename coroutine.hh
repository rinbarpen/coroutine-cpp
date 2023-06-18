#pragma once

#include <ucontext.h>
#include <inttypes.h>

#include <atomic>
#include <functional>
#include <type_traits>

#define CO_AUTO_ASSIGN

#define INVALID_HANDLE (-1)

namespace coroutine
{
enum co_status {
  READY,
  RUNNING,
  SUSPEND,
  DEAD
};

using co_handle = int32_t;

#ifdef CO_AUTO_ASSIGN
static std::atomic<co_handle> co_next_handle{1};
#endif

class Coroutine {
public:
  Coroutine();
  virtual ~Coroutine();

  Coroutine(Coroutine&&) = default;
  Coroutine& operator=(Coroutine&& other) = default;

  void resume();
  void yield();
  void destroy();
  virtual void process() = 0;

  co_handle get_handle() { return m_handle; }
  void set_status(co_status status) { m_status = status; }
  co_status get_status() { return m_status; }

  Coroutine(Coroutine&) = delete;
  Coroutine& operator=(Coroutine&) = delete;

#ifdef CO_AUTO_ASSIGN
private:
#else
public:
#endif
  void set_id(co_handle handle) { m_handle = handle; }
protected:
  void save_stack();
  void load_stack();
public:
  char *m_buffer;
  ucontext_t m_ctx;
private:
  int m_stack_size;  // current stack size
  int m_capacity;  // stack capacity
  co_status m_status;
  co_handle m_handle;
};

}  // namespace coroutine
