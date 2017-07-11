#pragma once
#ifndef TARIGO_MSGPACK_VALUES_HPP
#define TARIGO_MSGPACK_VALUES_HPP

#include <string>
#include <memory>

#include <msgpack/value_base.hpp>

namespace tarigo {
namespace msgpack {

// -----------------------------------------------------------------------------
// Nil.
// -----------------------------------------------------------------------------
class value_nil final : public value_base<type::NIL, nil_struct> {
public:
    explicit value_nil() : value_base({}) {}
    enum format format() const override { return format::NIL; }
};

// -----------------------------------------------------------------------------
// Boolean.
// -----------------------------------------------------------------------------
class value_boolean final : public value_base<type::BOOLEAN, bool> {
public:
    explicit value_boolean(bool value) : value_base(value) {}
    enum format format() const override {
        return value_ ? format::TRUE : format::FALSE;
    }
};

// -----------------------------------------------------------------------------
// String.
// -----------------------------------------------------------------------------
class value_string final : public value_base<type::RAW_STRING, std::string> {
public:
    explicit value_string(const std::string &value) : value_base(value) {}
    explicit value_string(std::string &&value) : value_base(std::move(value)) {}

    enum format format() const override {
        const size_t len = value_.size();
        if (len <= 0x1f) return format::FIXSTR;
        if (len <= 0xff) return format::STR8;
        if (len <= 0xffff) return format::STR16;
        return format::STR32;
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
            case type::INTEGER:
            case type::FLOAT:
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
            case type::INTEGER:
            case type::FLOAT:
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
    if (i >= 0 && i <= 127) return format::POSITIVE_FIXINT;

    if (std::is_signed<T>::value) {
        if (i >= -32 && i <= -1) return format::NEGATIVE_FIXINT;
    }

    if (std::is_same<T, int8_t>::value) return format::INT8;
    if (std::is_same<T, int16_t>::value) return format::INT16;
    if (std::is_same<T, int32_t>::value) return format::INT32;
    if (std::is_same<T, int64_t>::value) return format::INT64;

    if (std::is_same<T, uint8_t>::value) return format::UINT8;
    if (std::is_same<T, uint16_t>::value) return format::UINT16;
    if (std::is_same<T, uint32_t>::value) return format::UINT32;
    if (std::is_same<T, uint64_t>::value) return format::UINT64;


    return format::UNDEFINED;
}

// -----------------------------------------------------------------------------
// Integers.
// -----------------------------------------------------------------------------

class value_int8 final : public value_number<type::INTEGER, int8_t> {
public:
    explicit value_int8(int8_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int8_t >(value_);
    }
};

class value_int16 final : public value_number<type::INTEGER, int16_t> {
public:
    explicit value_int16(uint16_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int16_t >(value_);
    }
};

class value_int32 final : public value_number<type::INTEGER, int32_t> {
public:
    explicit value_int32(int32_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int32_t>(value_);
    }
};

class value_int64 final : public value_number<type::INTEGER, int64_t> {
public:
    explicit value_int64(int64_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<int64_t>(value_);
    }
};


class value_uint8 final : public value_number<type::INTEGER, uint8_t> {
public:
    explicit value_uint8(uint8_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint8_t>(value_);
    }
};

class value_uint16 final : public value_number<type::INTEGER, uint16_t> {
public:
    explicit value_uint16(uint16_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint16_t>(value_);
    }
};

class value_uint32 final : public value_number<type::INTEGER, uint32_t> {
public:
    explicit value_uint32(uint32_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint32_t>(value_);
    }
};

class value_uint64 final : public value_number<type::INTEGER, uint64_t> {
public:
    explicit value_uint64(uint64_t value) : value_number(value) {}

    enum format format() const override {
        return integer_format<uint64_t>(value_);
    }
};

// -----------------------------------------------------------------------------
// Floats.
// -----------------------------------------------------------------------------
class value_float final : public value_number<type::FLOAT, float> {
public:
    explicit value_float(float value) : value_number(value) {}
    enum format format() const override { return format::FLOAT32; }
};

class value_double final : public value_number<type::FLOAT, double> {
public:
    explicit value_double(double value) : value_number(value) {}
    enum format format() const override { return format::FLOAT64; }
};

// -----------------------------------------------------------------------------
// Binary.
// -----------------------------------------------------------------------------
class value_binary final : public value_base<type::RAW_BINARY,package::binary> {
    const package::binary &binary_items() const override { return value_; }
public:
    explicit value_binary(const package::binary &value) : value_base(value) {}
    explicit value_binary(package::binary &&value) : value_base(move(value)) {}

    enum format format() const override {
        const size_t len = value_.size();
        if(len <= 0xff) return format::BIN8;
        if (len <= 0xffff) return format::BIN16;
        return format::BIN32;
    }
};

// -----------------------------------------------------------------------------
// Array.
// -----------------------------------------------------------------------------
class value_array final : public value_base<type::ARRAY, package::array> {
public:
    explicit value_array(const package::array &values) : value_base(values) {}
    explicit value_array(package::array &&values) : value_base(move(values)) {}

public:
    enum format format() const override {
        const size_t len = value_.size();
        if (len <= 15) return format::FIXARRAY;
        if (len <= 0xffff) return format::ARRAY16;
        return format ::ARRAY32;
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
class value_map final : public value_base<type::MAP, package::map> {
public:
    explicit value_map(const package::map &value) : value_base(value) {}
    explicit value_map(package::map &&value)   : value_base(move(value)) {}

public:
    enum format format() const override {
        const size_t len = value_.size();
        if(len <= 15) return format::FIXMAP;
        if(len <= 0xffff) return format::MAP16;
        return format::MAP32;
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
