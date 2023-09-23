#!/bin/bash


gcc ../Client/src/MainClient.c ../Client/include/Def.h ../Client/src/ConsoleLayouts.c ../Client/include/ConsoleLayouts.h ../Client/src/SocketUtilClient.c ../Client/include/SocketUtilClient.h ../Client/src/CommUtilClient.c ../Client/include/CommUtilClient.h ../Client/src/Prompt.c ../Client/include/Prompt.h ../Client/src/ParserClient.c ../Client/include/ParserClient.h ../Client/src/PollSwitches.c ../Client/include/PollSwitches.h ../Client/src/LogUtilClient.c ../Client/include/LogUtilClient.h -pthread -L/$USER/local/mysql/lib -lmysqlclient -lz -o Client.out
