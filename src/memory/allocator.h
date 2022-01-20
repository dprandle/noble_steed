#pragma once

#include "../core/basic_types.h"

namespace noble_steed
{

class Allocator
{
  public:
    Allocator(sizet total_size);

    Allocator(const Allocator & copy) = delete;

    virtual ~Allocator();

    virtual void *allocate(sizet size, sizet alignment = 0) = 0;

    template<class T>
    T * allocate(sizet count, sizet alignment = 0)
    {
        return static_cast<T*>(allocate(sizeof(T) * count, alignment));
    }

    template<class T,class... Args>
    void construct(T * ptr, Args&&... args)
    {
        new (ptr) T(std::forward<Args>()...);
    }

    template<class T, class... Args>
    T * allocate_and_construct(sizet alignment = 0)
    {
        auto ret = allocate<T>(1, alignment);
        construct(ret, std::forward<Args>()...);
        return ret;
    }

    virtual void free(void *ptr) = 0;

    template<class T>
    void destroy_and_free(T * ptr)
    {
        destroy(ptr);
        free(ptr);
    }

    template<class T>
    void destroy(T * ptr)
    {
        ptr->~T();
    }

    virtual void init() = 0;

  protected:
    sizet _total_size;
    sizet _used;
    sizet _peak;
};

struct Alloc_Deleter
{
    Alloc_Deleter(Allocator * alloc): _alloc(alloc) {}
    template<class T>
    void operator()(T* ptr) {_alloc->destroy_and_free(ptr); }
    Allocator * _alloc;
};

}