#ifndef TARIGO_MSGPACK_UNPACKER_IPP
#define TARIGO_MSGPACK_UNPACKER_IPP

#include <functional>
#include <tuple>

#include "msgpack/error.hpp"
#include "msgpack/packer.hpp"

namespace tarigo::msgpack::unpacker {

using std::vector;

struct unpacker_ctx final {
    const uint8_t *buffer_ptr;
    const std::size_t buffer_size;
    size_t position;
    std::error_code ec;

    unpacker_ctx(const uint8_t *buff_ptr, size_t buff_size, size_t pos) :
            buffer_ptr(buff_ptr), buffer_size(buff_size), position(pos) {
    }

    size_t buffer_bytes_left() {
        return (buffer_size - position);
    }

    uint8_t get_first_byte() {
        if(buffer_size <= position) {
            ec = make_error_code(errc::unexpected_end_of_input);
            return 0xc1;
        }
        const uint8_t first_byte = buffer_ptr[position];
        ++position;
        return first_byte;
    }

    template< typename T> T unpack_number() {
        if(buffer_size <= position) {
            ec = make_error_code(errc::unexpected_end_of_input);
            return T(0);
        }
        packer::converter<T> converter;
        for( size_t j = 0; j < sizeof(T); ++j ) {
            converter.value.bytes[j] = buffer_ptr[position];
            ++position;
        }
        std::reverse(converter.value.bytes.begin(), converter.value.bytes.end());
        return converter.value.packed;
    }

    uint8_t unpack_positive_fixint(uint8_t first_byte) {
        return first_byte;
    }

    template< typename T > package::map unpack_map_impl(uint8_t, T bytes) {
        package::map res;
        for(T i = 0; i < bytes; ++i) {
            package key = unpack();
            package value = unpack();
            res.insert(std::make_pair(std::move(key), std::move(value)));
        }
        return res;
    }

    package::map unpack_fixmap(uint8_t first_byte) {
        const uint8_t bytes = first_byte & 0x0f;
        return unpack_map_impl<uint8_t>(first_byte, bytes);
    }

    template <typename T>
    package::map unpack_map(uint8_t first_byte) {
        T const bytes = unpack_number<T>();
        return unpack_map_impl<T>(first_byte, bytes);
    }

    template< typename T > package::array unpack_array_impl(T bytes) {
        package::array res;
        for(T i = 0; i < bytes; ++i) {
            res.emplace_back(unpack());
        }
        return res;
    }

    package::array unpack_fixarray(uint8_t first_byte) {
        const uint8_t bytes = first_byte & 0x0f;
        return unpack_array_impl<uint8_t>(bytes);
    }

    template <typename T>
    package::array unpack_array() {
        T const bytes = unpack_number<T>();
        return unpack_array_impl<T>(bytes);
    }

    std::nullptr_t unpack_nil(uint8_t) {
        return nullptr;
    }

    template< typename T >
    std::string unpack_string_impl(uint8_t, T bytes) {
        if (bytes > buffer_bytes_left()) {
            ec = make_error_code(errc::unexpected_end_of_input);
            return std::string();
        }
        std::string res(&(buffer_ptr[position]), &(buffer_ptr[position+bytes]));
        position += bytes;
        return res;
    }

    std::string unpack_fixstring(uint8_t first_byte) {
        const uint8_t bytes = first_byte & 0x1f;
        return unpack_string_impl<uint8_t>(first_byte, bytes);
    }

    bool unpack_boolean(uint8_t first_byte) {
        return (first_byte == 0xc3);
    }

    template <typename T>
    package::binary unpack_binary_impl(T bytes) {
        package::binary res;
        for(T j = 0; j < bytes; ++j) {
            res.push_back(buffer_ptr[position]);
            position++;
        }
        return res;
    }

    template <typename T>
    package::binary unpack_binary() {
        const T bytes = unpack_number<T>();
        return unpack_binary_impl<T>(bytes);
    }

    template <typename T>
    std::string unpack_string(uint8_t first_byte) {
        const T bytes = unpack_number<T>();
        return unpack_string_impl<T>(first_byte, bytes);
    }

    int8_t unpack_negative_fixint(uint8_t first_byte) {
        return *reinterpret_cast<int8_t*>(&first_byte);
    }

    package unpack() {
        uint8_t first_byte = get_first_byte();

        if (ec) return package();

        for (auto const &up : unpackers) {
            auto beg = std::get<0>(up);
            auto end = std::get<1>(up);

            if((beg <= first_byte) && (first_byte <= end)) {
                auto unpack_func = std::get<2>(up);
                auto result = unpack_func(this, first_byte);
                return ec ? package() : result;
            }
        }

        position--;
        ec = make_error_code(std::errc::illegal_byte_sequence);

        return package();
    }

