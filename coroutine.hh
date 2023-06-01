#pragma once

#include <ucontext.h>
#include <inttypes.h>

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

  virtual void resume();
  virtual void yield();
  virtual void destroy();
  virtual void process() = 0;

  // [[nodiscard]] co_handle get_handle() { m_id = next_id++; return m_id; }

  void set_status(co_status status) { m_status = status; }
  co_status get_status() { return m_status; }
  void set_id(co_handle handle) { m_id = handle; }
  co_handle get_id() { return m_id; }
protected:
  void save_stack();
  void load_stack();
public:
  char *m_buffer;
  ucontext_t m_ctx;
private:
  int m_stack_size;
  int m_capacity;
  co_status m_status;
  co_handle m_id;
};

// static co_handle next_id = 0;

}  // namespace coroutine
