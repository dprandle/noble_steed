#include "type_collection_db.h"
#include "../memory/callocator.h"

namespace noble_steed
{
Type_Collection_DB::Type_Collection_DB(Allocator * alloc):_alloc(alloc)
{
    if (!_alloc)
        _alloc.reset(new CAllocator);
}

Type_Collection_DB::~Type_Collection_DB()
{
}

void Type_Collection_DB::clear()
{
    _collections.clear();
}

}