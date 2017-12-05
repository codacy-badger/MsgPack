#pragma once
#ifndef TARIGO_MSGPACK_VALUE_HPP
#define TARIGO_MSGPACK_VALUE_HPP

#include <string>
#include <vector>
#include <memory>

#include "msgpack/package.hpp"

#ifdef nil
#undef nil
#endif

namespace tarigo {
namespace msgpack {

struct value_constants {
//    const std::shared_ptr<value> nil = std::make_shared<value_nil>();
    const std::string empty_string;
    const std::vector<package> empty_vector;
    const package::map empty_map;
    const package::binary empty_binary;
    value_constants() {}
};

static value_constants const& get_value_constants() {
    static const value_constants instance;
    return instance;
}

static const package & static_nil() {
    static const package nil;
    return nil;
}

class value {
public:
    virtual enum type type() const = 0;
    virtual enum format format() const = 0;
    virtual bool equals(const value *other) const = 0;
    virtual bool less(const value *other) const = 0;
    virtual void pack(std::vector<uint8_t> &out) const = 0;

    virtual bool boolean_value() const { return false; }

    virtual int8_t    int8_value()    const { return 0; }
    virtual int16_t   int16_value()   const { return 0; }
    virtual int32_t   int32_value()   const { return 0; }
    virtual int64_t   int64_value()   const { return 0; }

    virtual uint8_t   uint8_value()   const { return 0; }
    virtual uint16_t  uint16_value()  const { return 0; }
    virtual uint32_t  uint32_value()  const { return 0; }
    virtual uint64_t  uint64_value()  const { return 0; }

    virtual float float_value()   const { return 0.0; }
    virtual double double_value()  const { return 0.0; }

    virtual const std::string &string_value() const {
        return get_value_constants().empty_string;
    }

    virtual const package::array &array_items() const {
        return get_value_constants().empty_vector;
    }

    virtual const package::map &map_items() const {
        return get_value_constants().empty_map;
    }

    virtual const package::binary &binary_items() const {
        return get_value_constants().empty_binary;
    }

    virtual const package &operator[](size_t i) const {
        return static_nil();
    }

    virtual const package &operator[](std::string const& key) const {
        return static_nil();
    }

    virtual ~value() {}
};

} // namespace msgpack
} // namespace tarigo

#endif //TARIGO_MSGPACK_VALUE_HPP
