/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Header file for a set of functions implemented in utilityFunctions.c
 */

int outputFile(char* path);

int isOption(const char* argument);

char* readFromSource(const char* sourceFile);

int copyContents(char* buffer, char* newFileName);

int forceCopyContents(char* buffer, char* newFileName);

int fileExists(const char* filename);

int copyToDirectory(char* sourcePath, char* dirPath);

int moveToDirectory(char* sourcePath, char* dirPath);

int forceCopyToDirectory(char* sourcePath, char* dirPath);

int forceMoveToDirectory(char* sourcePath, char* dirPath);
