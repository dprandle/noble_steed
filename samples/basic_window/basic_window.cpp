#include <noble_steed/core/application.h>
#include <noble_steed/graphics/window.h>

int main()
{
    using namespace noble_steed;

    Application applic;
    Variant_Map init_params;
    
    //init_params[Init_Params::Window::ALWAS_ON_TOP] = true;
    init_params[Init_Params::Window::TITLE] = "Scoobers";
    init_params[Init_Params::Window::FULLSCREEN] = true;

    applic.initialize(init_params);
    applic.exec();
    return 0;
}
