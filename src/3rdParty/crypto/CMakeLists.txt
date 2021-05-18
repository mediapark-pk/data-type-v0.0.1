file(GLOB crypto_headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB crypto_sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
add_library(crypto ${crypto_headers} ${crypto_sources})
