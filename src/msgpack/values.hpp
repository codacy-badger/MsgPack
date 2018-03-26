#ifndef TARIGO_MSGPACK_VALUES_HPP
#define TARIGO_MSGPACK_VALUES_HPP

#include <string>
#include <memory>

#include "msgpack/value_base.hpp"

namespace tarigo {
namespace msgpack {

// -----------------------------------------------------------------------------
// Nil.
// -----------------------------------------------------------------------------
class value_nil final : public value_base<type::null, nil_struct> {
public:
    explicit value_nil() : value_base({}) {}
    enum format format() const override { return format::null; }
};

// -----------------------------------------------------------------------------
// Boolean.
// -----------------------------------------------------------------------------
class value_boolean final : public value_base<type::boolean, bool> {
public:
    explicit value_boolean(bool value) : value_base(value) {}
    enum format format() const override {
        return value_ ? format::boolean_true : format::boolean_false;
    }
};

// -----------------------------------------------------------------------------
// String.
// -----------------------------------------------------------------------------
class value_string final : public value_base<type::string, std::string> {
public:
    explicit value_string(const std::string &value) : value_base(value) {}
    explicit value_string(std::string &&value) : value_base(std::move(value)) {}

    enum format format() const override {
        const size_t len = value_.size();
        if (len <= 0x1f) return format::fixstr;
        if (len <= 0xff) return format::str8;
        if (len <= 0xffff) return format::str16;
        return format::str32;
    }

    const std::string &string_value() const override { return value_; }
};

// -----------------------------------------------------------------------------
// Generic numeric value.
// -----------------------------------------------------------------------------

template <type MsgPackType, typename NativeType>
class value_number: public value_base<MsgPackType, NativeType> {
protected:
    explicit value_number(const NativeType &value)
            : value_base<MsgPackType, NativeType>(value) {}
    explicit value_number(NativeType &&value)
            : value_base<MsgPackType, NativeType>(std::move(value)) {}

    //FIXME:
    bool equals(const value * other) const override {
        switch( other->type() )
        {
            case type::number:
            {
                return double_value() == other->double_value();
            } break;
            default:
            {
                return value_base<MsgPackType,NativeType>::equals(other);
            } break;
        }
    }

    //FIXME:
    bool less(const value * other) const override {
        switch( other->type() )
        {
            case type::number:
            {
                return double_value() < other->double_value();
            } break;
            default:
            {
                return value_base<MsgPackType,NativeType>::equals(other);
            } break;
        }
    }

    float float_value() const override {
        return static_cast<float>(value_base<MsgPackType,NativeType>::value_);
    }

    double double_value() const override {
        return static_cast<double>(value_base<MsgPackType,NativeType>::value_);
    }

    int8_t int8_value() const override {
        return static_cast<int8_t>(value_base<MsgPackType,NativeType>::value_);
    }

    int16_t int16_value() const override {
        return static_cast<int16_t>(value_base<MsgPackType,NativeType>::value_);
    }
    int32_t int32_value() const override {
        return static_cast<int32_t>(value_base<MsgPackType,NativeType>::value_);
    }

    int64_t int64_value() const override {
        return static_cast<int64_t>(value_base<MsgPackType,NativeType>::value_);
    }

    uint8_t uint8_value() const override {
        return static_cast<uint8_t>(value_base<MsgPackType,NativeType>::value_);
    }

    uint16_t uint16_value() const override {
        return static_cast<uint16_t>(value_base<MsgPackType,NativeType>::value_);
    }

    uint32_t uint32_value() const override {
        return static_cast<uint32_t>(value_base<MsgPackType,NativeType>::value_);
    }

