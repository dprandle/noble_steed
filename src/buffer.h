#pragma once

#include "basic_types.h"
#include "nsmemory.h"
#include "nsdebug.h"

namespace noble_steed
{
template<class T>
struct buffer
{
    sizet cap{};
    sizet size{};
    T *data{};
};

template<class T, sizet N>
struct array
{
    using iterator = T *;
    using const_iterator = const T *;
    using value_type = T;

    T mem[N];
    buffer<T> buf{N, 0, mem};

    inline iterator begin()
    {
        return buf->data;
    }

    inline const_iterator begin() const
    {
        return buf->data;
    }

    inline iterator end()
    {
        return (buf->data + buf->size);
    }

    inline const_iterator end() const
    {
        return (buf->data + buf->size);
    }

    inline sizet size() const
    {
        return buf->size;
    }

    inline bool empty() const
    {
        return (buf->size == 0);
    }

    inline sizet capacity() const
    {
        return buf->cap;
    }

    inline const T &operator[](sizet ind) const
    {
        return buf.data[ind];
    }
    inline T &operator[](sizet ind)
    {
        return buf.data[ind];
    }
};

template<class T>
struct vector
{
    using iterator = T *;
    using const_iterator = const T *;
    using value_type = T;

    vector(mem_store *alloc_ = nullptr)
    {
        if (alloc_)
            alloc = alloc_;
        else
            alloc = global_allocator();
    }

    ~vector()
    {
        if (buf.data)
        {
            ns_free(alloc, buf.data);
        }
    }

    buffer<T> buf{0, 0, nullptr};
    mem_store *alloc{};

    inline iterator begin()
    {
        return buf.data;
    }

    inline const_iterator begin() const
    {
        return buf.data;
    }

    inline iterator end()
    {
        return (buf.data + buf.size);
    }

    inline const_iterator end() const
    {
        return (buf.data + buf.size);
    }

    inline sizet size() const
    {
        return buf.size;
    }

    inline bool empty() const
    {
        return (buf->size == 0);
    }

    inline sizet capacity() const
    {
        return buf.cap;
    }

