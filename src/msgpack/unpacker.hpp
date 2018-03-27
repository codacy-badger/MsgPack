#ifndef TARIGO_MSGPACK_UNPACKER_HPP
#define TARIGO_MSGPACK_UNPACKER_HPP

#include <system_error>
#include <vector>
#include <cstddef>
#include <cstdint>

#include "msgpack/package.hpp"

namespace tarigo::msgpack::unpacker {

package unpack(std::vector <uint8_t> const &in, std::error_code &ec);

std::vector<package> unpack_sequence(std::vector<uint8_t> const& in,
                                     size_t &stopped_at_position,
                                     std::error_code &ec);
}

#include "msgpack/impl/unpacker.ipp"

#endif // TARIGO_MSGPACK_UNPACKER_HPP
