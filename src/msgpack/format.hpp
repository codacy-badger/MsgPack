#ifndef TARIGO_MSGPACK_FORMAT_HPP
#define TARIGO_MSGPACK_FORMAT_HPP

namespace tarigo {
namespace msgpack {

// MessagePack formats.
enum class format : unsigned char {
    UNDEFINED,          // Not defined.
    POSITIVE_FIXINT,    // 0xxxxxxx | 0x00 - 0x7f
    FIXMAP,             // 1000xxxx | 0x80 - 0x8f
    FIXARRAY,           // 1001xxxx	| 0x90 - 0x9f
    FIXSTR,             // 101xxxxx | 0xa0 - 0xbf
    NIL,                // 0xc0
    FALSE,              // 0xc2
    TRUE,               // 0xc3
    BIN8,               // 0xc4
    BIN16,              // 0xc5
    BIN32,              // 0xc6
    EXT8,               // 0xc7
    EXT16,              // 0xc8
    EXT32,              // 0xc9
    FLOAT32,            // 0xca
    FLOAT64,            // 0xcb
    UINT8,              // 0xcc
    UINT16,             // 0xcd
    UINT32,             // 0xce
    UINT64,             // 0xcf
    INT8,               // 0xd0
    INT16,              // 0xd1
    INT32,              // 0xd2
    INT64,              // 0xd3
    FIXEXT1,            // 0xd4
    FIXEXT2,            // 0xd5
    FIXEXT4,            // 0xd6
    FIXEXT8,            // 0xd7
    FIXEXT16,           // 0xd8
    STR8,               // 0xd9
    STR16,              // 0xda
    STR32,              // 0xdb
    ARRAY16,            // 0xdc
    ARRAY32,            // 0xdd
    MAP16,              // 0xde
    MAP32,              // 0xdf
    NEGATIVE_FIXINT     // 0xe0 - 0xff
};

} //namespace msgpack
} //namespace tarigo

#endif //TARIGO_MSGPACK_FORMAT_HPP