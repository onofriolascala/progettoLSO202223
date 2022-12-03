cmake_minimum_required(VERSION 3.23)
project(progettoLSO202223 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)

add_executable(Server.out MainServer.c)
add_executable(Client.out MainClient.c)
