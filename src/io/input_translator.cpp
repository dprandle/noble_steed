#include <GLFW/glfw3.h>

#include "input_translator.h"
#include "logger.h"
#include "input_map.h"

#include "../core/context.h"
#include "../core/application.h"
#include "../core/engine.h"
#include "../core/string_hash.h"
#include "../graphics/window.h"
#include "../scene/world.h"


namespace noble_steed
{
namespace events::input
{
namespace trigger
{
const Str_Hash id = "Trigger";
const Str_Hash state = "state";
const Str_Hash scroll_offsets = "scroll_offsets";
const Str_Hash mouse_delta = "mouse_delta";
const Str_Hash current_mpos = "current_mpos";
} // namespace trigger

} // namespace events

Input_Translator::Input_Translator()
{}

Input_Translator::~Input_Translator()
{}

void Input_Translator::initialize(const Variant_Map & init_params)
{
    System::initialize(init_params);

    subscribe_event_handler(events::window::key_press::id, this, &Input_Translator::handle_key_press);
    subscribe_event_handler(events::window::mouse_press::id, this, &Input_Translator::handle_mouse_press);
    subscribe_event_handler(events::window::scroll::id, this, &Input_Translator::handle_scroll);
    subscribe_event_handler(events::window::cursor_moved::id, this, &Input_Translator::handle_mouse_movement);
    //sig_connect(ns_eng->update, this, &Input_Translator::update);
}

void Input_Translator::handle_key_press(Event & ev)
{
    using namespace events::window;
    Trigger_Condition tc;
    i8 action = ev.data[key_press::action].get_value<i8>();
    tc.input_code = ev.data[key_press::key].get_value<i16>();
    tc.modifier_mask = ev.data[key_press::mods].get_value<i16>();
    dlog("Detected key {}  {}",tc.input_code,tc.modifier_mask);
    send_event_for_trigger_action_(tc, action);
}

void Input_Translator::send_event_for_trigger_action_(const Trigger_Condition & tc, i8 action, const Variant_Map & other_params)
{
    // Go through the context stack starting with the top.. send out the event for the first
    // matching one found and return
    using namespace events::input;
    for (int i = context_stack_.size() - 1; i >= 0; --i)
    {
        Input_Context * cur_ctxt = context_stack_[i];
        auto trigger_range = cur_ctxt->get_triggers(tc);
        auto range_iter = trigger_range.first;
        bool found_match = false;
        while (range_iter != trigger_range.second)
        {
            if (check_bitmask_all(range_iter->second.trigger_state, action))
            {
                Event ev;
                ev.id = range_iter->second.name_hash;
                ev.data[trigger::state] = action;
                ev.data[trigger::current_mpos] = cur_mpos_;
                ev.data.insert(other_params.begin(), other_params.end());
                post_event(ev);
                found_match = true;
            }
            ++range_iter;
        }
        if (found_match)
            return;
    }
}

void Input_Translator::handle_mouse_press(Event & ev)
{
    using namespace events::window;
    Trigger_Condition tc;
    i8 action = ev.data[mouse_press::action].get_value<i8>();
    tc.input_code = ev.data[mouse_press::button].get_value<i16>();
    tc.modifier_mask = ev.data[mouse_press::mods].get_value<i16>();
    dlog("in code: {}  mod mask: {}",tc.input_code, tc.modifier_mask);
    send_event_for_trigger_action_(tc, action);
}

void Input_Translator::handle_scroll(Event & ev)
{
    using namespace events::window;
    using namespace events::input;
    Trigger_Condition tc;
    i8 action = T_MOUSE_MOVE_OR_SCROLL;
    tc.input_code = imap::mouse::SCROLL;
    tc.modifier_mask = ev.data[scroll::mods].get_value<i16>();
    Variant_Map params;
    dtup2 nscroll;
    nscroll.x = ev.data[scroll::x_offset].get_value<double>();
    nscroll.y = ev.data[scroll::y_offset].get_value<double>();
    params[trigger::scroll_offsets] = nscroll;
    send_event_for_trigger_action_(tc, action, params);
}

void Input_Translator::handle_focus_change(Event & ev)
{
    using namespace events;
    i8 foc = ev.data[window::focus_change::focused].get_value<i8>();
    dtup2 new_mpos = ev.data[window::focus_change::new_pos].get_value<dtup2>();
    if (foc)
    {
        cur_mpos_ = new_mpos;
        prev_mpos_ = cur_mpos_;
    }
}

void Input_Translator::handle_mouse_movement(Event & ev)
{
    using namespace events::window;
    using namespace events::input;

    Trigger_Condition tc;
    i8 action = T_MOUSE_MOVE_OR_SCROLL;
    tc.input_code = imap::mouse::MOVEMENT;
    tc.modifier_mask = ev.data[cursor_moved::mods].get_value<i16>();
    prev_mpos_ = cur_mpos_;
    cur_mpos_ = ev.data[cursor_moved::new_pos].get_value<dtup2>();
    dtup2 mdelta = cur_mpos_ - prev_mpos_;
    Variant_Map params;
    params[trigger::mouse_delta] = mdelta;
    send_event_for_trigger_action_(tc, action, params);
}

void Input_Translator::terminate()
{
    System::terminate();
}

void Input_Translator::update()
{
    process_events();
}

bool Input_Translator::_trigger_already_active(Input_Action_Trigger * trig)
{
    return false;
}

void Input_Translator::push_context(Input_Context * ctxt)
{
    context_stack_.push_back(ctxt);
}

void Input_Translator::pop_context()
{
    context_stack_.pop_back();
}

} // namespace noble_steed
