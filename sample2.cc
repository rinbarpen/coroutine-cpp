#include "schedule.hh"
#include "coroutine.hh"
#include "coroutine-template.hh"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <memory>

using coroutine::Coroutine, coroutine::SingleSchedule, coroutine::Schedule;

const char *str = "1234567890";
int n = 0;

BEGIN_COROUTINE(Logic1)
void process() {
  while (true) {
    putchar(' ');
    yield();
  }
}
END_COROUTINE(Logic1)

BEGIN_COROUTINE(Logic2)
void process() {
  while (!is_end()) {
    event_process();
    yield();
  }
}

void event_process() { putchar(str[n++]); }
bool is_end() { return n >= strlen(str); }
END_COROUTINE(Logic2)

BEGIN_COROUTINE(Logic3)
void process() {
  while (true) {
    putchar('\n');
    yield();
  }
}
END_COROUTINE(Logic3)

int main()
{
  auto SPtr = SingleSchedule::instance();
  std::shared_ptr<Coroutine> logic1(new Logic1());
  std::shared_ptr<Coroutine> logic2(new Logic2());
  std::shared_ptr<Coroutine> logic3(new Logic3());

  SPtr->coroutine_new(logic1.get());
  SPtr->coroutine_new(logic2.get());
  SPtr->coroutine_new(logic3.get());

  while (n < strlen(str)) {
    SPtr->resume(logic1->get_handle());
    SPtr->resume(logic2->get_handle());
    SPtr->resume(logic3->get_handle());
  }

  SPtr->free_coroutine();
  return 0;
}
