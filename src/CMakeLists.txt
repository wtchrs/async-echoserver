cmake_minimum_required(VERSION 3.8)

add_executable(${SERVER_OUTPUT} server.cpp)
target_link_libraries(${SERVER_OUTPUT} PRIVATE asio asio::asio)

add_executable(${CLIENT_OUTPUT} client.cpp)
target_link_libraries(${CLIENT_OUTPUT} PRIVATE asio asio::asio)
