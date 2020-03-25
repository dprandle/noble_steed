#include <noble_steed/core/signal.h>

namespace noble_steed
{
Router::Router(): con_slots()
{}

// This will prevent copying!
Router::Router(const Router & copy): con_slots()
{}

Router::~Router()
{
    disconnect_all();
}

void Router::disconnect_all()
{
    while (con_slots.begin() != con_slots.end())
    {
        delete con_slots.back();
        con_slots.pop_back();
    }
}

void Router::remove_slot(Slot_Base * slot)
{
    auto iter = con_slots.begin();
    while (iter != con_slots.end())
    {
        if (*iter == slot)
        {
            delete *iter;
            iter = con_slots.erase(iter);
        }
        else
            ++iter;
    }
}

void assist_delete(Slot_Base * del)
{
    del->router->remove_slot(del);
}

sizet Router::signals_connected_count()
{
    return con_slots.size();
}

} // namespace noble_steed