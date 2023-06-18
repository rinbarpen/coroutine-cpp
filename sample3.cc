#include "schedule.hh"
#include "coroutine.hh"
#include "coroutine-template.hh"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <memory>
#include <functional>

using namespace coroutine;

const char *str = "1234567890";
int n = 0;

void event_handler1()
{
  putchar(' ');
}

void event_handler2()
{
  putchar(str[n++]);
}

void event_handler3()
{
  putchar('\n');
}

std::function<void()> ev_handler1{event_handler1};
std::function<void()> ev_handler2{event_handler2};
std::function<void()> ev_handler3{event_handler3};

std::function<bool()> cf_func1{[]{return true;}};
std::function<bool()> cf_func2{[]{return n < strlen(str);}};
std::function<bool()> cf_func3{[]{return true;}};

auto cf_f1 = []{return true;};
auto cf_f2 = []{return n < strlen(str);};
auto cf_f3 = []{return true;};

// successful normal function
// CO_TEMPLATE1(Logic1, cf_func1, ev_handler1);
// CO_TEMPLATE1(Logic2, cf_func2, ev_handler2);
// CO_TEMPLATE1(Logic3, cf_func3, ev_handler3);

// successful function
// CO_TEMPLATE1(Logic1, cf_func1, event_handler1);
// CO_TEMPLATE1(Logic2, cf_func2, event_handler2);
// CO_TEMPLATE1(Logic3, cf_func3, event_handler3);

// successful
// CO_TEMPLATE2(Logic1, std::function<bool()>{[]{return true;}}, ev_handler1);
// CO_TEMPLATE2(Logic2, std::function<bool()>{[]{return n < strlen(str);}}, ev_handler2);
// CO_TEMPLATE2(Logic3, std::function<bool()>{[]{return true;}}, ev_handler3);

// successful lambda
// CO_TEMPLATE2(Logic1, cf_f1, event_handler1);
// CO_TEMPLATE2(Logic2, cf_f2, event_handler2);
// CO_TEMPLATE2(Logic3, cf_f3, event_handler3);

// successful temporary function
CO_TEMPLATE2(Logic1, std::function<bool()>{[]{return true;}}, event_handler1);
CO_TEMPLATE2(Logic2, std::function<bool()>{[]{return n < strlen(str);}}, event_handler2);
CO_TEMPLATE2(Logic3, std::function<bool()>{[]{return true;}}, event_handler3);

int main()
{
  auto SPtr = SingleSchedule::instance();
  std::shared_ptr<Coroutine> ct1(new Logic1());
  std::shared_ptr<Coroutine> ct2(new Logic2());
  std::shared_ptr<Coroutine> ct3(new Logic3());

  SPtr->coroutine_new(ct1.get());
  SPtr->coroutine_new(ct2.get());
  SPtr->coroutine_new(ct3.get());

  while (n < strlen(str))
  {
    SPtr->resume(ct1->get_handle());
    SPtr->resume(ct2->get_handle());
    SPtr->resume(ct3->get_handle());
  }

  SPtr->free_coroutine();
  return 0;
}
