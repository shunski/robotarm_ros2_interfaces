#include "support_lib/support_lib.hpp"
#include <iostream>

void fn( wb::Voltage v, wb::Current i ){
  std::cout << "v = ";
  std::cout << v.info();
  std::cout << "c = ";
  std::cout << i.info();
  wb::ServoPos pos = wb::ServoPos(120);
  std::cout << "pos = ";
  std::cout << pos.info();
  wb::Power p1 = compute(v, i);
  wb::Power p2 = compute(i, v);
  std::cout << "p1 = ";
  std::cout << p1.info();
  std::cout << "p2 = ";
  std::cout << p2.info();
  std::cout << wb::Temperature(12_C).info();

  std::cout << std::endl;
  wb::Power p = p1 + p2;
  std::cout << "p = p1 + p2 = ";
  std::cout << p.info();
}

int main()
{
  fn(wb::Voltage(10.0_V), wb::Current(20.0_A));

  return 0;
}
