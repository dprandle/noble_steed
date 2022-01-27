#include <bgfx/bgfx.h>

#include "core/application.h"
#include "graphics/window.h"
#include "io/input_map.h"
#include "io/input_translator.h"
#include "core/resource_cache.h"
#include "core/context.h"
#include "scene/world.h"
#include "graphics/renderer.h"
#include "graphics/mesh.h"

#include "scene/world_chunk.h"
#include "scene/world.h"
#include "scene/entity.h"

int main()
{
    // using namespace noble_steed;

    // Application applic;
    // Variant_Map init_params;
    // ilog("STARTING BASIC WINDOW");

    // //init_params[Init_Params::Window::ALWAS_ON_TOP] = true;
    // init_params[init_param_key::window::TITLE] = String("Basic Window");
    // init_params[init_param_key::renderer::SHADER_PROFILE] = String("spirv");

    // applic.initialize(init_params);
    // auto rc = ns_ctxt.get_resource_cache();

    // auto allr = rc->get_all();
    // for (int i = 0; i < allr.size(); ++i)
    // {
    //     dlog("Res name: {}  Res id: {}  Res package name: {}",allr[i]->get_name(),allr[i]->get_id(),allr[i]->get_package());
    // }

    // Input_Context ic;
    // Input_Action_Trigger iac("load_and_compile_shader");
    // iac.condition.input_code = MOUSE_BUTTON_LEFT;
    // iac.condition.modifier_mask = MOD_CONTROL;
    // iac.trigger_state = Trigger_State::T_PRESS;
    // ic.add_trigger(iac);
    // auto imap = rc->add<Input_Map>("editor");
    // if (!imap)
    //     imap = rc->get<Input_Map>("editor");

    // if (imap)
    // {
    //     auto icptr = imap->add_context("global", ic);
    //     if (!icptr)
    //         icptr = imap->get_context("global");

    //     if (icptr)
    //     {
    //         auto isys = ns_world->get_system<Input_Translator>();
    //         isys->push_context(icptr);
    //     }

    //     imap->save();
    // }

    // World_Chunk * wc = ns_ctxt.get_resource_cache()->add<World_Chunk>("maps/basic");
    // Entity * ent = ns_world->create();
    // ent->set_name("Test");
    // wc->add(ent);

    // Entity * ent2 = wc->add();

    //wc->save();

    //applic.exec();
    return 0;
}
