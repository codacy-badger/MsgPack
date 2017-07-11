add_library(catch_main OBJECT "src/unit.cpp")
target_include_directories(catch_main PRIVATE "third_party")

set(MSGPACK_UNITTEST_TARGET_NAME "msgpack_unit")

add_executable(${MSGPACK_UNITTEST_TARGET_NAME}
                $<TARGET_OBJECTS:catch_main>
                src/unit-pack-unpack.cpp
                src/unit-package-integer.cpp
                src/unit-unpacker-incompete.cpp)
target_include_directories(${MSGPACK_UNITTEST_TARGET_NAME} PRIVATE "third_party")

set_target_properties(${MSGPACK_UNITTEST_TARGET_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

add_test(NAME "${MSGPACK_UNITTEST_TARGET_NAME}_default"
    COMMAND ${MSGPACK_UNITTEST_TARGET_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_test(NAME "${MSGPACK_UNITTEST_TARGET_NAME}_all"
    COMMAND ${MSGPACK_UNITTEST_TARGET_NAME} "*"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
