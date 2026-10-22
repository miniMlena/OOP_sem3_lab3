cmake_minimum_required(VERSION 3.16)
project(Lab3_Variant14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

function(apply_sanitizers target)
    target_compile_options(${target} PRIVATE -g)

    if(ENABLE_ASAN)
        message(STATUS "AddressSanitizer enabled for ${target}")
        target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address)
    endif()

    if(ENABLE_UBSAN)
        message(STATUS "UndefinedBehaviorSanitizer enabled for ${target}")
        target_compile_options(${target} PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=undefined)
    endif()

    if(ENABLE_TSAN)
        message(STATUS "ThreadSanitizer enabled for ${target}")
        target_compile_options(${target} PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=thread)
    endif()
endfunction()

add_executable(lab3
    src/geo_coordinate.cpp
    src/geo_set.cpp
)

target_include_directories(lab3 PRIVATE src)
apply_sanitizers(lab3)

find_package(GTest REQUIRED)

add_executable(test_variant
    tests/test_variant.cpp
    src/geo_coordinate.cpp
    src/geo_set.cpp
)

target_include_directories(test_variant PRIVATE src)
target_link_libraries(test_variant PRIVATE GTest::gtest GTest::gtest_main)
apply_sanitizers(test_variant)