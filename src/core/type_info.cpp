#include "type_info.h"

namespace noble_steed
{
Type_Info::Type_Info(const String &name, sizet size) : _cached_hash(name), _name(name), _size(size)
{}

Type_Info::Type_Info() : _cached_hash(), _name(), _size(0)
{}

const String &Type_Info::name() const
{
    return _name;
}

sizet Type_Info::hash_code() const
{
    return _cached_hash.value();
}

sizet Type_Info::size() const
{
    return _size;
}

Type_Info::operator bool() const
{
    return _cached_hash;
}

bool Type_Info::operator==(const Type_Info &rhs) const
{
    return (_cached_hash == rhs._cached_hash);
}

} // namespace noble_steed