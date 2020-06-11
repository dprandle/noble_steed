#include <noble_steed/core/common.h>
#include <rttr/registration.h>
#include <noble_steed/io/json_archive.h>

std::ostream & operator<<(std::ostream & lhs, const rttr::variant & rhs)
{
    noble_steed::JSON_Archive ja;
    ja.io_dir = noble_steed::Archive::DIR_OUT;
    pack_unpack(ja, rhs);
    return lhs << ja.json_str;
}


RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    // Tuple2<T>
    registration::class_<dtup2>("noble_steed::dtup2")
        .property("first", &dtup2::first, registration::public_access)
        .property("second", &dtup2::second, registration::public_access);

    registration::class_<ftup2>("noble_steed::ftup2")
        .property("first", &ftup2::first, registration::public_access)
        .property("second", &ftup2::second, registration::public_access);

    registration::class_<i8tup2>("noble_steed::i8tup2")
        .property("first", &i8tup2::first, registration::public_access)
        .property("second", &i8tup2::second, registration::public_access);

    registration::class_<i16tup2>("noble_steed::i16tup2")
        .property("first", &i16tup2::first, registration::public_access)
        .property("second", &i16tup2::second, registration::public_access);

    registration::class_<itup2>("noble_steed::itup2")
        .property("first", &itup2::first, registration::public_access)
        .property("second", &itup2::second, registration::public_access);

    registration::class_<i64tup2>("noble_steed::i64tup2")
        .property("first", &i64tup2::first, registration::public_access)
        .property("second", &i64tup2::second, registration::public_access);

    registration::class_<u8tup2>("noble_steed::u8tup2")
        .property("first", &u8tup2::first, registration::public_access)
        .property("second", &u8tup2::second, registration::public_access);

    registration::class_<u16tup2>("noble_steed::u16tup2")
        .property("first", &u16tup2::first, registration::public_access)
        .property("second", &u16tup2::second, registration::public_access);

    registration::class_<utup2>("noble_steed::utup2")
        .property("first", &utup2::first, registration::public_access)
        .property("second", &utup2::second, registration::public_access);

    registration::class_<u64tup2>("noble_steed::u64tup2")
        .property("first", &u64tup2::first, registration::public_access)
        .property("second", &u64tup2::second, registration::public_access);

    registration::class_<str_tup2>("noble_steed::str_tup2")
        .property("first", &str_tup2::first, registration::public_access)
        .property("second", &str_tup2::second, registration::public_access);

    registration::class_<xtup2>("noble_steed::xtup2")
        .property("first", &xtup2::first, registration::public_access)
        .property("second", &xtup2::second, registration::public_access);

    // Tuple3<T>
    registration::class_<dtup3>("noble_steed::dtup3")
        .property("first", &dtup3::first, registration::public_access)
        .property("second", &dtup3::second, registration::public_access)
        .property("third", &dtup3::third, registration::public_access);

    registration::class_<ftup3>("noble_steed::ftup3")
        .property("first", &ftup3::first, registration::public_access)
        .property("second", &ftup3::second, registration::public_access)
        .property("third", &ftup3::third, registration::public_access);

    registration::class_<i8tup3>("noble_steed::i8tup3")
        .property("first", &i8tup3::first, registration::public_access)
        .property("second", &i8tup3::second, registration::public_access)
        .property("third", &i8tup3::third, registration::public_access);

    registration::class_<i16tup3>("noble_steed::i16tup3")
        .property("first", &i16tup3::first, registration::public_access)
        .property("second", &i16tup3::second, registration::public_access)
        .property("third", &i16tup3::third, registration::public_access);

    registration::class_<itup3>("noble_steed::itup3")
        .property("first", &itup3::first, registration::public_access)
        .property("second", &itup3::second, registration::public_access)
        .property("third", &itup3::third, registration::public_access);

    registration::class_<i64tup3>("noble_steed::i64tup3")
        .property("first", &i64tup3::first, registration::public_access)
        .property("second", &i64tup3::second, registration::public_access)
        .property("third", &i64tup3::third, registration::public_access);

    registration::class_<u8tup3>("noble_steed::u8tup3")
        .property("first", &u8tup3::first, registration::public_access)
        .property("second", &u8tup3::second, registration::public_access)
        .property("third", &u8tup3::third, registration::public_access);

    registration::class_<u16tup3>("noble_steed::u16tup3")
        .property("first", &u16tup3::first, registration::public_access)
        .property("second", &u16tup3::second, registration::public_access)
        .property("third", &u16tup3::third, registration::public_access);

    registration::class_<utup3>("noble_steed::utup3")
        .property("first", &utup3::first, registration::public_access)
        .property("second", &utup3::second, registration::public_access)
        .property("third", &utup3::third, registration::public_access);

    registration::class_<u64tup3>("noble_steed::u64tup3")
        .property("first", &u64tup3::first, registration::public_access)
        .property("second", &u64tup3::second, registration::public_access)
        .property("third", &u64tup3::third, registration::public_access);

    registration::class_<str_tup3>("noble_steed::str_tup3")
        .property("first", &str_tup3::first, registration::public_access)
        .property("second", &str_tup3::second, registration::public_access)
        .property("third", &str_tup3::third, registration::public_access);

    registration::class_<xtup3>("noble_steed::xtup3")
        .property("first", &xtup3::first, registration::public_access)
        .property("second", &xtup3::second, registration::public_access)
        .property("third", &xtup3::third, registration::public_access);

    // Tuple4<T>
    registration::class_<dtup4>("noble_steed::dtup4")
        .property("first", &dtup4::first, registration::public_access)
        .property("second", &dtup4::second, registration::public_access)
        .property("third", &dtup4::third, registration::public_access)
        .property("fourth", &dtup4::fourth, registration::public_access);

    registration::class_<ftup4>("noble_steed::ftup4")
        .property("first", &ftup4::first, registration::public_access)
        .property("second", &ftup4::second, registration::public_access)
        .property("third", &ftup4::third, registration::public_access)
        .property("fourth", &ftup4::fourth, registration::public_access);

    registration::class_<i8tup4>("noble_steed::i8tup4")
        .property("first", &i8tup4::first, registration::public_access)
        .property("second", &i8tup4::second, registration::public_access)
        .property("third", &i8tup4::third, registration::public_access)
        .property("fourth", &i8tup4::fourth, registration::public_access);

    registration::class_<i16tup4>("noble_steed::i16tup4")
        .property("first", &i16tup4::first, registration::public_access)
        .property("second", &i16tup4::second, registration::public_access)
        .property("third", &i16tup4::third, registration::public_access)
        .property("fourth", &i16tup4::fourth, registration::public_access);

    registration::class_<itup4>("noble_steed::itup4")
        .property("first", &itup4::first, registration::public_access)
        .property("second", &itup4::second, registration::public_access)
        .property("third", &itup4::third, registration::public_access)
        .property("fourth", &itup4::fourth, registration::public_access);

    registration::class_<i64tup4>("noble_steed::i64tup4")
        .property("first", &i64tup4::first, registration::public_access)
        .property("second", &i64tup4::second, registration::public_access)
        .property("third", &i64tup4::third, registration::public_access)
        .property("fourth", &i64tup4::fourth, registration::public_access);

    registration::class_<u8tup4>("noble_steed::u8tup4")
        .property("first", &u8tup4::first, registration::public_access)
        .property("second", &u8tup4::second, registration::public_access)
        .property("third", &u8tup4::third, registration::public_access)
        .property("fourth", &u8tup4::fourth, registration::public_access);

    registration::class_<u16tup4>("noble_steed::u16tup4")
        .property("first", &u16tup4::first, registration::public_access)
        .property("second", &u16tup4::second, registration::public_access)
        .property("third", &u16tup4::third, registration::public_access)
        .property("fourth", &u16tup4::fourth, registration::public_access);

    registration::class_<utup4>("noble_steed::utup4")
        .property("first", &utup4::first, registration::public_access)
        .property("second", &utup4::second, registration::public_access)
        .property("third", &utup4::third, registration::public_access)
        .property("fourth", &utup4::fourth, registration::public_access);

    registration::class_<u64tup4>("noble_steed::u64tup4")
        .property("first", &u64tup4::first, registration::public_access)
        .property("second", &u64tup4::second, registration::public_access)
        .property("third", &u64tup4::third, registration::public_access)
        .property("fourth", &u64tup4::fourth, registration::public_access);

    registration::class_<str_tup4>("noble_steed::str_tup4")
        .property("first", &str_tup4::first, registration::public_access)
        .property("second", &str_tup4::second, registration::public_access)
        .property("third", &str_tup4::third, registration::public_access)
        .property("fourth", &str_tup4::fourth, registration::public_access);

    registration::class_<xtup4>("noble_steed::xtup4")
        .property("first", &xtup4::first, registration::public_access)
        .property("second", &xtup4::second, registration::public_access)
        .property("third", &xtup4::third, registration::public_access)
        .property("fourth", &xtup4::fourth, registration::public_access);

}