#include <ucontext.h>
#include <map>
#include "schedule.hh"
#include <cstdio>

namespace coroutine
{
void Schedule::co_entry(void *vco) {
  Coroutine *co = static_cast<Coroutine*>(vco);
  co->set_status(co_status::RUNNING);
  co->process();
  co->set_status(co_status::SUSPEND);
}

void Schedule::coroutine_new(Coroutine* co) {
  co_handle id = co->get_id();
  if (auto it = co_pool.find(id); it != co_pool.end()) { co_pool.erase(it); }

  getcontext(&co->m_ctx);
  co->m_ctx.uc_stack.ss_flags = 0;
  co->m_ctx.uc_stack.ss_sp = &main_stack[0];
  co->m_ctx.uc_stack.ss_size = MAX_STACK_SIZE;

  co->m_ctx.uc_link = &main_ctx;
  co_pool[id] = co;

  makecontext(&(co->m_ctx), (void(*)())&Schedule::co_entry, 1, static_cast<void*>(co));
  // swapcontext(&main_ctx, &co->m_ctx);
}

void Schedule::resume(co_handle handle) {
  if (co_pool[handle] && (co_pool[handle]->get_status() == co_status::SUSPEND || co_pool[handle]->get_status() == co_status::READY)) {
    co_pool[handle]->resume();
    current_handle = handle;
  } else {
    // handle isn't available
  }
}

void Schedule::destroy(co_handle handle) {
  if (co_pool[handle]) {
    co_pool[handle]->destroy();
    current_handle = -1;
  } else {
    // handle isn't available
  }
}

}
