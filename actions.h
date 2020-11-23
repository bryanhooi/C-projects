/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Header file for functions implemented in actions.c
 */

int displayLogfile();

int displayFile(char* path);

int copy(char* source, char* destination);

int move(char* source, char* destination);

int forcedCopy(char* source, char* destination);

int forcedMove(char* source, char* destination);
