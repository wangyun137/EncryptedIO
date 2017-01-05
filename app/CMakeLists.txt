cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

set(external_DIR, ${CMAKE_SOURCE_DIR}/src/main/cpp/external)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

## Import the libcrypto.so

add_library(openssl-crypto
            SHARED
            IMPORTED )
set_target_properties(openssl-crypto
                      PROPERTIES
                      IMPORTED_LOCATION
                      ${external_DIR}/libs/${ANDROID_ABI}/libcrypto.so )
include_directories(${external_DIR}/include/openssl)

## Add the aes-crypto static library

aux_source_directory(${CMAKE_SOURCE_DIR}/src/main/cpp/crypto CRYPTO_SRCS)
add_library(aes-crypto
            STATIC
            ${CRYPTO_SRCS})
# target_link_libraries(aes-crypto)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/external/include
                    ${CMAKE_SOURCE_DIR}/src/main/cpp/io/include
                    ${CMAKE_SOURCE_DIR}/src/main/cpp/crypto/include)

## Import the libnativehelper.so

add_library(jni-native-helper SHARED IMPORTED)
set_target_properties(jni-native-helper
                      PROPERTIES
                      IMPORTED_LOCATION
                      ${external_DIR}/libs/${ANDROID_ABI}/libnativehelper.so)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/io/include/jni)

## Add the libio.so library

aux_source_directory(${CMAKE_SOURCE_DIR}/src/main/cpp/io IO_SRCS)
add_library(io
            SHARED
            ${IO_SRCS})

find_library(log-lib
             log)

target_link_libraries(io aes-crypto ${log-lib})
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/io/include
                    ${CMAKE_SOURCE_DIR}/src/main/cpp/crypto/include)





