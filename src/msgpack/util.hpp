#ifndef TARIGO_MSGPACK_UTIL_HPP
#define TARIGO_MSGPACK_UTIL_HPP

#include <string>

#include "msgpack/type.hpp"
#include "msgpack/format.hpp"

namespace std {

    inline string to_string(tarigo::msgpack::type type) {
        switch (type) {
            case tarigo::msgpack::type::UNDEFINED:
                return "UNDEFINED";
            case tarigo::msgpack::type::INTEGER:
                return "INTEGER";
            case tarigo::msgpack::type::NIL:
                return "NIL";
            case tarigo::msgpack::type::BOOLEAN:
                return "BOOLEAN";
            case tarigo::msgpack::type::FLOAT:
                return "FLOAT";
            case tarigo::msgpack::type::RAW_STRING:
                return "RAW_STRING";
            case tarigo::msgpack::type::RAW_BINARY:
                return "RAW_BINARY";
            case tarigo::msgpack::type::ARRAY:
                return "ARRAY";
            case tarigo::msgpack::type::MAP:
                return "MAP";
            case tarigo::msgpack::type::EXTENSION:
                return "EXTENSION";
        }
    }

    inline string to_string(tarigo::msgpack::format fmt) {
        switch (fmt) {
            case tarigo::msgpack::format::UNDEFINED:
                return "UNDEFINED";
            case tarigo::msgpack::format::POSITIVE_FIXINT:
                return "POSITIVE_FIXINT";
            case tarigo::msgpack::format::FIXMAP:
                return "FIXMAP";
            case tarigo::msgpack::format::FIXARRAY:
                return "FIXARRAY";
            case tarigo::msgpack::format::FIXSTR:
                return "FIXSTR";
            case tarigo::msgpack::format::NIL:
                return "NIL";
            case tarigo::msgpack::format::FALSE:
                return "FALSE";
            case tarigo::msgpack::format::TRUE:
                return "TRUE";
            case tarigo::msgpack::format::BIN8:
                return "BIN8";
            case tarigo::msgpack::format::BIN16:
                return "BIN16";
            case tarigo::msgpack::format::BIN32:
                return "BIN32";
            case tarigo::msgpack::format::EXT8:
                return "EXT8";
            case tarigo::msgpack::format::EXT16:
                return "EXT16";
            case tarigo::msgpack::format::EXT32:
                return "EXT32";
            case tarigo::msgpack::format::FLOAT32:
                return "FLOAT32";
            case tarigo::msgpack::format::FLOAT64:
                return "FLOAT64";
            case tarigo::msgpack::format::UINT8:
                return "UINT8";
            case tarigo::msgpack::format::UINT16:
                return "UINT16";
            case tarigo::msgpack::format::UINT32:
                return "UINT32";
            case tarigo::msgpack::format::UINT64:
                return "UINT64";
            case tarigo::msgpack::format::INT8:
                return "INT8";
            case tarigo::msgpack::format::INT16:
                return "INT16";
            case tarigo::msgpack::format::INT32:
                return "INT32";
            case tarigo::msgpack::format::INT64:
                return "INT64";
            case tarigo::msgpack::format::FIXEXT1:
                return "FIXEXT1";
            case tarigo::msgpack::format::FIXEXT2:
                return "FIXEXT2";
            case tarigo::msgpack::format::FIXEXT4:
                return "FIXEXT4";
            case tarigo::msgpack::format::FIXEXT8:
                return "FIXEXT8";
            case tarigo::msgpack::format::FIXEXT16:
                return "FIXEXT16";
            case tarigo::msgpack::format::STR8:
                return "STR8";
            case tarigo::msgpack::format::STR16:
                return "STR16";
            case tarigo::msgpack::format::STR32:
                return "STR32";
            case tarigo::msgpack::format::ARRAY16:
                return "ARRAY16";
            case tarigo::msgpack::format::ARRAY32:
                return "ARRAY32";
            case tarigo::msgpack::format::MAP16:
                return "MAP16";
            case tarigo::msgpack::format::MAP32:
                return "MAP32";
            case tarigo::msgpack::format::NEGATIVE_FIXINT:
                return "NEGATIVE_FIXINT";
        }
    }
}

#endif //TARIGO_MSGPACK_UTIL_HPP