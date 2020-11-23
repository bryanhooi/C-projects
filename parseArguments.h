/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Header file for functions implemented in parseArguments.c
 */

int determineAction(int argc, char *argv[]);

int isLegal(int argc, char *argv[]);

int containsM(int argc, char *argv[]);

int containsF(int argc, char *argv[]);

char* extractSource(int argc, char *argv[]);

char* extractDestination(int argc, char *argv[]);
