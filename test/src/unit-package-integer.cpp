#include <catch.hpp>
#include <iostream>
#include <msgpack/package.hpp>
#include <msgpack/util.hpp>


using namespace tarigo::msgpack;

template <typename T, format FORMAT> void checkTypeAndFormat(int min, int max) {
    for (int i = min; i <= max; i++) {
        package p(static_cast<T>(i));
        REQUIRE(p.type() == type::number);
        REQUIRE(p.format() == FORMAT);
    }
}

TEST_CASE("Integer - positive fixint", "[integer]") {
    checkTypeAndFormat<int8_t, format::positive_fixint>(0, 127);
    checkTypeAndFormat<int16_t, format::positive_fixint>(0, 127);
    checkTypeAndFormat<int32_t, format::positive_fixint>(0, 127);
    checkTypeAndFormat<int64_t, format::positive_fixint>(0, 127);

    checkTypeAndFormat<uint8_t, format::positive_fixint>(0, 127);
    checkTypeAndFormat<uint16_t, format::positive_fixint>(0, 127);
    checkTypeAndFormat<uint32_t, format::positive_fixint>(0, 127);
    checkTypeAndFormat<uint64_t, format::positive_fixint>(0, 127);
}

TEST_CASE("Integer - negative fixint", "[integer]") {
    checkTypeAndFormat<int8_t, format::negative_fixint>(-32, -1);
    checkTypeAndFormat<int16_t, format::negative_fixint>(-32, -1);
    checkTypeAndFormat<int32_t, format::negative_fixint>(-32, -1);
    checkTypeAndFormat<int64_t, format::negative_fixint>(-32, -1);
}

TEST_CASE("Integer - non fixints", "[integer]") {
    package int8(static_cast<int8_t>(128));
    package int16(static_cast<int16_t>(128));
    package int32(static_cast<int32_t>(128));
    package int64(static_cast<int64_t>(128));

    REQUIRE(int8.format() == format::int8);
    REQUIRE(int16.format() == format::int16);
    REQUIRE(int32.format() == format::int32);
    REQUIRE(int64.format() == format::int64);

    package uint8(static_cast<uint8_t>(128));
    package uint16(static_cast<uint16_t>(128));
    package uint32(static_cast<uint32_t>(128));
    package uint64(static_cast<uint64_t>(128));

    REQUIRE(uint8.format() == format::uint8);
    REQUIRE(uint16.format() == format::uint16);
    REQUIRE(uint32.format() == format::uint32);
    REQUIRE(uint64.format() == format::uint64);
}
