#ifndef TARIGO_MSGPACK_UTIL_HPP
#define TARIGO_MSGPACK_UTIL_HPP

#include <string>

#include "msgpack/type.hpp"
#include "msgpack/format.hpp"

namespace std {

    inline string to_string(tarigo::msgpack::type type) {
        switch (type) {
        case tarigo::msgpack::type::number:
            return "number";
        case tarigo::msgpack::type::null:
            return "null";
        case tarigo::msgpack::type::boolean:
            return "boolean";
        case tarigo::msgpack::type::string:
            return "string";
        case tarigo::msgpack::type::binary:
            return "binary";
        case tarigo::msgpack::type::array:
            return "array";
        case tarigo::msgpack::type::map:
            return "map";
        case tarigo::msgpack::type::extension:
            return "extension";
        }
    }

    inline string to_string(tarigo::msgpack::format fmt) {
        switch (fmt) {
        case tarigo::msgpack::format::positive_fixint:
            return "POSITIVE_FIXINT";
        case tarigo::msgpack::format::fixmap:
            return "FIXMAP";
        case tarigo::msgpack::format::fixarray:
            return "FIXARRAY";
        case tarigo::msgpack::format::fixstr:
            return "FIXSTR";
        case tarigo::msgpack::format::null:
            return "NIL";
        case tarigo::msgpack::format::boolean_false:
            return "FALSE";
        case tarigo::msgpack::format::boolean_true:
            return "TRUE";
        case tarigo::msgpack::format::bin8:
            return "BIN8";
        case tarigo::msgpack::format::bin16:
            return "BIN16";
        case tarigo::msgpack::format::bin32:
            return "BIN32";
        case tarigo::msgpack::format::ext8:
            return "EXT8";
        case tarigo::msgpack::format::ext16:
            return "EXT16";
        case tarigo::msgpack::format::ext32:
            return "EXT32";
        case tarigo::msgpack::format::float32:
            return "FLOAT32";
        case tarigo::msgpack::format::float64:
            return "FLOAT64";
        case tarigo::msgpack::format::uint8:
            return "UINT8";
        case tarigo::msgpack::format::uint16:
            return "UINT16";
        case tarigo::msgpack::format::uint32:
            return "UINT32";
        case tarigo::msgpack::format::uint64:
            return "UINT64";
        case tarigo::msgpack::format::int8:
            return "INT8";
        case tarigo::msgpack::format::int16:
            return "INT16";
        case tarigo::msgpack::format::int32:
            return "INT32";
        case tarigo::msgpack::format::int64:
            return "INT64";
        case tarigo::msgpack::format::fixext1:
            return "FIXEXT1";
        case tarigo::msgpack::format::fixext2:
            return "FIXEXT2";
        case tarigo::msgpack::format::fixext4:
            return "FIXEXT4";
        case tarigo::msgpack::format::fixext8:
            return "FIXEXT8";
        case tarigo::msgpack::format::fixext16:
            return "FIXEXT16";
        case tarigo::msgpack::format::str8:
            return "STR8";
        case tarigo::msgpack::format::str16:
            return "STR16";
        case tarigo::msgpack::format::str32:
            return "STR32";
        case tarigo::msgpack::format::array16:
            return "ARRAY16";
        case tarigo::msgpack::format::array32:
            return "ARRAY32";
        case tarigo::msgpack::format::map16:
            return "MAP16";
        case tarigo::msgpack::format::map32:
            return "MAP32";
        case tarigo::msgpack::format::negative_fixint:
            return "NEGATIVE_FIXINT";
        }
    }
}

#endif //TARIGO_MSGPACK_UTIL_HPP
