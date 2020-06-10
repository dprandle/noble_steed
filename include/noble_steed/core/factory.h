#pragma once

#include <noble_steed/io/logger.h>
#include <FreeListAllocator.h>
#include <PoolAllocator.h>
#include <noble_steed/core/common.h>
#include <noble_steed/core/context_obj.h>

namespace noble_steed
{
// The min alloc size was determined by running in debugger and seing what the min size was
// required by the FreeListAllocator
const u8 MIN_ALLOC_SIZE = 16;

// The min align size was determined by running in debugger and seing what the min size was
// required by the FreeListAllocator
const u8 MIN_ALIGN_SIZE = 8;

class Factory
{
  public:
    Factory()
    {}

    virtual ~Factory()
    {}

    virtual Context_Obj * create() = 0;

    virtual Context_Obj * create(const Context_Obj & copy) = 0;

    template<class BaseType>
    BaseType * create_and_cast()
    {
        return static_cast<BaseType*>(create());
    }

    template<class BaseType>
    BaseType * create_and_cast(const Context_Obj & copy)
    {
        return static_cast<BaseType*>(create(copy));
    }

    virtual void destroy(Context_Obj * obj) = 0;
};

template<class Derived_Type>
class Pool_Factory : public Factory
{
  public:
    Pool_Factory(PoolAllocator * alloc) : alloc_(alloc), Factory()
    {}

    Derived_Type * create()
    {
        rttr::type tp = rttr::type::get<Derived_Type>();
        ilog("Allocating {0} bytes for {1} in pool allocator", tp.get_sizeof(), String(tp.get_name()));
        Derived_Type * obj_ptr = static_cast<Derived_Type *>(alloc_->Allocate(tp.get_sizeof()));
        new (obj_ptr) Derived_Type();
        return obj_ptr;
    }

    Derived_Type * create(const Context_Obj & copy)
    {
        rttr::type tp = rttr::type::get<Derived_Type>();
        ilog("Allocating {0} bytes for {1} in pool allocator", tp.get_sizeof(), String(tp.get_name()));
        Derived_Type * obj_ptr = static_cast<Derived_Type *>(alloc_->Allocate(tp.get_sizeof()));
        const Derived_Type & copy_cast = static_cast<const Derived_Type &>(copy);
        new (obj_ptr) Derived_Type(copy_cast);
        return obj_ptr;
    }

    void destroy(Context_Obj * obj)
    {
        rttr::type t = rttr::type::get<Derived_Type>();
        obj->~Context_Obj();
        ilog("De-allocating {0} bytes for {1} in pool allocator", t.get_sizeof(), String(t.get_name()));
        alloc_->Free(obj);
    }

  private:
    PoolAllocator * alloc_;
};

template<class Derived_Type>
class Free_List_Factory : public Factory
{
  public:
    Free_List_Factory(FreeListAllocator * alloc) : alloc_(alloc), Factory()
    {}

    Derived_Type * create()
    {
        rttr::type tp = rttr::type::get<Derived_Type>();
        sizet type_size = rttr::type::get<Derived_Type>().get_sizeof();
        if (type_size < MIN_ALLOC_SIZE)
            type_size = MIN_ALLOC_SIZE;
        ilog("Allocating {0} bytes for {1} in free list allocator", type_size, String(tp.get_name()));
        Derived_Type * ptr = static_cast<Derived_Type *>(alloc_->Allocate(type_size, MIN_ALIGN_SIZE));
        new (ptr) Derived_Type();
        return ptr;
    }

    Derived_Type * create(const Context_Obj & copy)
    {
        rttr::type tp = rttr::type::get<Derived_Type>();
        sizet type_size = rttr::type::get<Derived_Type>().get_sizeof();
        if (type_size < MIN_ALLOC_SIZE)
            type_size = MIN_ALLOC_SIZE;
        ilog("Allocating {0} bytes for {1} in free list allocator", type_size, String(tp.get_name()));
        Derived_Type * ptr = static_cast<Derived_Type *>(alloc_->Allocate(type_size, MIN_ALIGN_SIZE));
        const Derived_Type & copy_cast = static_cast<const Derived_Type &>(copy);
        new (ptr) Derived_Type(copy_cast);
        return ptr;
    }

    void destroy(Context_Obj * obj)
    {
        rttr::type t = rttr::type::get<Derived_Type>();
        obj->~Context_Obj();
        ilog("De-allocating {0} bytes for {1} in free list allocator", t.get_sizeof(), String(t.get_name()));
        alloc_->Free(obj);
    }

  private:
    FreeListAllocator * alloc_;
};
} // namespace noble_steed