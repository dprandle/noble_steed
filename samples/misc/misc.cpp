#include "core/type_info.h"
#include "core/type_collection_db.h"
#include "io/logger.h"
#include "core/handle.h"

using namespace noble_steed;

struct Simple
{
    int x;
    int y;
};

struct Scooby
{
    int x;
    int y;
    double yy;
};

struct Dooby
{
    int x;
    int y;
    String ble;
};

struct Fala
{
    int x;
    int y;
    Variant c;
};

DECLARE_CUSTOM_TYPE(Simple)
DECLARE_CUSTOM_TYPE(Scooby)
DECLARE_CUSTOM_TYPE(Dooby)
DECLARE_CUSTOM_TYPE(Fala)

template<class T>
void print_collection_info(Type_Collection<T> * col)
{
    ilog("Size: {}  Capacity: {}  Type of each: {}  Size of each: {}  Type hash code: {}",
         col->data.size(),
         col->data.capacity(),
         col->type.name(),
         col->type.size(),
         col->type.hash_code());
}

template<class T>
auto create_and_resize(Type_Collection_DB * db, int cap, int count)
{
    auto cdbit = db->create<T>(cap);
    cdbit->data.resize(count);
    return cdbit;
}

int main()
{
    Type_Collection_DB cdb;
    auto cdb_type = create_and_resize<Simple>(&cdb, 23, 13);
    auto cdb_type2 = create_and_resize<Scooby>(&cdb, 23, 13);
    auto cdb_type3 = create_and_resize<Dooby>(&cdb, 23, 13);
    auto cdb_type4 = create_and_resize<Fala>(&cdb, 23, 13);

    print_collection_info(cdb_type);
    print_collection_info(cdb_type2);
    print_collection_info(cdb_type3);
    print_collection_info(cdb_type4);
    cdb.clear();
}