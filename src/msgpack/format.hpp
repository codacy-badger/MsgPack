#ifndef TARIGO_MSGPACK_FORMAT_HPP
#define TARIGO_MSGPACK_FORMAT_HPP

namespace tarigo::msgpack {

// MessagePack formats.
enum class format : unsigned char {
    positive_fixint,    // 0xxxxxxx | 0x00 - 0x7f
    fixmap,             // 1000xxxx | 0x80 - 0x8f
    fixarray,           // 1001xxxx	| 0x90 - 0x9f
    fixstr,             // 101xxxxx | 0xa0 - 0xbf
    null,               // 0xc0
    boolean_false,      // 0xc2
    boolean_true,       // 0xc3
    bin8,               // 0xc4
    bin16,              // 0xc5
    bin32,              // 0xc6
    ext8,               // 0xc7
    ext16,              // 0xc8
    ext32,              // 0xc9
    float32,            // 0xca
    float64,            // 0xcb
    uint8,              // 0xcc
    uint16,             // 0xcd
    uint32,             // 0xce
    uint64,             // 0xcf
    int8,               // 0xd0
    int16,              // 0xd1
    int32,              // 0xd2
    int64,              // 0xd3
    fixext1,            // 0xd4
    fixext2,            // 0xd5
    fixext4,            // 0xd6
    fixext8,            // 0xd7
    fixext16,           // 0xd8
    str8,               // 0xd9
    str16,              // 0xda
    str32,              // 0xdb
    array16,            // 0xdc
    array32,            // 0xdd
    map16,              // 0xde
    map32,              // 0xdf
    negative_fixint     // 0xe0 - 0xff
};

}

#endif //TARIGO_MSGPACK_FORMAT_HPP
