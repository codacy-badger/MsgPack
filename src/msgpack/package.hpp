#ifndef TARIGO_MSGPACK_PACKAGE_HPP
#define TARIGO_MSGPACK_PACKAGE_HPP

#include <vector>
#include <map>
#include <memory>
#include <system_error>

#include "msgpack/format.hpp"
#include "msgpack/type.hpp"

namespace tarigo {
namespace msgpack {

class value;

class package final {
public:
    // -------------------------------------------------------------------------
    // Aliases.
    // -------------------------------------------------------------------------
    using array = std::vector<package>;
    using map = std::map<package, package>;
    using binary = std::vector<uint8_t>;

    // -------------------------------------------------------------------------
    // Constructors.
    // -------------------------------------------------------------------------
    inline package() noexcept;

    inline package(std::nullptr_t) noexcept;

    inline package(bool value);

    inline package(int8_t value);
    inline package(int16_t value);
    inline package(int32_t value);
    inline package(int64_t value);

    inline package(uint8_t value);
    inline package(uint16_t value);
    inline package(uint32_t value);
    inline package(uint64_t value);

    inline package(float value);
    inline package(double value);

    inline package(const std::string &value);
    inline package(std::string &&value);
    inline package(const char *value);

    inline package(const binary &value);
    inline package(binary &&value);

    inline package(const array &values);
    inline package(array &&values);

    inline package(const package::map &values);
    inline package(package::map &&values);

    template<class T, class = decltype(&T::to_msgpack)>
    package(const T &t) : package(t.to_msgpack()) {}

    template<class M, typename std::enable_if<
            std::is_constructible<package, typename M::key_type>::value
            && std::is_constructible<package, typename M::mapped_type>::value,
            int>::type = 0>
    package(const M &m) : package(map(m.begin(), m.end())) {}

    template<class V, typename std::enable_if<
            std::is_constructible<package, typename V::value_type>::value &&
            !std::is_same<typename binary::value_type,
                    typename V::value_type>::value, int>::type = 0>
    package(const V &v) : package(array(v.begin(), v.end())) {}

    template<class V, typename std::enable_if<
            std::is_constructible<package, typename V::value_type>::value &&
            std::is_same<typename binary::value_type,
                    typename V::value_type>::value, int>::type = 0>
    package(const V &v) : package(binary(v.begin(), v.end())) {}

    // -------------------------------------------------------------------------
    // Type and format getters.
    // -------------------------------------------------------------------------
    inline enum type type() const;
    inline enum format format() const;

    bool is_type_integer()      const { return type() == type::number; }
    bool is_type_nil()          const { return type() == type::null; }
    bool is_type_boolean()      const { return type() == type::boolean; };
    bool is_type_float()        const { return type() == type::number; }
    bool is_type_string()       const { return type() == type::string; }
    bool is_type_binary()       const { return type() == type::binary; }
    bool is_type_array()        const { return type() == type::array; }
    bool is_type_map()          const { return type() == type::map; }
    bool is_type_extension()    const { return type() == type::extension; }

    // -------------------------------------------------------------------------
    // Value getters.
    // -------------------------------------------------------------------------
    inline bool boolean_value() const;

    inline int8_t   int8_value() const;
    inline int16_t  int16_value() const;
    inline int32_t  int32_value() const;
    inline int64_t  int64_value() const;


    inline uint8_t   uint8_value() const;
    inline uint16_t  uint16_value() const;
    inline uint32_t  uint32_value() const;
    inline uint64_t  uint64_value() const;

    inline float float_value() const;
    inline double double_value() const;

    inline const std::string &string_value() const;

    inline const array &array_items() const;
    inline const map &map_items() const;
    inline const binary &binary_items() const;

    // -------------------------------------------------------------------------
    // Serialization
    // -------------------------------------------------------------------------
    inline void pack(std::vector<uint8_t> &out) const;

    // -------------------------------------------------------------------------
    // Deserialization
    // -------------------------------------------------------------------------
    inline static
    package unpack(std::vector<uint8_t> const &in, std::error_code &ec);

    inline static
    std::vector<package> unpack_sequence(const uint8_t *in,
                                         std::size_t in_size,
                                         size_t &successfully_parsed,
                                         size_t &stopped_at_position,
                                         std::error_code &ec);
    inline static
    std::vector<package> unpack_sequence(std::vector<uint8_t> const &in,
                                         size_t &stopped_at_position,
                                         std::error_code &ec);
    inline static
    std::vector<package> unpack_sequence(std::vector<uint8_t> const &in,
                                         size_t &successfully_parsed,
                                         size_t &stopped_at_position,
                                         std::error_code &ec);

    // -------------------------------------------------------------------------
    // Comparisons
    // -------------------------------------------------------------------------
    inline bool operator==(const package &rhs) const;
    inline bool operator<(const package &rhs) const;
    bool operator!= (const package &rhs) const { return !(*this == rhs); }
    bool operator<= (const package &rhs) const { return !(rhs < *this); }
    bool operator>  (const package &rhs) const { return  (rhs < *this); }
    bool operator>= (const package &rhs) const { return !(*this < rhs); }

private:
    std::shared_ptr<value> ptr_;
};

} //namespace msgpack
} //namespace tarigo

#include <msgpack/impl/package.ipp>

#endif // TARIGO_MSGPACK_PACKAGE_HPP
