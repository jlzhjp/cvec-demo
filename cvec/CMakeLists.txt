cmake_minimum_required(VERSION 4.3)

project(cvec LANGUAGES C CXX)

add_library(cvec STATIC src/cvec.c)

target_include_directories(cvec PUBLIC include)

install(TARGETS cvec DESTINATION lib)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.18.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

if (BUILD_TESTING)
    add_executable(
        cvec_test
        tests/cvec_test.cpp
    )

    target_link_libraries(
        cvec_test
        PRIVATE
        cvec
        GTest::gtest_main
    )

    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang"
       AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(cvec PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
            $<$<CONFIG:Debug>:-fno-sanitize-recover=undefined>
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        target_compile_options(cvec_test PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
            $<$<CONFIG:Debug>:-fno-sanitize-recover=undefined>
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        target_link_options(cvec_test PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        )
    endif()

    include(GoogleTest)
    gtest_discover_tests(cvec_test)
endif()
