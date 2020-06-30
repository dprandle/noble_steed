#include <noble_steed/core/system.h>

namespace noble_steed
{
class Renderer : public System
{
    RTTR_ENABLE(System)
  public:
    Renderer();

    Renderer(const Renderer & copy);

    Renderer & operator=(Renderer rhs);

    virtual ~Renderer();

    void render_frame();

    void initialize(const Variant_Map & init_params);

    void terminate();

  protected:
    void swap(const Renderer & rhs);

};



}