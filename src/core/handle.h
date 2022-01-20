#pragma once

#include "basic_types.h"
#include "string_hash.h"
#include "type_info.h"

namespace noble_steed
{
using uID = Str_Hash;

class Handle_Base
{
  public:
    Handle_Base(const uID &id);
    Handle_Base(const Handle_Base &copy);
    virtual ~Handle_Base();

    virtual Handle_Base &operator=(const Handle_Base &rhs) = 0;
    
    bool operator==(const Handle_Base &rhs) const;
    bool operator!=(const Handle_Base &rhs) const;
    virtual operator bool() const;

    const uID &id();

  protected:
    uID _id;
};

template<class T>
class Handle : public Handle_Base
{
  public:
    template<class... Args>

    Handle(const uID &id, const SPtr<T> &ptr) : Handle_Base(id), _ptr(ptr)
    {}
    Handle(const uID &id = uID(), T *ptr = nullptr) : Handle_Base(id), _ptr(ptr)
    {}
    Handle(const Handle<T> &copy) : Handle_Base(copy), _ptr(copy._ptr)
    {}
    ~Handle()
    {}

    Handle_Base &operator=(const Handle_Base &rhs)
    {
        auto ptr = &rhs;
        auto cast_ptr = dynamic_cast<const Handle<T> *>(ptr);
        if (cast_ptr)
            *this = *cast_ptr;
        return *this;
    }

    Handle<T> &operator=(Handle<T> rhs)
    {
        std::swap(_ptr, rhs._ptr);
        std::swap(_id, rhs._id);
        return *this;
    }

    bool operator==(const Handle<T> &rhs) const
    {
        return (_ptr == rhs._ptr && Handle_Base::operator==(rhs));
    }

    bool operator==(const T *rhs) const
    {
        return (_ptr.get() == rhs);
    }

    bool operator!=(const Handle<T> &rhs) const
    {
        return !(*this == rhs);
    }

    bool operator!=(const T *rhs) const
    {
        return !(*this == rhs);
    }

    operator bool() const
    {
        return (_ptr && _id);
    }

    T *operator->()
    {
        return _ptr.get();
    }

    const T *operator->() const
    {
        return _ptr.get();
    }

    T &operator*()
    {
        return *_ptr;
    }

    const T &operator*() const
    {
        return *_ptr;
    }

    T *get()
    {
        return _ptr.get();
    }

    const T *get() const
    {
        return _ptr.get();
    }

    unsigned int use_count() const
    {
        return _ptr.use_count();
    }

  private:
    SPtr<T> _ptr;
};

template<class T>
bool operator==(const T *lhs, const Handle<T> &rhs)
{
    return (rhs == lhs);
}

template<class T>
bool operator!=(const T *lhs, const Handle<T> &rhs)
{
    return (rhs != lhs);
}
} // namespace noble_steed