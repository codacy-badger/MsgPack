#include <catch.hpp>
#include "msgpack/msgpack.hpp"

using namespace tarigo;

TEST_CASE("Consume packages from empty buffer", "[streambuf]") {
    std::vector<msgpack::package> packages;
    msgpack::streambuf streambuf;
    std::error_code error;
    auto consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 0);
    REQUIRE(!error);
}

TEST_CASE("Consume string", "[streambuf]") {
    uint8_t hello [] = { 0xa5, 0x48, 0x65, 0x6c, 0x6c, 0x6f };
    msgpack::streambuf streambuf;
    streambuf.sputn(hello, sizeof(hello));

    std::vector<msgpack::package> packages;
    std::error_code error;
    auto consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 1);
    REQUIRE(!error);
    REQUIRE(packages.size() == 1);
    REQUIRE(packages[0].is_type_string());
    REQUIRE(packages[0].string_value() == "Hello");
}

TEST_CASE("Consume incomplete array sequentially", "[streambuf]") {
    uint8_t data[] = { 0x91 };
    msgpack::streambuf streambuf;
    streambuf.sputn(data, sizeof(data));
    std::vector<msgpack::package> packages;
    std::error_code error;
    auto consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 0);
    REQUIRE(!error);

    streambuf.sputc(0x03);
    consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 1);
    REQUIRE(!error);
    REQUIRE(packages[0].is_type_array());
    REQUIRE(packages[0].array_items()[0].is_type_integer());
    REQUIRE(packages[0].array_items()[0].int32_value() == 3);
}


TEST_CASE("Consume complete string and incomplete buffer sequentially", "[streambuf]") {
    uint8_t data [] = { 0xa5, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x91 };
    msgpack::streambuf streambuf;
    streambuf.sputn(data, sizeof(data));

    std::vector<msgpack::package> packages;
    std::error_code error;
    auto consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 1);
    REQUIRE(!error);
    REQUIRE(packages.size() == 1);
    REQUIRE(packages[0].is_type_string());
    REQUIRE(packages[0].string_value() == "Hello");

    consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 0);
    REQUIRE(!error);

    streambuf.sputc(0x03);
    consumed = streambuf.consume_msgpack(packages, error);
    REQUIRE(consumed == 1);
    REQUIRE(!error);
    REQUIRE(packages[1].is_type_array());
    REQUIRE(packages[1].array_items()[0].is_type_integer());
    REQUIRE(packages[1].array_items()[0].int32_value() == 3);
}

TEST_CASE("Produce string", "[streambuf]") {
    std::vector<msgpack::package> packages = { msgpack::package("Hello!") };
    std::error_code error;
    msgpack::streambuf streambuf;
    std::basic_ostream<uint8_t> out(&streambuf);
    REQUIRE(streambuf.produce_msgpack(packages, error) == 7);
}
