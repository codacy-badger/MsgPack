#ifndef TARIGO_MSGPACK_TYPE_HPP
#define TARIGO_MSGPACK_TYPE_HPP

namespace tarigo::msgpack {

enum class type : unsigned char {
    null,
    boolean,
    number,
    string,
    binary,
    array,
    map,
    extension
};

}

#endif //TARIGO_MSGPACK_TYPE_HPP
