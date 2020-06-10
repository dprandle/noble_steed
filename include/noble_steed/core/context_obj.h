#pragma once

#include <rttr/type>
#include <rttr/registration_friend>
#include <noble_steed/io/json_archive.h>
#include <noble_steed/core/signal.h>
#include <noble_steed/container/array.h>
#include <mutex>

namespace noble_steed
{

struct Event
{
    Event();
    Event(const String & name, const Variant_Map & data_=Variant_Map());
    u32 id;
    Variant_Map data;
};

struct Event_Buffer
{
    Event_Buffer();
    void push(const Event & event);
    Event & process();
    bool available();

    Array<Event,20> frame_events;
    size_t cur_ind;
    size_t processed_ind;
};

#define subscribe_event_handler(event_id, ...) ns_ctxt.subscribe_to_event(this,event_id); \
    sig_connect(process_event, __VA_ARGS__);

#define unsubscribe_event_handler(event_id, ...) ns_ctxt.unsubscribe_to_event(this,event_id); \
    sig_disconnect(process_event, __VA_ARGS__);

class Context_Obj
{    
    template<class T> friend class Pool_Factory;
    template<class T> friend class Free_List_Factory;

  public:
    Context_Obj();

    Context_Obj(const Context_Obj & copy);

    virtual ~Context_Obj();

    virtual void pack_unpack(JSON_Archive & ar);

    String to_json();

    void from_json(const String & json_str);

    bool is_owned_by_context();

    Context_Obj & operator=(Context_Obj rhs);

    void push_event(const Event & event);

    void process_events();

    Signal<Event&> process_event;
    Signal<Context_Obj*> destroyed;
    
  protected:
    void swap(Context_Obj & rhs);

    virtual void pack_begin(JSON_Archive::Direction io_dir);

    virtual void pack_end(JSON_Archive::Direction io_dir);

    Event_Buffer events_;
    std::mutex event_mutex_;

    bool owned;

    SLOT_OBJECT
    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};
} // namespace noble_steed