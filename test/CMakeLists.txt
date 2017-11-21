add_library(catch_main OBJECT "src/unit.cpp")
target_include_directories(catch_main PRIVATE "third_party")

find_package(Threads)

#############################################################################
# one executable for each unit test file
#############################################################################

file(GLOB files "src/unit-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})

    set_target_properties(${testcase} PROPERTIES
                          CXX_STANDARD 17
                          CXX_STANDARD_REQUIRED ON)

    target_include_directories(${testcase} PRIVATE "third_party")
    target_link_libraries(${testcase} mbedtls ${CMAKE_THREAD_LIBS_INIT})

    add_test(NAME "${testcase}_default"
      COMMAND ${testcase} ${CATCH_TEST_FILTER}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_default" PROPERTIES LABELS "default")

    add_test(NAME "${testcase}_all"
      COMMAND ${testcase} ${CATCH_TEST_FILTER} "*"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_all" PROPERTIES LABELS "all")
endforeach()
