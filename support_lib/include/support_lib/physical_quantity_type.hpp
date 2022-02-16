#ifndef PHYSICAL_QUANTITY_HPP
#define PHYSICAL_QUANTITY_HPP

#include "rclcpp/rclcpp.hpp"
#include <string>
#include <iostream>

template <typename TypeParam >
class PhysicalQuantityType
{
public:
  explicit PhysicalQuantityType(typename TypeParam::FieldType const& value) : value(value) {}

  typename TypeParam::FieldType& get() { return value; }
  typename TypeParam::FieldType const& get() const { return value; }

  const PhysicalQuantityType operator+ (const PhysicalQuantityType & );

  std::string info();

private:
  typename TypeParam::FieldType value;

  void operator=( const typename TypeParam::MessageType::SharedPtr msg_ptr ) { value = msg_ptr.value; }
  void fill( typename TypeParam::MessageType & msg ) const { msg.value = value; }
};

#endif