    using element_unpacker =
    std::tuple<uint8_t, uint8_t, std::function<package(unpacker_ctx *, uint8_t)>>;

    const std::vector<element_unpacker> unpackers {
            //positive fixint
            element_unpacker{ 0x00u, 0x7fu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_positive_fixint(first_byte));
                              }},
            //fixmap
            element_unpacker{ 0x80u, 0x8f,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_fixmap(first_byte));
                              }},
            //fixarray
            element_unpacker{ 0x90u, 0x9fu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_fixarray(first_byte));
                              }},
            //fixstr
            element_unpacker{ 0xa0u, 0xbfu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_fixstring(first_byte));
                              }},
            //nil
            element_unpacker{ 0xc0u, 0xc0u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_nil(first_byte));
                              }},
            //boolean
            element_unpacker{ 0xc2u, 0xc3u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_boolean(first_byte));
                              }},
            //bin8
            element_unpacker{ 0xc4u, 0xc4u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_binary<uint8_t>());
                              }},
            //bin16
            element_unpacker{ 0xc5u, 0xc5u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_binary<uint16_t>());
                              }},
            //bin32
            element_unpacker{ 0xc5u, 0xc5u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_binary<uint32_t>());
                              }},
            //float32
            element_unpacker{ 0xcau, 0xcau,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<float>());
                              }},
            //float64
            element_unpacker{ 0xcbu, 0xcbu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<double>());
                              }},
            //uint8
            element_unpacker{ 0xccu, 0xccu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<uint8_t>());
                              }},
            //uint16
            element_unpacker{ 0xcdu, 0xcdu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<uint16_t>());
                              }},
            //uint32
            element_unpacker{ 0xceu, 0xceu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<uint32_t>());
                              }},
            //uint64
            element_unpacker{ 0xcfu, 0xcfu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<uint64_t>());
                              }},
            //int8
            element_unpacker{ 0xd0u, 0xd0u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<int8_t>());
                              }},
            //int16
            element_unpacker{ 0xd1u, 0xd1u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<int16_t>());
                              }},
            //int32
            element_unpacker{ 0xd2u, 0xd2u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<int32_t>());
                              }},
            //int64
            element_unpacker{ 0xd3u, 0xd3u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_number<int64_t>());
                              }},
            //str8
            element_unpacker{ 0xd9u, 0xd9u,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_string<uint8_t>(first_byte));
                              }},
            //str16
            element_unpacker{ 0xdau, 0xdau,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_string<uint16_t>(first_byte));
                              }},
            //str32
            element_unpacker{ 0xdau, 0xdau,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_string<uint32_t>(first_byte));
                              }},
            //array16
            element_unpacker{ 0xdcu, 0xdcu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_array<uint16_t>());
                              }},
            //array32
            element_unpacker{ 0xddu, 0xddu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_array<uint32_t>());
                              }},
            //map16
            element_unpacker{ 0xdeu, 0xdeu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_map<uint16_t>(first_byte));
                              }},
            //map32
            element_unpacker{ 0xdfu, 0xdfu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_map<uint32_t>(first_byte));
                              }},
            //negative fixint
            element_unpacker{ 0xe0u, 0xffu,
                              [](unpacker_ctx* that, uint8_t first_byte) -> package {
                                  return package(that->unpack_negative_fixint(first_byte));
                              }}
    };
};

inline package unpack(vector <uint8_t> const &in, std::error_code &ec) {
    unpacker_ctx context { in.data(), in.size(), 0 };
    auto result = context.unpack();
    ec = context.ec;
    return result;
}

inline std::vector<package> unpack_sequence (const uint8_t *in,
                                             std::size_t in_size,
                                             size_t &successfully_parsed,
                                             size_t &stopped_at_position,
                                             std::error_code &ec) {
    unpacker_ctx context { in, in_size, 0 };
    stopped_at_position = 0;
    successfully_parsed = 0;
    std::vector<package> packages;

    do  {
        auto pkg = context.unpack();
        if (!context.ec) {
            packages.push_back(pkg);
            successfully_parsed = context.position;
        }
    } while (context.position != in_size && !context.ec);

    stopped_at_position = context.position;
    ec = context.ec;

    return packages;
}

inline std::vector<package> unpack_sequence (std::vector<uint8_t> const& in,
                                             size_t &stopped_at_position,
                                             std::error_code &ec) {
    size_t successfully_parsed;
    return unpack_sequence(in.data(), in.size(),
                           successfully_parsed, stopped_at_position, ec);
}


}

#endif //TARIGO_MSGPACK_UNPACKER_IPP
