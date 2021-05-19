
add_subdirectory(3rdParty/crypto)
include(buffer/buffer.cmake)
include(utils/utils.cmake)
include(algo/algo.cmake)
add_executable(data_types_v0_0_0 validator.h main.cpp configexception.h
        ${Buffer}
        ${Utils}
        ${CAlgo})
target_link_libraries(data_types_v0_0_0 crypto)