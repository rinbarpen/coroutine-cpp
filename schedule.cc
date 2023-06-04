#include <map>
#include <stdexcept>
#include <algorithm>
#include <utility>

#include <ucontext.h>

#include "schedule.hh"

namespace coroutine
{
void Schedule::co_entry(void *vco) {
  Coroutine *co = static_cast<Coroutine*>(vco);
  co->set_status(co_status::RUNNING);
  co->process();
  co->set_status(co_status::SUSPEND);
}

void Schedule::coroutine_new(Coroutine* co) {
  if (m_co_pool.size() > m_capacity) return; // full

  co_handle id = co->get_id();
  if (auto it = m_co_pool.find(id); it != m_co_pool.end()) { m_co_pool.erase(it); }

  getcontext(&co->m_ctx);
  co->m_ctx.uc_stack.ss_flags = 0;
  co->m_ctx.uc_stack.ss_sp = main_stack;
  co->m_ctx.uc_stack.ss_size = MAX_STACK_SIZE;

  co->m_ctx.uc_link = &main_ctx;
  m_co_pool[id] = co;

  makecontext(&(co->m_ctx), (void(*)())&Schedule::co_entry, 1, static_cast<void*>(co));
}

void Schedule::resume(co_handle handle) {
  if (auto it = m_co_pool.find(handle); it != m_co_pool.end()) {
    co_status status = it->second->get_status();
    if (status == co_status::SUSPEND || status == co_status::READY) {
      it->second->resume();
      m_current_handle = handle;
    }
  } else {
    // no matched coroutine
    throw std::runtime_error("no matched coroutine");
  }
}

void Schedule::destroy(co_handle handle) {
  if (auto it = m_co_pool.find(handle); it != m_co_pool.end()) {
    it->second->destroy();
  } else {
    // no matched coroutine
    throw std::runtime_error("no matched coroutine");
  }
}


void Schedule::free_coroutine() {
  for (auto it = m_co_pool.begin(); it != m_co_pool.end();) {
    if (it->second->get_status() == co_status::DEAD) {
      it = m_co_pool.erase(it);
    } else {
      ++it;
    }
  }
}

}  // namespace coroutine
