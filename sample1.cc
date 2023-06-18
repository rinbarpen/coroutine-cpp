#include "coroutine.hh"
#include "schedule.hh"

#include <memory>
#include <cstdio>

using namespace coroutine;

class Logic1 : public Coroutine {
void process() {
  puts("1");
  yield();
  puts("4");
  yield();
  puts("7");
}
};

class Logic2 : public Coroutine {
void process() {
  puts("2");
  yield();
  puts("5");
  yield();
  puts("8");
}
};

class Logic3 : public Coroutine {
void process() {
  puts("3");
  yield();
  puts("6");
  yield();
  puts("9");
}
};

int main()
{
  auto SPtr = SingleSchedule::instance();
  std::shared_ptr<Coroutine> ct1(new Logic1());
  std::shared_ptr<Coroutine> ct2(new Logic2());
  std::shared_ptr<Coroutine> ct3(new Logic3());

  // ct1->set_id(1);
  // ct2->set_id(2);
  // ct3->set_id(3);

  SPtr->coroutine_new(ct1.get());
  SPtr->coroutine_new(ct2.get());
  SPtr->coroutine_new(ct3.get());

  SPtr->resume(1);
  SPtr->resume(2);
  SPtr->resume(3);
  SPtr->resume(1);
  SPtr->resume(2);
  SPtr->resume(3);
  SPtr->resume(1);
  SPtr->resume(2);
  SPtr->resume(3);


  SPtr->destroy(1);
  SPtr->destroy(2);
  SPtr->destroy(3);

  SPtr->free_coroutine();
  int count = SPtr->count();
  printf("count: %d\n", count);

  return 0;
}
