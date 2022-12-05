cmake_minimum_required(VERSION 3.23)
project(progettoLSO202223 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)

add_executable(Server.out ./Server/src/MainServer.c ./Server/src/SocketUtilServer.c ./Server/src/ThreadService.c ./Server/src/ThreadRoom.c Server/src/LogicSigninLogin.c Server/include/LogicSigninLogin.h Server/src/LogicRoom.c Server/include/LogicRoom.h Server/src/CommUtilServer.c Server/include/CommUtilServer.h)
add_executable(Client.out ./Client/src/MainClient.c ./Client/src/SocketUtilClient.c)
