#include <noble_steed/core/application.h>

int main()
{
    noble_steed::Application applic;
    applic.initialize();
    applic.exec();
    return 0;
}
