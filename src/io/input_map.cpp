#include <GLFW/glfw3.h>

#include "input_map.h"

#include "../core/variant.h"
#include "../core/context.h"
#include "../core/string_hash.h"

namespace noble_steed
{
Input_Action_Trigger::Input_Action_Trigger(const Str_Hash &name, const Trigger_Condition &tcond, Trigger_State tstate)
    : name_hash(name), condition(tcond), trigger_state(tstate)
{
    if (tcond.input_code <= GLFW_MOUSE_BUTTON_LAST)
        trigger_state = T_MOUSE_MOVE_OR_SCROLL;
}

bool Input_Action_Trigger::operator==(const Input_Action_Trigger &rhs) const
{
    return (name_hash == rhs.name_hash && condition == rhs.condition && trigger_state == rhs.trigger_state);
}

bool Input_Context::add_trigger(const Input_Action_Trigger &trigger)
{
    // Make sure we have no absolute duplicates (not allowed due to stacking contexts)
    Trigger_Range tr = get_triggers(trigger.condition);
    while (tr.first != tr.second)
    {
        if (tr.first->second == trigger)
            return false;
        ++tr.first;
    }

    t_map_.emplace(trigger.condition.lookup_key, trigger);
    return true;
}

bool Input_Context::remove_trigger(const Input_Action_Trigger &trigger)
{
    auto iter = t_map_.begin();
    while (iter != t_map_.end())
    {
        if (iter->second == trigger)
        {
            t_map_.erase(iter);
            return true;
        }
        ++iter;
    }
    return false;
}

u32 Input_Context::remove_triggers(const Trigger_Condition &cond)
{
    u32 ret = 0;
    Trigger_Range tr = get_triggers(cond);
    while (tr.first != tr.second)
    {
        tr.first = t_map_.erase(tr.first);
        ++ret;
    }
    return ret;
}

Trigger_Range Input_Context::get_triggers(const Trigger_Condition &cond)
{
    return t_map_.equal_range(cond.lookup_key);
}

Input_Map::Input_Map()
{}

Input_Map::Input_Map(const Input_Map &copy_)
{}

Input_Map::~Input_Map()
{}

Input_Map &Input_Map::operator=(Input_Map rhs)
{
    return *this;
}

Input_Context *Input_Map::add_context(const Str_Hash &name, const Input_Context &to_add)
{
    auto ret = contexts_.emplace(name, to_add);
    if (ret.second)
        return &ret.first->second;
    return nullptr;
}

Input_Context *Input_Map::get_context(const Str_Hash &name)
{
    auto iter = contexts_.find(name);
    if (iter != contexts_.end())
        return &iter->second;
    return nullptr;
}

bool Input_Map::remove_context(const Str_Hash &name)
{
    auto count = contexts_.erase(name);
    return count > 0;
}

bool Input_Map::rename_context(const Str_Hash &old_name, const Str_Hash &new_name)
{
    Input_Context *old_ic = get_context(old_name);
    if (!old_ic)
        return false;

    Input_Context cpy = *old_ic;
    remove_context(old_name);
    return add_context(new_name, cpy);
}

} // namespace noble_steed
