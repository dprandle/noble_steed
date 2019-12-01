#pragma once

#include <noble_steed/container/vector.h>
#include <noble_steed/core/common.h>

namespace noble_steed
{

class Router;

struct Slot_Base
{
    virtual ~Slot_Base()
    {}
    Router * router;
};

template<class... Args>
struct Signal;

template<class... Args>
struct Slot : public Slot_Base
{
    ~Slot();

    virtual void call(Args...) = 0;
    Signal<Args...> * connected_signal;
};

template<class T, class... Args>
struct Slot_Concrete : public Slot<Args...>
{
    typedef void (T::*mem_func_t)(Args...);

    Slot_Concrete(T * inst_, mem_func_t func_) : inst(inst_), func(func_)
    {}

    void call(Args... args)
    {
        (inst->*func)(args...);
    }

    T * inst;
    mem_func_t func;
};

template<class... Args>
struct Signal
{
    ~Signal()
    {
        while (con_slots.begin() != con_slots.end())
            assist_delete(con_slots.back());
    }

    void operator()(Args... args)
    {
        // By using a temp copy instead of the originals, slot functions can disconnect from
        // signals - ie modify con_slots without causing crashes
        Vector<Slot<Args...> *> tmp_copy = con_slots;
        auto iter = tmp_copy.begin();
        while (iter != tmp_copy.end())
        {
            (*iter)->call(args...);
            ++iter;
        }
    }

    Vector<Slot<Args...> *> con_slots;
};

template<class... Args>
Slot<Args...>::~Slot()
{
    auto iter = connected_signal->con_slots.begin();
    while (iter != connected_signal->con_slots.end())
    {
        if (this == *iter)
            iter = connected_signal->con_slots.erase(iter);
        else
            ++iter;
    }
}

#define SLOT_OBJECT Router router_;
#define sig_connect(signal, slot) router_.connect(this, &slot, signal)
#define sig_disconnect(signal) router_.disconnect(signal)

void assist_delete(Slot_Base * del);

class Router
{
  public:
    Router();
    ~Router();

    void disconnect_all();

    template<class T, class... Args>
    void connect(T * inst, void (T::*mf)(Args...), Signal<Args...> & sig)
    {
        Slot_Concrete<T, Args...> * slot_ptr = new Slot_Concrete<T, Args...>(inst, mf);
        slot_ptr->connected_signal = &sig;
        slot_ptr->router = this;
        sig.con_slots.push_back(slot_ptr);
        con_slots.push_back(slot_ptr);
    }

    template<class... Args>
    void disconnect(Signal<Args...> & sig)
    {
        auto iter = con_slots.begin();
        while (iter != con_slots.end())
        {
            Slot<Args...> * cast_down = dynamic_cast<Slot<Args...> *>(*iter);
            if (cast_down != nullptr && cast_down->connected_signal == &sig)
            {
                delete *iter;
                iter = con_slots.erase(iter);
            }
            else
                ++iter;
        }
    }

    void remove_slot(Slot_Base * slot);

    sizet signals_connected_count();

  private:
    Vector<Slot_Base*> con_slots;
};
} // namespace noble_steed