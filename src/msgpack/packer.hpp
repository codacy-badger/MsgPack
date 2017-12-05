#ifndef TARIGO_MSGPACK_PACKER_HPP
#define TARIGO_MSGPACK_PACKER_HPP

#include <string>
#include <vector>
#include <algorithm>

namespace tarigo {
namespace msgpack {

struct nil_struct {
    bool operator==(nil_struct) const { return true; }
    bool operator<(nil_struct)  const { return false; }
};

namespace packer {

template<typename T> struct converter {
    union {
        T packed;
        std::array<uint8_t, sizeof(T)> bytes;
    } value;
};

template<typename T>
void pack_data(T value, std::vector<uint8_t> &out) {
    converter<T> c;
    c.value.packed = value;
    std::for_each(c.value.bytes.rbegin(), c.value.bytes.rend(),
                  [&](uint8_t byte) {
                      out.push_back(byte);
                  });
}

inline void pack(nil_struct, std::vector<uint8_t> &out) {
    out.push_back(0xc0);
}

inline void pack(bool value, std::vector<uint8_t> &out) {
    const uint8_t b = (value) ? 0xc3 : 0xc2;
    out.push_back(b);
}

inline void pack(int8_t value, std::vector<uint8_t> &out) {
    if( value < -32 ) {
        out.push_back(0xd0);
    }
    out.push_back(value);
}

inline void pack(int16_t value, std::vector<uint8_t> &out) {
    out.push_back(0xd1);
    pack_data(value, out);
}

inline void pack(int32_t value, std::vector<uint8_t> &out) {
    out.push_back(0xd2);
    pack_data(value, out);
}

inline void pack(int64_t value, std::vector<uint8_t> &out) {
    out.push_back(0xd3);
    pack_data(value, out);
}

inline void pack(uint8_t value, std::vector<uint8_t> &out) {
    if(128 <= value) {
        out.push_back(0xcc);
    }
    out.push_back(value);
}

inline void pack(uint16_t value, std::vector<uint8_t> &out) {
    out.push_back(0xcd);
    pack_data(value, out);
}

inline void pack(uint32_t value, std::vector<uint8_t> &out) {
    out.push_back(0xce);
    pack_data(value, out);
}

inline void pack(uint64_t value, std::vector<uint8_t> &out) {
    out.push_back(0xcf);
    pack_data(value, out);
}

inline void pack(float value, std::vector<uint8_t> &out) {
    out.push_back(0xca);
    pack_data(value, out);
}

inline void pack(double value, std::vector<uint8_t> &out) {
    out.push_back(0xcb);
    pack_data(value, out);
}

inline void pack(std::string const &value, std::vector<uint8_t> &out) {
    size_t const len = value.size();
    if (len <= 0x1f) {
        uint8_t const first_byte = 0xa0 | static_cast<uint8_t>(len);
        out.push_back(first_byte);
    } else if (len <= 0xff) {
        uint8_t const length = static_cast<uint8_t>(len);
        out.push_back(0xd9);
        out.push_back(length);
    } else if (len <= 0xffff) {
        uint16_t const length = static_cast<uint16_t>(len);
        out.push_back(0xda);
        pack_data(length, out);
    } else if (len <= 0xffffffff) {
        uint32_t const length = static_cast<uint32_t>(len);
        out.push_back(0xdb);
        pack_data(length, out);
    } else {
        throw std::invalid_argument("exceeded maximum data length");
    }

    std::for_each(std::begin(value), std::end(value), [&out](char v) {
        pack_data(v, out);
    });
}

inline void pack(const package::binary& value, std::vector<uint8_t> &out) {
    size_t const len = value.size();
    if(len <= 0xff) {
        uint8_t const length = static_cast<uint8_t>(len);
        out.push_back(0xc4);
        pack_data(length, out);
    } else if(len <= 0xffff) {
        uint16_t const length = static_cast<uint16_t>(len);
        out.push_back(0xc5);
        pack_data(length, out);
    } else if(len <= 0xffffffff) {
        uint32_t const length = static_cast<uint32_t>(len);
        out.push_back(0xc6);
        pack_data(length, out);
    } else {
        throw std::invalid_argument("exceeded maximum data length");
    }

    std::for_each(std::begin(value), std::end(value),
                  [&out](package::binary::value_type const& v){
                      out.push_back(v);
                  });
}

inline void pack(const package::array &values, std::vector<uint8_t> &out) {
    size_t const len = values.size();
    if (len <= 15) {
        uint8_t const first_byte = 0x90 | static_cast<uint8_t>(len);
        out.push_back(first_byte);
    } else if (len <= 0xffff) {
        uint16_t const length = static_cast<uint16_t>(len);
        out.push_back(0xdc);
        pack_data(length, out);
    } else if (len <= 0xffffffff) {
        uint32_t const length = static_cast<uint32_t>(len);
        out.push_back(0xdd);
        pack_data(length, out);
    } else {
        throw std::invalid_argument("exceeded maximum data length");
    }

    std::for_each(std::begin(values), std::end(values),
                  [&out](package::array::value_type const &v) { v.pack(out); });
}

inline void pack(const package::map& value, std::vector<uint8_t> &out) {
    size_t const len = value.size();
    if(len <= 15) {
        uint8_t const first_byte = 0x80 | static_cast<uint8_t>(len);
        out.push_back(first_byte);
    } else if(len <= 0xffff) {
        uint16_t const length = static_cast<uint16_t>(len);
        out.push_back(0xde);
        pack_data(length, out);
    } else if(len <= 0xffffffff) {
        uint32_t const length = static_cast<uint32_t>(len);
        out.push_back(0xdf);
        pack_data(length, out);
    } else {
        throw std::invalid_argument("exceeded maximum data length");
    }

    std::for_each(std::begin(value), std::end(value),
                  [&out](package::map::value_type const& v){
                      v.first.pack(out);
                      v.second.pack(out);
                  });
}

} // namespace packer
} // namespace msgpack
} // namespace tarigo

#endif // TARIGO_MSGPACK_PACKER_HPP
