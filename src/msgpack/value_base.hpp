#ifndef TARIGO_MSGPACK_VALUE_BASE_HPP
#define TARIGO_MSGPACK_VALUE_BASE_HPP

#include <vector>

#include "msgpack/value.hpp"
#include "msgpack/packer.hpp"

namespace tarigo {
namespace msgpack {

template<type MsgPackType, typename NativeType>
class value_base : public value {
protected:
    explicit value_base(const NativeType &value) : value_(value) {}
    explicit value_base(NativeType &&value) : value_(std::move(value)) {}

    enum type type() const override { return MsgPackType; }

    bool equals(const value *other) const override {
        const bool is_same_type = MsgPackType == other->type();

        const bool is_same_native_type_value =
                value_ == static_cast<const value_base<
                        MsgPackType, NativeType> *>(other)->value_;

        return is_same_type && is_same_native_type_value;
    }

    bool less(const value *other) const override {
        const bool is_same_type = MsgPackType == other->type();
        return (is_same_type &&
                (value_ < static_cast<const value_base<
                        MsgPackType, NativeType> *>(other)->value_));
    }

    void pack(std::vector<uint8_t> &out) const override {
        msgpack::packer::pack(value_, out);
    }

    const NativeType value_;
};

} // namespace msgpack
} // namespace tarigo

#endif // TARIGO_MSGPACK_VALUE_BASE_HPP
