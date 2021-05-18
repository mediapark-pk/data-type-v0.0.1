include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

file(GLOB utils_sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB utils_headers ${CMAKE_CURRENT_LIST_DIR}/*.h)

set(Utils ${utils_sources} ${utils_headers})
