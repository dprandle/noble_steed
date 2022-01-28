#pragma once

#include "../core/system.h"

namespace noble_steed
{
struct Input_Context;
struct Input_Action_Trigger;
union Trigger_Condition;

namespace events::input
{

namespace trigger
{
/// Trigger event is special in that it's id is the name_hash determined at runtime - it doesn't have a set id
extern const Str_Hash id;
/// i8 - state is either Trigget_State::T_PRESS or Trigger_State::T_RELEASE - wont be here in mouse movement events or scroll ones
extern const Str_Hash state;
/// dtup2 - Scroll offsets when the trigger is triggered from a scroll event
extern const Str_Hash scroll_offsets;
/// dtup2 - Mouse delta from this frame to previous frame - will be 0 if just received focus - when trigger generated from mouse move event
extern const Str_Hash mouse_delta;
/// dtup2 - Mouse new position as of this frame in screen coordinates
extern const Str_Hash current_mpos;
} // namespace trigger

} // namespace events

// Keep track of current modifier state etc?
class Input_Translator : public System
{
  public:
    Input_Translator();
    ~Input_Translator();

    void initialize(const Variant_Map & init_params);

    void terminate();

    void push_context(Input_Context * ctxt);

    void pop_context();

    void update();

  private:
    void send_event_for_trigger_action_(const Trigger_Condition & tc, i8 action, const Variant_Map & other_params = Variant_Map());

    bool _trigger_already_active(Input_Action_Trigger * trig);

    void handle_key_press(Event & ev);

    void handle_mouse_press(Event & ev);

    void handle_scroll(Event & ev);

    void handle_mouse_movement(Event & ev);

    void handle_focus_change(Event & ev);

    Vector<Input_Context *> context_stack_;
    Vector<Input_Action_Trigger *> active_triggers_;

    dtup2 cur_mpos_;
    dtup2 prev_mpos_;
};
} // namespace noble_steed