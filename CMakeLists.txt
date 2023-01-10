cmake_minimum_required(VERSION 3.23)
project(progettoLSO202223 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)

add_executable(Server.out Server/src/MainServer.c
        Server/src/SocketUtilServer.c Server/include/SocketUtilServer.h
        Server/src/CommUtilServer.c Server/include/CommUtilServer.h
        Server/src/ThreadService.c Server/include/ThreadService.h
        Server/src/ThreadRoom.c Server/include/ThreadRoom.h
        Server/src/LogicSigninLogin.c Server/include/LogicSigninLogin.h
        Server/src/LogicRoom.c Server/include/LogicRoom.h
        Server/src/ListHandler.c Server/include/ListHandler.h
        Server/src/LogicGame.c Server/include/LogicGame.h
        Server/include/Def.h
        Server/src/LogicRoom.c Server/include/LogicRoom.h)
add_executable(Client.out Client/src/MainClient.c
        Client/src/SocketUtilClient.c Client/include/SocketUtilClient.h
        Client/src/CommUtilClient.c Client/include/CommUtilClient.h
        Client/src/Prompt.c Client/include/ThreadPrompt.h
        Client/src/ConsoleLayouts.c Client/include/ConsoleLayouts.h
        Client/include/Def.h
        Client/src/PollSwitches.c Client/include/PollSwitches.h)
