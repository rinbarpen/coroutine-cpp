#pragma once

#include <ucontext.h>
#include <inttypes.h>

#define INVALID_HANDLE (-1)

#define __CO_AUTO_ASSIGN__
#undef  __CO_AUTO_ASSIGN__

namespace coroutine
{
enum co_status {
  READY,
  RUNNING,
  SUSPEND,
  DEAD
};

using co_handle = int32_t;

class Coroutine {
public:
  Coroutine();
  virtual ~Coroutine();

  Coroutine(Coroutine&&) = default;
  Coroutine& operator=(Coroutine&& other) = default;

  virtual void resume();
  virtual void yield();
  virtual void destroy();
  virtual void process() = 0;

  #ifdef CO_AUTO_ASSIGN
  // automatically assign a handle and return the handle assigned
  [[nodiscard]] static co_handle get_handle() { m_id = next_id++; return m_id; }
  #else
  void set_id(co_handle handle) { m_id = handle; }
  #endif

  void set_status(co_status status) { m_status = status; }
  co_status get_status() { return m_status; }
  co_handle get_id() { return m_id; }
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
  co_handle m_id;
};

#ifdef CO_AUTO_ASSIGN
static co_handle next_id = 0;
#endif

}  // namespace coroutine

#define BEGIN_COROUTINE(name) \
  class name : public Coroutine {

#define END_COROUTINE(name) \
  };
