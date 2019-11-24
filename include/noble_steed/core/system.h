#pragma once

#include <noble_steed/container/string.h>
#include <rttr/type>

namespace noble_steed
{
class System
{
    RTTR_ENABLE()

  public:
    System();
    virtual ~System();

    const String & get_internal();

    int get_num();

    void set_num(int num);

    void log_internal();

    void set_internal(const String & to_set);

  private:
    String internal_;
    int internal_num_;
};

} // namespace noble_steed