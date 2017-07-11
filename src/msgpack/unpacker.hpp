#pragma once
#ifndef TARIGO_MSGPACK_UNPACKER_HPP
#define TARIGO_MSGPACK_UNPACKER_HPP

#include <system_error>
#include <vector>
#include <cstddef>
#include <cstdint>

#include <msgpack/package.hpp>

namespace tarigo {
namespace msgpack {
namespace unpacker {

package unpack(std::vector <uint8_t> const &in, std::error_code &ec);

std::vector<package> unpack_sequence(std::vector<uint8_t> const& in,
                                     size_t &stopped_at_position,
                                     std::error_code &ec);

} // namespace unpacker
} // namespace msgpack
} // namespace tarigo

#include <msgpack/impl/unpacker.ipp>

#endif // TARIGO_MSGPACK_UNPACKER_HPP