    inline const T &operator[](sizet ind) const
    {
        return buf.data[ind];
    }
    inline T &operator[](sizet ind)
    {
        return buf.data[ind];
    }
};

template<class T>
void reallocate_and_copy(vector<T> *vec, sizet new_cap)
{
    // New cap can't be any smaller than mem_nod since we are using free list allocator
    while (new_cap * sizeof(T) < sizeof(mem_node))
        ++new_cap;

    auto old_ptr = vec->buf.data;

    vec->buf.cap = new_cap;
    vec->buf.data = (T *)ns_alloc(vec->alloc, vec->buf.cap * sizeof(T));

    // Shrink the old size if its greater than the new capacity (so we only copy those items)
    if (vec->buf.size > vec->buf.cap)
        vec->buf.size = vec->buf.cap;

    // Copy old items to new vec before updating size and adding item, and deallocating prev mem
    for (int i = 0; i < vec->size(); ++i)
    {
        vec->buf.data[i] = old_ptr[i];
    }

#ifdef DEBUG_BUILD
    for (int i = vec->buf.size; i < vec->buf.cap; ++i)
        vec->buf.data[i] = {};
#endif

    ns_free(vec->alloc, old_ptr);
}

template<class T>
void reserve(vector<T> *vec, sizet capacity)
{
    if (vec->buf.cap > capacity)
        return;
    reallocate_and_copy(vec, capacity);
}

template<class T>
void shrink_to_fit(vector<T> *vec)
{
    nsassert(vec->size() <= vec->capacity());
    if (vec->size() == vec->capacity())
        return;
    reallocate_and_copy(vec, vec->size());
}

template<class T>
T *push_back(vector<T> *vec, const T &item)
{
    nsassert(vec->size() <= vec->capacity());
    if (vec->size() == vec->capacity())
    {
        reallocate_and_copy(vec, vec->buf.cap * 2);
    }

    T *ret = &vec->buf.data[vec->size()];
    *ret = item;
    ++vec->buf.size;
    return ret;
}

template<class T, sizet N>
T *push_back(array<T, N> *arr, const T &item)
{
    if (arr->buf.size == arr->buf.cap)
        return nullptr;
    T *ret = &arr->buf.data[arr->buf.size];
    *ret = item;
    ++arr->buf.size;
    return ret;
}

template<class T, class... Args>
T *emplace_back(vector<T> *vec, Args &&...args)
{
    nsassert(vec->size() <= vec->capacity());
    if (vec->size() == vec->capacity())
    {
        reallocate_and_copy(vec, vec->buf.cap * 2);
    }

    T *ret = &vec->buf.data[vec->size()];
    new (ret) T(std::forward<Args>(args)...);
    ++vec->buf.size;
    return ret;
}

template<class T, sizet N, class... Args>
T *emplace_back(array<T, N> *arr, Args &&...args)
{
    if (arr->buf.size == arr->buf.cap)
        return nullptr;
    T *ret = &arr->buf.data[arr->buf.size];
    new (ret) T(std::forward<Args>(args)...);
    ++arr->buf.size;
    return ret;
}

template<class T>
void clear_to(T *bufobj, const typename T::value_type &item)
{
    for (int i = 0; i < bufobj->buf.cap; ++i)
        bufobj->buf.data[i] = item;
}

template<class T>
void pop_back(T *bufobj)
{
    if (bufobj->buf.size == 0)
        return;
#ifdef DEBUG_BUILD
    bufobj->buf.data[bufobj->buf.size-1] = {};
#endif
    --bufobj->buf.size;
}

template<class T>
void clear(T *bufobj)
{
    bufobj->buf.size = 0;
#ifdef DEBUG_BUILD
    for (int i = 0; i < bufobj->buf.cap; ++i)
        bufobj->buf.data[i] = {};
#endif
}

template<class T>
typename T::value_type *back(T *bufobj)
{
    if (bufobj->buf.size > 0)
        return &bufobj->buf.data[bufobj->buf.size - 1];
    return {};
}

template<class T>
typename T::value_type *front(T *bufobj)
{
    if (bufobj->buf.size > 0)
        return &bufobj->buf.data[0];
    return {};
}

template<class T>
bool remove(T *bufobj, sizet index)
{
    if (index >= bufobj->buf.size)
        return false;

    // Copy the items after the item to delete
    for (int i = index; i < (bufobj->buf.size - 1); ++i)
    {
        bufobj->buf.data[i] = bufobj->buf.data[i + 1];
    }
    pop_back(bufobj);
    return true;
}

template<class T>
typename T::iterator find(T *bufobj, const typename T::value_type &item)
{
    auto iter = bufobj->begin();
    while (iter != bufobj->end())
    {
        if (*iter == item)
            return iter;
        ++iter;
    }
    return bufobj->end();
}


template<class T>
void resize(vector<T> *vec, sizet new_size)
{
    nsassert(vec->size() <= vec->capacity());
    sizet cap = vec->buf.cap;
    if (new_size > cap)
    {
        while (cap < new_size)
            cap *= 2;
        reallocate_and_copy(vec, cap);
    }

    // If in debug - clear the removed data (if any) to 0
    #ifdef DEBUG_BUILD
    for (int i = new_size; i < vec->buf.size; ++i)
        vec->buf.data[i] = {};
    #endif
    vec->buf.size = new_size;
}

template<class T>
void resize(vector<T> *vec, sizet new_size, const T &copy)
{
    nsassert(vec->size() <= vec->capacity());
    sizet cap = vec->buf.cap;
    if (new_size > cap)
    {
        while (cap < new_size)
            cap *= 2;
        reallocate_and_copy(vec, cap);
    }
    for (int i = vec->buf.size; i < new_size; ++i)
        vec->buf.data[i] = copy;

    // If in debug - clear the removed data (if any) to 0
    #ifdef DEBUG_BUILD
    for (int i = new_size; i < vec->buf.size; ++i)
        vec->buf.data[i] = {};
    #endif
    vec->buf.size = new_size;
}

template<class T, class... Args>
void resize(vector<T> *vec, sizet new_size, Args &&...args)
{
    nsassert(vec->size() <= vec->capacity());
    sizet cap = vec->buf.cap;
    if (new_size > cap)
    {
        while (cap < new_size)
            cap *= 2;
        reallocate_and_copy(vec, cap);
    }
    for (int i = vec->buf.size; i < new_size; ++i)
    {
        T *item = &vec->buf[i];
        new (item) T(std::forward<Args>(args)...);
    }
    // If in debug - clear the removed data (if any) to 0
    #ifdef DEBUG_BUILD
    for (int i = new_size; i < vec->buf.size; ++i)
        vec->buf.data[i] = {};
    #endif
    vec->buf.size = new_size;
}

template<class T>
typename T::iterator erase(T *bufobj, typename T::iterator iter)
{
    while (iter + 1 != bufobj->end())
    {
        *iter = *(iter + 1);
        ++iter;
    }
    pop_back(bufobj);
    return iter;
}

template<class T>
sizet index_from_ptr(T *bufobj, typename T::value_type *item)
{
    sizet offset = (item - bufobj->buf.data);
    if (offset < bufobj->buf.size)
        return offset;
    return npos;
}

} // namespace noble_steed