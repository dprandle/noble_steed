#include "world.h"
#include "entity.h"
#include "component.h"
#include "../core/variant.h"
#include "../core/context.h"
#include "../core/engine.h"
#include "../io/logger.h"
#include "../io/input_translator.h"
#include "../graphics/renderer.h"

namespace noble_steed::scene
{

World::World() : ent_ptrs_(), ent_id_stack_(), ent_current_id_(0)
{}

World::~World()
{
    clear_entities();
}

void World::initialize(const Variant_Map &init_params)
{}

void World::terminate()
{
    ilog("Terminating world");
    clear_entities();
}

void World::clear_entities()
{}

Entity *World::create(const Variant_Map &init_params)
{
    return nullptr;
}

Entity *World::create(const Entity &copy, const Variant_Map &init_params)
{
    return nullptr;
}

void World::rebuild_available_id_stack_()
{
    u32 largest_id = 0;
    auto iter = entity_ids_.begin();
    while (iter != entity_ids_.end())
    {
        if (iter->first > largest_id)
            largest_id = iter->first;
        ++iter;
    }

    ent_current_id_ = largest_id;
    while (ent_id_stack_.size() > 0)
        ent_id_stack_.pop();

    for (u32 i = 0; i < ent_current_id_; ++i)
    {
        if (!contains(i))
            ent_id_stack_.emplace(i);
    }
}

void World::add_entity_(Entity *to_add, const Variant_Map &init_params)
{}

bool World::contains(u32 id)
{
    return false;
}

Entity *World::get(u32 id)
{
    return nullptr;
}

bool World::destroy(u32 id)
{
    return destroy(get(id));
}

void World::handle_entity_packed_in(Event &ev)
{
    // Entity * ent = ev.data[events::entity::entity_packed_in::entity_ptr].get_value<Entity*>();
    // add_entity_(ent, Variant_Map());
}

bool World::destroy(Entity *ent)
{
    return false;
}
} // namespace noble_steed::scene
