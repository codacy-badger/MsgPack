#ifndef TARIGO_MSGPACK_TYPE_HPP
#define TARIGO_MSGPACK_TYPE_HPP

namespace tarigo {
namespace msgpack {

// MessagePack types.
enum class type : unsigned char {
    UNDEFINED,  // Not defined.
    INTEGER,    // Integer represents an integer.
    NIL,        // Nil represents nil.
    BOOLEAN,    // Boolean represents true or false.
    FLOAT,      // Float represents a IEEE 754 double precision
                // floating point number including NaN and Infinity.
    RAW_STRING, // String extending Raw type represents a UTF-8 string.
    RAW_BINARY, // Binary extending Raw type represents a byte array.
    ARRAY,      // Array represents a sequence of objects.
    MAP,        // Map represents key-value pairs of objects.
    EXTENSION   // Extension represents a tuple of type information
                // and a byte array where
                // type information is an integer whose.
};

} //namespace msgpack
} //namespace tarigo

#endif //TARIGO_MSGPACK_TYPE_HPP
