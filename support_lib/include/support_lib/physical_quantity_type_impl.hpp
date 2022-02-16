#ifndef PHHYSICAL_QUANTITY_TYPE_IMPL_HPP
#define PHHYSICAL_QUANTITY_TYPE_IMPL_HPP

template <typename TypeParam >
const PhysicalQuantityType<TypeParam> PhysicalQuantityType<TypeParam>::operator+ (const PhysicalQuantityType & summand )
{
  if ( !TypeParam::is_additive ) {
    std::string error = "ERROR: Addition cannot be performed on " + wb::get_description(TypeParam::quantity_type) + ". ";
    std::cout << error << std::endl;
  }

  return PhysicalQuantityType(this->value + summand.value);
}



template <typename TypeParam >
std::string PhysicalQuantityType<TypeParam>::info(){
  return wb::get_description(TypeParam::quantity_type)+ ": " + std::to_string(value) + wb::get_unit(TypeParam::quantity_type) + "\n";
}

#endif
