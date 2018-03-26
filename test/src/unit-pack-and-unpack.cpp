#include <catch.hpp>
#include <msgpack/msgpack.hpp>

#include <vector>
#include <map>
#include <system_error>

namespace msgpack = tarigo::msgpack;

using std::vector;
using std::map;
using std::string;
using std::error_code;

TEST_CASE("Check nil value", "[pack/unpack]") {
    msgpack::package nil, nil_explicit(nullptr);

    REQUIRE(nil == nil_explicit);
    REQUIRE(nil.type() == msgpack::type::null);
    REQUIRE(nil.format() == msgpack::format::null);

    vector<uint8_t> raw_msgpack;
    nil.pack(raw_msgpack);
    REQUIRE(raw_msgpack.size() == 1);
    REQUIRE(raw_msgpack.at(0) == 0xc0);

    error_code ec;
    auto unpacked = msgpack::package::unpack(raw_msgpack, ec);
    REQUIRE(!ec);
    REQUIRE(nil == unpacked);
    REQUIRE(nil_explicit == unpacked);
}

TEST_CASE("Check boolean values", "[pack/unpack]") {
    msgpack::package f(false), t(true);

    REQUIRE(f.type() == msgpack::type::boolean);
    REQUIRE(f.type() == t.type());

    REQUIRE(f.format() == msgpack::format::boolean_false);
    REQUIRE(t.format() == msgpack::format::boolean_true);

    REQUIRE(f != t);
    REQUIRE(t > f);

    vector<uint8_t> raw_msgpack;
    f.pack(raw_msgpack);
    REQUIRE(raw_msgpack.size() == 1);
    REQUIRE(raw_msgpack.at(0) == 0xc2);

    error_code ec;
    auto unpacked_f = msgpack::unpacker::unpack(raw_msgpack, ec);
    REQUIRE(!ec);
    REQUIRE(f == unpacked_f);

    raw_msgpack.clear();

    t.pack(raw_msgpack);
    REQUIRE(raw_msgpack.size() == 1);
    REQUIRE(raw_msgpack.at(0) == 0xc3);

    auto unpacked_t = msgpack::unpacker::unpack(raw_msgpack, ec);
    REQUIRE(!ec);
    REQUIRE(t == unpacked_t);
}

TEST_CASE("Check fixed string", "[pack/unpack]") {
    map<string, vector<uint8_t>> sample {
        { "", {0xa0} },
        { "f", {0xa1, 0x66} },
        { "foo", {0xa3, 0x66, 0x6f, 0x6f} },
        { "fooo",  {0xa4, 0x66, 0x6f, 0x6f, 0x6f} },
        { "foooo", {0xa5, 0x66, 0x6f, 0x6f, 0x6f, 0x6f} },
        { "fooooo", {0xa6, 0x66, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f} }
        //TODO: ...
    };

    for (auto entry : sample) {
        msgpack::package package(entry.first);

        REQUIRE(package.type() == msgpack::type::string);
        REQUIRE(package.format() == msgpack::format::fixstr);

        vector<uint8_t > raw_msgpack;
        package.pack(raw_msgpack);

        REQUIRE(entry.second == raw_msgpack);
    }

    //TODO: Range tests for fixed values.
    map<string, vector<uint8_t>> one_byte_more_than_fixstr {
        {
            "this_is_not_a_fixed_string_32byt",
            {0xbe, 0x74, 0x68, 0x69, 0x73, 0x5f, 0x69, 0x73, 0x5f, 0x6e, 0x6f,
             0x74, 0x5f, 0x61, 0x5f, 0x66, 0x69, 0x78, 0x65, 0x64, 0x5f, 0x73,
             0x74, 0x72, 0x69, 0x6e, 0x67, 0x5f, 0x33, 0x32, 0x62}
        }
    };

    for(auto entry : one_byte_more_than_fixstr) {
        msgpack::package package(entry.first);
        REQUIRE(package.type() == msgpack::type::string);
        REQUIRE(package.format() != msgpack::format::fixstr);
        REQUIRE(package.format() == msgpack::format::str8);
    }
}

TEST_CASE("Check fixed map", "[pack/unpack]") {
    const vector<uint8_t> expected { 0x82, 0xa7, 0x63, 0x6f, 0x6d, 0x70,
                                     0x61, 0x63, 0x74, 0xc3, 0xa6, 0x73,
                                     0x63, 0x68, 0x65, 0x6d, 0x61, 0x00 };
    msgpack::package::map map {
            {"compact", true },
            //FIXME: should not require explicit casting
            {"schema", static_cast<uint8_t >(0) }
    };

    vector<uint8_t> raw_msgpack;
    msgpack::package package { map };

    package.pack(raw_msgpack);
    REQUIRE(raw_msgpack == expected);

    error_code ec;
    auto unpacked = msgpack::unpacker::unpack(expected, ec);
    REQUIRE(unpacked.type() == msgpack::type::map);
    REQUIRE(unpacked.format() == msgpack::format::fixmap);

    REQUIRE(package == unpacked);
}
