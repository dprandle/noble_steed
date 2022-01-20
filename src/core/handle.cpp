#include "handle.h"
namespace noble_steed
{
Handle_Base::Handle_Base(const uID &id) : _id(id)
{}

Handle_Base::Handle_Base(const Handle_Base &copy) : _id(copy._id)
{}

Handle_Base::~Handle_Base()
{}

const uID &Handle_Base::id()
{
    return _id;
}

bool Handle_Base::operator==(const Handle_Base &rhs) const
{
    return _id == rhs._id;
}

bool Handle_Base::operator!=(const Handle_Base &rhs) const
{
    return !(*this == rhs);
}

Handle_Base::operator bool() const
{
    return _id;
}
} // namespace noble_steed