cmake_minimum_required(VERSION 3.16)
project(edenmember)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(clsdk REQUIRED)


add_executable(edenmember src/edenmember.cpp)
target_link_libraries(edenmember eosio-contract-simple-malloc)


add_executable(edenmember-abigen src/edenmember.cpp)
target_link_libraries(edenmember-abigen eosio-contract-abigen)
add_custom_command(TARGET edenmember-abigen POST_BUILD
    COMMAND cltester edenmember-abigen.wasm >edenmember.abi
)


execute_process(COMMAND ln -sf ${clsdk_DIR} ${CMAKE_CURRENT_BINARY_DIR}/clsdk)
execute_process(COMMAND ln -sf ${WASI_SDK_PREFIX} ${CMAKE_CURRENT_BINARY_DIR}/wasi-sdk)


set(CMAKE_EXPORT_COMPILE_COMMANDS on)