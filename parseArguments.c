/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Implementations of functions that help parse the arguments passed
 *               in along with the call to the fileutil program in order to determine
 *               the necessary actions to be made and also extract useful values.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "parseArguments.h"
#include "utilityFunctions.h"


/*
 * Function that assigns an int value depending on the action that is required
 * by the arguments provided in argv[].
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: an integer representing the type of action to be performed
 * 0 - invalid arguments
 * 1 - task 1
 * 2 - task 2
 * 3 - copy
 * 4 - move
 * 5 - force copy
 * 6 - force move
 */
int determineAction(int argc, char *argv[]) {
	int action;
	
	if (argc == 1) {
		action = 1;
	} else if (argc == 2) {
		action = 2;
	} else {
		if (isLegal(argc, argv)) { // checks if "-d {path}" is present
			if (containsM(argc, argv) && containsF(argc, argv)) {
				action = 6;
			} else if (containsM(argc, argv)) {
				action = 4;
			} else if (containsF(argc, argv)) {
				action = 5;
			} else {
				action = 3;
			}
		} else {
			action = 0;
		}
	}
	
	return action;
};

/*
 * Function that determines whether the given list of arguments is valid or not
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: 1 if list of arguments is valid, 0 otherwise
 */
int isLegal(int argc, char *argv[]) {
	for(int i = 1; i < argc-1; i++) {
		if (strcmp(argv[i], "-d") == 0 && !isOption(argv[i+1])) { // checks if a non-option argument appears directly after -d
			return 1;
		}
	}
	
	return 0;
};

/*
 * Function that determines whether the given list of arguments contains the -M option
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: 1 if list of arguments contains -M, 0 otherwise
 */
int containsM(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-M") == 0) {
			return 1;
		}
	}
	
	return 0;
};

/*
 * Function that determines whether the given list of arguments contains the -F option
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: 1 if list of arguments contains -M, 0 otherwise
 */
int containsF(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-F") == 0) {
			return 1;
		}
	}
	
	return 0;
};

/*
 * Function that extracts the string representing the source(file) from the list of arguments
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: pointer to a string representing the source file name allocated on the heap, NULL by default
 */
char* extractSource(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0) { // source is logfile.txt if no source files are specified prior to -d
			char* source = (char *) malloc(sizeof(char) * strlen("logfile.txt") + 1);
			char* logfile = "logfile.txt";
			source = strcpy(source, logfile);
			return source;
		} else if (!isOption(argv[i])) { // otherwise, source is the first non-option string found
			char* source = (char *) malloc(sizeof(char) * strlen(argv[i]) + 1);
			source = strcpy(source, argv[i]);
			return source;
		}
	}
	return NULL;
};

/*
 * Function that extracts the string representing the destination(directory) from the list of arguments
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: pointer to a string representing the directory path allocated on the heap, NULL by default
 */
char* extractDestination(int argc, char *argv[]) {
	for (int i = argc-1; i > 0; i--) { // searches through the arguments list backwards as directory path must come after -d
		if (!isOption(argv[i])) {
			char* dest = (char *) malloc(sizeof(char) * strlen(argv[i]) + 1);
			dest = strcpy(dest, argv[i]);
			return dest;
		}
	}
	return NULL;
};
