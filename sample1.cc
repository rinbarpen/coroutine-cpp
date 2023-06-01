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
  std::shared_ptr<Coroutine> ct1(new Logic1());
  std::shared_ptr<Coroutine> ct2(new Logic2());
  std::shared_ptr<Coroutine> ct3(new Logic3());

  ct1->set_id(1);
  ct2->set_id(2);
  ct3->set_id(3);

  Schedule::instance()->coroutine_new(ct1.get());
  Schedule::instance()->coroutine_new(ct2.get());
  Schedule::instance()->coroutine_new(ct3.get());

  Schedule::instance()->resume(1);
  Schedule::instance()->resume(2);
  Schedule::instance()->resume(3);
  Schedule::instance()->resume(1);
  Schedule::instance()->resume(2);
  Schedule::instance()->resume(3);
  Schedule::instance()->resume(1);
  Schedule::instance()->resume(2);
  Schedule::instance()->resume(3);


  // SingleSchedule::instance()->remove(1);
  // SingleSchedule::instance()->remove(2);
  // SingleSchedule::instance()->remove(3);

  // SingleSchedule::instance()->free_coroutine();
  int count = Schedule::instance()->count();
  printf("count: %d\n", count);

  return 0;
}
