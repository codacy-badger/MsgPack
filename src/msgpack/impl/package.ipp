#ifndef TARIGO_MSGPACK_PACKAGE_IPP
#define TARIGO_MSGPACK_PACKAGE_IPP

#include <array>
#include <exception>

#include "msgpack/packer.hpp"
#include "msgpack/values.hpp"
#include "msgpack/unpacker.hpp"

namespace tarigo {
namespace msgpack {

using std::vector;
using std::string;
using std::make_shared;
using std::shared_ptr;

// -----------------------------------------------------------------------------
// Serialization.
// -----------------------------------------------------------------------------
void package::pack(std::vector<uint8_t> &out) const {
    ptr_->pack(out);
}

struct constants {
    const std::shared_ptr<value> nil = std::make_shared<value_nil>();
    constants() {}
};

static constants const& get_constants() {
    static const constants instance;
    return instance;
}

// -----------------------------------------------------------------------------
// Constructors.
// -----------------------------------------------------------------------------
package::package() noexcept : ptr_(get_constants().nil) {}
package::package(std::nullptr_t) noexcept : ptr_(get_constants().nil) {}

package::package(bool value) : ptr_(make_shared<value_boolean>(value)) {}

package::package(const string &value) :
        ptr_(make_shared<value_string>(value)) {}
package::package(string &&value) :
        ptr_(make_shared<value_string>(move(value))) {}
package::package(const char * value) :
        ptr_(make_shared<value_string>(value)) {}

package::package(const array &values) :
        ptr_(make_shared<value_array>(values)){}
package::package(array &&values) :
        ptr_(make_shared<value_array>(move(values))){}


package::package(const package::map &values) :
        ptr_(make_shared<value_map>(values)){}
package::package(package::map &&values) :
        ptr_(make_shared<value_map>(move(values))) {}

package::package(int8_t value) : ptr_(make_shared<value_int8>(value)) {}
package::package(int16_t value) : ptr_(make_shared<value_int16>(value)) {}
package::package(int32_t value) : ptr_(make_shared<value_int32>(value)) {}
package::package(int64_t value) : ptr_(make_shared<value_int64>(value)) {}

package::package(uint8_t value) : ptr_(make_shared<value_uint8>(value)) {}
package::package(uint16_t value) : ptr_(make_shared<value_uint16>(value)) {}
package::package(uint32_t value) : ptr_(make_shared<value_uint32>(value)) {}
package::package(uint64_t value) : ptr_(make_shared<value_uint64>(value)) {}

package::package(float value) : ptr_(make_shared<value_float>(value)) {}
package::package(double value) : ptr_(make_shared<value_double>(value)) {}

package::package(const binary &values) :
        ptr_(make_shared<value_binary>(values)) {}
package::package(binary &&values) :
        ptr_(make_shared<value_binary>(move(values))) {}

// -----------------------------------------------------------------------------
// Value getters.
// -----------------------------------------------------------------------------

enum type package::type()       const { return ptr_->type(); }
enum format package::format()   const { return ptr_->format(); }

float package::float_value()  const { return ptr_->float_value(); }
double package::double_value() const { return ptr_->double_value(); }

bool package::boolean_value() const { return ptr_->boolean_value(); }

int8_t  package::int8_value()    const { return ptr_->int8_value(); }
int16_t package::int16_value()   const { return ptr_->int16_value(); }
int32_t package::int32_value()   const { return ptr_->int32_value(); }
int64_t package::int64_value()   const { return ptr_->int64_value(); }

inline uint8_t  package::uint8_value()  const { return ptr_->uint8_value(); }
inline uint16_t package::uint16_value() const { return ptr_->uint16_value(); }
inline uint32_t package::uint32_value() const { return ptr_->uint32_value(); }
inline uint64_t package::uint64_value() const { return ptr_->uint64_value(); }

const std::string& package::string_value() const {
    return ptr_->string_value();
}

const package::array &package::array_items() const {
    return ptr_->array_items();
}

const package::map &package::map_items() const {
    return ptr_->map_items();
}

const package::binary &package::binary_items() const {
    return ptr_->binary_items();
}

// Comparisons.
// -----------------------------------------------------------------------------

bool package::operator== (const package &other) const {
    return ptr_->equals(other.ptr_.get());
}

bool package::operator< (const package &other) const {
    return ptr_->less(other.ptr_.get());
}

// Unpacking.
// -----------------------------------------------------------------------------
std::vector<package> package::unpack_sequence (const uint8_t *in,
                                               std::size_t in_size,
                                               size_t &successfully_parsed,
                                               size_t &stopped_at_position,
                                               std::error_code &ec) {
    return unpacker::unpack_sequence(in, in_size, successfully_parsed,
                                     stopped_at_position, ec);
}

package package::unpack(vector<uint8_t> const& in, std::error_code &ec) {
    return unpacker::unpack(in, ec);
}

std::vector<package> package::unpack_sequence (std::vector<uint8_t> const& in,
                                               size_t &successfully_parsed,
                                               size_t &stopped_at_position,
                                               std::error_code &ec) {
    return unpacker::unpack_sequence(in.data(), in.size(), successfully_parsed,
                                     stopped_at_position, ec);
}

std::vector<package> package::unpack_sequence (std::vector<uint8_t> const& in,
                                               size_t &stopped_at_position,
                                               std::error_code &ec) {
    size_t successfully_parsed;
    return unpacker::unpack_sequence(in.data(), in.size(), successfully_parsed,
                                     stopped_at_position, ec);
}


} // namespace msgpack
} // namespace tarigo

#endif // TARIGO_MSGPACK_PACKAGE_IPP