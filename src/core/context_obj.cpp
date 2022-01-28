#include "context_obj.h"
#include "context.h"
#include "../io/logger.h"
#include "string_hash.h"

namespace noble_steed
{
Event::Event() : id(), data()
{}

Event::Event(const Str_Hash &id_, const Variant_Map & data_): id(id_), data(data_)
{}

void Event_Buffer::push(const Event & event)
{
    frame_events[cur_ind] = event;
    ++cur_ind;
    if (cur_ind == frame_events.size())
        cur_ind = 0;
}

bool Event_Buffer::available()
{
    return (processed_ind != cur_ind);
}

Event & Event_Buffer::process()
{
    Event & ev = frame_events[processed_ind];
    ++processed_ind;
    if (processed_ind == frame_events.size())
        processed_ind = 0;
    return ev;
}

Event_Buffer::Event_Buffer() : frame_events(), cur_ind(0), processed_ind(0)
{}

Context_Obj::Context_Obj() : events_(), event_mutex_()
{}

Context_Obj::Context_Obj(const Context_Obj & copy) : events_(), event_mutex_()
{}

Context_Obj & Context_Obj::operator=(Context_Obj rhs)
{
    swap(rhs);
    return *this;
}

void Context_Obj::swap(Context_Obj & rhs)
{}

Context_Obj::~Context_Obj()
{
    emit_sig destroyed(this);
}

void Context_Obj::process_events()
{
    event_mutex_.lock();
    while (events_.available())
    {
        Event & e = events_.process();
        auto fiter = process_event_map.find(e.id);
        if (fiter != process_event_map.end())
        {
            emit_sig fiter->second(e);
        }
    }
    event_mutex_.unlock();
}

void Context_Obj::push_event(const Event & event)
{
    event_mutex_.lock();
    events_.push(event);
    event_mutex_.unlock();
}
} // namespace noble_steed