#include "catch.hpp"

#include <iostream>
#include <msgpack/msgpack.hpp>

using namespace tarigo::msgpack;

TEST_CASE("Incomplete fixstring", "[unpacker]") {
    std::error_code ec;
    std::vector<uint8_t> buf { 0xa3, 0x66, 0x6f };

    auto pkg = unpacker::unpack(buf, ec);
    REQUIRE(ec);
    REQUIRE(ec == errc::unexpected_end_of_input);
    REQUIRE(pkg.type() == type::null);
    REQUIRE(pkg.format() == format::null);
}

TEST_CASE("Incomplete fixstring - format byte only", "[unpacker]") {
    std::error_code ec;
    std::vector<uint8_t> buf { 0xa3 };

    auto pkg = unpacker::unpack(buf, ec);
    REQUIRE(ec);
    REQUIRE(ec == errc::unexpected_end_of_input);
    REQUIRE(pkg.type() == type::null);
    REQUIRE(pkg.format() == format::null);
}


TEST_CASE("Incomplete str 8 - format byte only", "[unpacker]") {
    std::error_code ec;
    std::vector<uint8_t> buf { 0xd9, 0x20, 0x66, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f,
                               0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f,
                               0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f,
                               0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f,
                               0x6f, };
    auto pkg = unpacker::unpack(buf, ec);
    REQUIRE(ec);
    REQUIRE(ec == errc::unexpected_end_of_input);
    REQUIRE(pkg.type() == type::null);
    REQUIRE(pkg.format() == format::null);
}

TEST_CASE("Incomplete str 8", "[unpacker]") {
    std::error_code ec;
    std::vector<uint8_t> buf { 0xd9 };
    auto pkg = unpacker::unpack(buf, ec);
    REQUIRE(ec);
    REQUIRE(ec == errc::unexpected_end_of_input);
    REQUIRE(pkg.type() == type::null);
    REQUIRE(pkg.format() == format::null);
}
