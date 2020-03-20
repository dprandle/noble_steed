#pragma once

#include <noble_steed/core/logger.h>
#include <FreeListAllocator.h>
#include <PoolAllocator.h>
#include <noble_steed/core/common.h>

namespace noble_steed
{
class Resource;
class Component;
class System;

// The min alloc size was determined by running in debugger and seing what the min size was
// required by the FreeListAllocator
const u8 MIN_ALLOC_SIZE = 16;

// The min align size was determined by running in debugger and seing what the min size was
// required by the FreeListAllocator
const u8 MIN_ALIGN_SIZE = 8;

class Factory
{
  public:
    enum Factory_Type
    {
        Type_Component,
        Type_System,
        Type_Resource
    };

    Factory(Factory_Type fac_type) : fac_type_(fac_type)
    {}

    virtual ~Factory()
    {}

    Factory_Type type()
    {
        return fac_type_;
    }

  private:
    Factory_Type fac_type_;
};

class Resource_Factory : public Factory
{
  public:
	Resource_Factory():
		Factory(Type_Resource)
	{}

	virtual Resource * create() = 0;

	virtual Resource * create(const Resource & copy) = 0;
};

template<class Obj_Type>
class Resource_Factory_Type : public Resource_Factory
{
  public:
	Resource_Factory_Type(PoolAllocator * alloc) : alloc_(alloc),
		Resource_Factory()
	{}
	
	Resource * create()
	{
        rttr::type tp = rttr::type::get<Obj_Type>();
        ilog("Allocating {0} bytes for {1}", tp.get_sizeof(), String(tp.get_name()));
        Obj_Type * res_ptr = static_cast<Obj_Type *>(alloc_->Allocate(tp.get_sizeof()));
        new (res_ptr) Obj_Type();
        return res_ptr;
    }

    Resource * create(const Resource & copy)
    {
        rttr::type tp = rttr::type::get<Obj_Type>();
        ilog("Allocating {0} bytes for {1}", tp.get_sizeof(), String(tp.get_name()));
        Obj_Type * res_ptr = static_cast<Obj_Type *>(alloc_->Allocate(tp.get_sizeof()));
        const Obj_Type & copy_cast = static_cast<const Obj_Type &>(copy);
        new (res_ptr) Obj_Type(copy_cast);
        return res_ptr;
    }
    private:
    PoolAllocator * alloc_;
};

class System_Factory : public Factory
{
  public:
	System_Factory():
		Factory(Type_System)
	{}

	virtual System * create() = 0;

	virtual System * create(const System & copy) = 0;
};

template<class Obj_Type>
class System_Factory_Type : public System_Factory
{
  public:
	System_Factory_Type(FreeListAllocator * alloc) : alloc_(alloc),
		System_Factory()
	{}
	
	System * create()
	{
        rttr::type tp = rttr::type::get<Obj_Type>();
        sizet type_size = rttr::type::get<Obj_Type>().get_sizeof();
        if (type_size < MIN_ALLOC_SIZE)
            type_size = MIN_ALLOC_SIZE;
        ilog("Allocating {0} bytes for {1}", type_size, String(tp.get_name()));
        Obj_Type * ptr = static_cast<Obj_Type*>(alloc_->Allocate(type_size, MIN_ALIGN_SIZE));
        new (ptr) Obj_Type();
        return ptr;
    }

    System * create(const System & copy)
    {
        rttr::type tp = rttr::type::get<Obj_Type>();
        sizet type_size = rttr::type::get<Obj_Type>().get_sizeof();
        if (type_size < MIN_ALLOC_SIZE)
            type_size = MIN_ALLOC_SIZE;
        ilog("Allocating {0} bytes for {1}", type_size, String(tp.get_name()));
        Obj_Type * ptr = static_cast<Obj_Type*>(alloc_->Allocate(type_size, MIN_ALIGN_SIZE));
        const Obj_Type & copy_cast = static_cast<const Obj_Type &>(copy);
        new (ptr) Obj_Type(copy_cast);
        return ptr;
    }

    private:
    FreeListAllocator * alloc_;
};

class Component_Factory : public Factory
{
  public:
	Component_Factory():
		Factory(Type_Component)
	{}

	virtual Component * create() = 0;

	virtual Component * create(const Component & copy) = 0;
};

template<class Obj_Type>
class Component_Factory_Type : public Component_Factory
{
  public:
	Component_Factory_Type(PoolAllocator * alloc) : alloc_(alloc),
		Component_Factory()
	{}
	
	Component * create()
	{
        rttr::type tp = rttr::type::get<Obj_Type>();
        ilog("Allocating {0} bytes for {1}", tp.get_sizeof(), String(tp.get_name()));
        Obj_Type * comp_ptr = static_cast<Obj_Type *>(alloc_->Allocate(tp.get_sizeof()));
        new (comp_ptr) Obj_Type();
        return comp_ptr;
    }

    Component * create(const Component & copy)
    {
        rttr::type tp = rttr::type::get<Obj_Type>();
        ilog("Allocating {0} bytes for {1}", tp.get_sizeof(), String(tp.get_name()));
        Obj_Type * comp_ptr = static_cast<Obj_Type *>(alloc_->Allocate(tp.get_sizeof()));
        const Obj_Type & copy_cast = static_cast<const Obj_Type &>(copy);
        new (comp_ptr) Obj_Type(copy_cast);
        return comp_ptr;
    }
private:
    PoolAllocator * alloc_;
};
} // namespace noble_steed