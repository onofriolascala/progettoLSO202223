cmake_minimum_required(VERSION 3.24)
project(progettoLSO202223 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)

include(FindPkgConfig)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
endforeach()

link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})

add_executable(Server.out Server/src/MainServer.c
        Server/include/Def.h
        Server/src/SocketUtilServer.c Server/include/SocketUtilServer.h
        Server/src/CommUtilServer.c Server/include/CommUtilServer.h
        Server/src/ThreadService.c Server/include/ThreadService.h
        Server/src/ThreadRoom.c Server/include/ThreadRoom.h
        Server/src/LogicSigninLogin.c Server/include/LogicSigninLogin.h
        Server/src/ListHandler.c Server/include/ListHandler.h
        Server/src/LogicGame.c Server/include/LogicGame.h
        Server/src/ParserServer.c
        Server/src/MySqlUtil.c Server/include/MySqlUtil.h
        Server/src/LogUtilServer.c
        Server/src/LogUtilServer.c
        Server/include/LogUtilServer.h)
add_executable(Client.out Client/src/MainClient.c
        Client/include/Def.h
        Client/src/ConsoleLayouts.c Client/include/ConsoleLayouts.h
        Client/src/SocketUtilClient.c Client/include/SocketUtilClient.h
        Client/src/CommUtilClient.c Client/include/CommUtilClient.h
        Client/src/Prompt.c Client/include/Prompt.h
        Client/src/ParserClient.c Client/include/ParserClient.h
        Client/src/PollSwitches.c Client/include/PollSwitches.h
        Client/src/LogUtilClient.c Client/include/LogUtilClient.h)
add_executable(PromptBot1.out Client/src/MainClient.c
        Client/include/Def.h
        Client/src/ConsoleLayouts.c Client/include/ConsoleLayouts.h
        Client/src/SocketUtilClient.c Client/include/SocketUtilClient.h
        Client/src/CommUtilClient.c Client/include/CommUtilClient.h
        Client/src/PromptBot1.c Client/include/Prompt.h
        Client/src/ParserClient.c Client/include/ParserClient.h
        Client/src/PollSwitches.c Client/include/PollSwitches.h
        Client/src/LogUtilClient.c Client/include/LogUtilClient.h)

target_include_directories(Server.out PRIVATE ${LIBMYSQLCLIENT_INCLUDE_DIRS})
target_link_libraries(Server.out PRIVATE ${LIBMYSQLCLIENT_LIBRARIES})