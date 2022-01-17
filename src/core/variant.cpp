#include "variant.h"

namespace noble_steed
{
Variant::~Variant()
{}

Variant::Variant() : data_()
{}

Variant::Variant(const Variant & rhs) : data_(rhs.data_)
{}

void Variant::swap(Variant & rhs)
{
    std::swap(data_, rhs.data_);
}

Variant & Variant::operator=(Variant rhs_cpy)
{
    swap(rhs_cpy);
    return *this;
}

} // namespace noble_steed
