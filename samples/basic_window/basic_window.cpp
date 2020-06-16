#include <noble_steed/core/application.h>
#include <noble_steed/graphics/window.h>
#include <noble_steed/io/input_map.h>
#include <noble_steed/io/input_translator.h>
#include <noble_steed/core/resource_cache.h>
#include <noble_steed/core/context.h>
#include <noble_steed/scene/world.h>

int main()
{
    using namespace noble_steed;

    Application applic;
    Variant_Map init_params;
    
    //init_params[Init_Params::Window::ALWAS_ON_TOP] = true;
    init_params[Init_Params::Window::TITLE] = "Scoobers";

    applic.initialize(init_params);
    auto rc = ns_ctxt.get_resource_cache();

    Input_Context ic;
    Input_Action_Trigger iac;
    iac.name_hash = str_hash("Scooby");
    iac.condition.input_code = KEY_G;
    iac.condition.modifier_mask = MOD_ALT | MOD_CAPS_LOCK;
    iac.trigger_state = Trigger_State::T_PRESS;
    ic.add_trigger(iac);

    auto isys = ns_world->get_system<Input_Translator>();
    isys->push_context(&ic);

    applic.exec();
    return 0;
}
