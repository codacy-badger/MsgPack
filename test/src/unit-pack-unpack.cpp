#include <catch.hpp>
#include <vector>
#include <system_error>

#include <msgpack/msgpack.hpp>

namespace msgpack = tarigo::msgpack;
using std::vector;
using std::error_code;

TEST_CASE("Pack/unpack nil", "[pack/unpack]") {
    msgpack::package nil, nil_explicit(nullptr);

    REQUIRE(nil == nil_explicit);
    REQUIRE(nil.type() == msgpack::type::NIL);
    REQUIRE(nil.format() == msgpack::format::NIL);

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

TEST_CASE("Pack/unpack boolean", "[pack/unpack]") {
    msgpack::package f(false), t(true);

    REQUIRE(f.type() == msgpack::type::BOOLEAN);
    REQUIRE(f.type() == t.type());

    REQUIRE(f.format() == msgpack::format::FALSE);
    REQUIRE(t.format() == msgpack::format::TRUE);

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
