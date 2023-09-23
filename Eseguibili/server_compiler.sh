#!/bin/bash


gcc ../Server/include/Def.h ../Server/src/SocketUtilServer.c ../Server/include/SocketUtilServer.h ../Server/src/CommUtilServer.c ../Server/include/CommUtilServer.h ../Server/src/ThreadService.c ../Server/include/ThreadService.h ../Server/src/ThreadRoom.c ../Server/include/ThreadRoom.h ../Server/src/LogicSigninLogin.c ../Server/include/LogicSigninLogin.h ../Server/src/ListHandler.c  ../Server/include/ListHandler.h ../Server/src/LogicGame.c  ../Server/include/LogicGame.h ../Server/src/ParserServer.c ../Server/src/MySqlUtil.c ../Server/include/MySqlUtil.h ../Server/src/LogUtilServer.c ../Server/include/LogUtilServer.h ../Server/src/MainServer.c -pthread -L/$USER/local/mysql/lib -lmysqlclient -lz -o Server.out
