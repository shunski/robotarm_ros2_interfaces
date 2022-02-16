#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>
#include "support_lib/physical_quantity_type.hpp"

#include "support_lib/msg/current.hpp"
#include "support_lib/msg/voltage.hpp"
#include "support_lib/msg/power.hpp"
#include "support_lib/msg/servo_pos.hpp"
#include "support_lib/msg/temperature.hpp"
#include "support_lib/msg/id.hpp"

namespace wb{

enum Error{ SUCCESS, ID_NOT_MATCH };
enum ComponentId { SERVO };

enum PhysicalQuantity{ NONE, ID, CURRENT, VOLTAGE, POWER, SERVO_POS, TEMPERATURE };
enum ArithmeticType { ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, DIFFERENTIATION, INTEGRATION };

struct CurrentParameter{
  static const PhysicalQuantity quantity_type = CURRENT;
  using FieldType = double;
  using MessageType = support_lib::msg::Current;
  static const bool is_additive = true;
};

struct VoltageParameter{
  static const PhysicalQuantity quantity_type = VOLTAGE;
  using FieldType = double;
  using MessageType = support_lib::msg::Voltage;
  static const bool is_additive = true;
};

struct PowerParameter{
  static const PhysicalQuantity quantity_type = POWER;
  using FieldType = double;
  using MessageType = support_lib::msg::Power;
  static const bool is_additive = true;
};

struct TemperatureParameter{
  static const PhysicalQuantity quantity_type = TEMPERATURE;
  using FieldType = int;
  using MessageType = support_lib::msg::Temperature;
  static const bool is_additive = false;
};

struct ServoPosParameter{
  static const PhysicalQuantity quantity_type = SERVO_POS;
  using FieldType = double;
  using MessageType = support_lib::msg::ServoPos;
  static const bool is_additive = true;
};

struct IdParameter{
  static const PhysicalQuantity quantity_type = ID;
  using FieldType = int;
  using MessageType = support_lib::msg::Id;
  static const bool is_additive = false;
};

const std::string get_description( const PhysicalQuantity quontity_type ){
  switch (quontity_type) {
    case CURRENT:
      return "[current]";
    case VOLTAGE:
      return "[voltage]";
    case POWER:
      return "[power]";
    case SERVO_POS:
      return "[servo_position]";
    case TEMPERATURE:
      return "[temperature]";
    default:
      return "NONE";
  }
}

const std::string get_unit( const PhysicalQuantity quontity_type ){
  switch (quontity_type) {
    case CURRENT:
      return "[A]";
    case VOLTAGE:
      return "[V]";
    case POWER:
      return "[W]";
    case TEMPERATURE:
      return "[deg/C]";
    case SERVO_POS:
      return "[deg]";
    case ID:
      return "";
    default:
      return "NONE";
  }
}

using Current = PhysicalQuantityType<CurrentParameter>;
using Voltage = PhysicalQuantityType<VoltageParameter>;
using Power = PhysicalQuantityType<PowerParameter>;
using Temperature = PhysicalQuantityType<TemperatureParameter>;
using ServoPos = PhysicalQuantityType<ServoPosParameter>;
using Id = PhysicalQuantityType<IdParameter>;



Power compute(Voltage v, Current i){ return Power(v.get() * i.get()); }
Power compute(Current i, Voltage v){ return compute(v, i); }

}


// Numeric Literals: defined outside of the namespace
wb::Current operator"" _A(long double i)
{
  return wb::Current(i);
}

wb::Voltage operator"" _V(long double v)
{
  return wb::Voltage(v);
}

wb::Power operator"" _W(long double p)
{
  return wb::Power(p);
}

wb::Temperature operator"" _C(unsigned long long  t)
{
  return wb::Temperature(t);
}

wb::ServoPos operator"" _deg(long double pos)
{
  if (!(pos >= 0 && pos < 359)){
    // To Do
  }
  return wb::ServoPos(pos);
}

#include "support_lib/physical_quantity_type_impl.hpp"

#endif