    uint64_t uint64_value() const override {
        return static_cast<uint64_t>(value_base<MsgPackType,NativeType>::value_);
    }
};

template <typename T,
        class = typename std::enable_if<std::is_integral<T>::value>::type>
format integer_format(T i) {
    if (i >= 0) {
        if (i < 128) {
            return format::positive_fixint;
        } else if (i <= std::numeric_limits<uint8_t>::max()) {
            return format::uint8;
        } else if (i <= std::numeric_limits<uint16_t>::max()) {
            return format::uint16;
        } else if (i <= std::numeric_limits<uint32_t>::max()) {
            return format::uint32;
        } else if (i <= std::numeric_limits<uint64_t>::max()) {
            return format::uint64;
        }
    } else {
        if (i >= -32) {
            return format::negative_fixint;
        } else if (i >= std::numeric_limits<int8_t>::min() and
                   i <= std::numeric_limits<int8_t>::max()) {
            return format::int8;
        } else if (i >= std::numeric_limits<int16_t>::min() and
                   i <= std::numeric_limits<int16_t>::max()) {
            return format::int16;
        } else if (i >= std::numeric_limits<int32_t>::min() and
                   i <= std::numeric_limits<int32_t>::max()) {
            return format::int32;
        } else if (i >= std::numeric_limits<int64_t>::min() and
                   i <= std::numeric_limits<int64_t>::max()) {
            return format::int64;
        }
    }

    //return format::UNDEFINED;
}

// -----------------------------------------------------------------------------
// Integers.
// -----------------------------------------------------------------------------

class value_int8 final : public value_number<type::number, int8_t> {
public:
    explicit value_int8(int8_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int8_t >(value_);
    }
};

class value_int16 final : public value_number<type::number, int16_t> {
public:
    explicit value_int16(uint16_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int16_t >(value_);
    }
};

class value_int32 final : public value_number<type::number, int32_t> {
public:
    explicit value_int32(int32_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int32_t>(value_);
    }
};

class value_int64 final : public value_number<type::number, int64_t> {
public:
    explicit value_int64(int64_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int64_t>(value_);
    }
};


class value_uint8 final : public value_number<type::number, uint8_t> {
public:
    explicit value_uint8(uint8_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint8_t>(value_);
    }
};

class value_uint16 final : public value_number<type::number, uint16_t> {
public:
    explicit value_uint16(uint16_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint16_t>(value_);
    }
};

class value_uint32 final : public value_number<type::number, uint32_t> {
public:
    explicit value_uint32(uint32_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint32_t>(value_);
    }
};

class value_uint64 final : public value_number<type::number, uint64_t> {
public:
    explicit value_uint64(uint64_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint64_t>(value_);
    }
};

// -----------------------------------------------------------------------------
// Floats.
// -----------------------------------------------------------------------------
class value_float final : public value_number<type::number, float> {
public:
    explicit value_float(float value) : value_number(value) {}
    enum format format() const override { return format::float32; }
};

class value_double final : public value_number<type::number, double> {
public:
    explicit value_double(double value) : value_number(value) {}
    enum format format() const override { return format::float64; }
};

// -----------------------------------------------------------------------------
// Binary.
// -----------------------------------------------------------------------------
class value_binary final : public value_base<type::binary,package::binary> {
    const package::binary &binary_items() const override { return value_; }
public:
    explicit value_binary(const package::binary &value) : value_base(value) {}
    explicit value_binary(package::binary &&value) : value_base(move(value)) {}

    enum format format() const override {
        const size_t len = value_.size();
        if(len <= 0xff) return format::bin8;
        if (len <= 0xffff) return format::bin16;
        return format::bin32;
    }
};

// -----------------------------------------------------------------------------
// Array.
// -----------------------------------------------------------------------------
class value_array final : public value_base<type::array, package::array> {
public:
    explicit value_array(const package::array &values) : value_base(values) {}
    explicit value_array(package::array &&values) : value_base(move(values)) {}

public:
    enum format format() const override {
        const size_t len = value_.size();
        if (len <= 15) return format::fixarray;
        if (len <= 0xffff) return format::array16;
        return format::array32;
    }

    const package &operator[](size_t i) const override {
        if (i >= value_.size()) return static_nil();
        else return value_[i];
    }

    const package::array &array_items() const override { return value_; }
};

// -----------------------------------------------------------------------------
// Map.
// -----------------------------------------------------------------------------
class value_map final : public value_base<type::map, package::map> {
public:
    explicit value_map(const package::map &value) : value_base(value) {}
    explicit value_map(package::map &&value)   : value_base(move(value)) {}

public:
    enum format format() const override {
        const size_t len = value_.size();
        if(len <= 15) return format::fixmap;
        if(len <= 0xffff) return format::map16;
        return format::map32;
    }

    const package::map &map_items() const override { return value_; }

    const package & operator[](std::string const& key) const override {
        auto iter = value_.find(key);
        return (iter == value_.end()) ? static_nil() : iter->second;
    }
};

} // namespace msgpack
} // namespace tarigo

#endif // TARIGO_MSGPACK_VALUES_HPP
