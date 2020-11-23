/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Implementations of functions used in the main file to execute
 *               the required tasks based on user arguments.
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
#include "actions.h"
#include "utilityFunctions.h"


/*
 * Function that outputs the contents of logfile.txt onto standard output
 * Input: None
 * Return: 0 if no errors occur, 1 otherwise
 */
int displayLogfile() {
	if (outputFile("logfile.txt") == 1) { // checking if any error has occurred
		return 1;
	}
	return 0;
}

/*
 * Function that outputs the contents of the file specified by path onto standard output
 * Input: path <- a pointer to char value allocated on the heap
 * Return: 0 if no errors occur, 1 otherwise
 */
int displayFile(char* path) {
	if (outputFile(path) == 1) { // checking if any error has occurred
		free(path);
		return 1;
	}
	free(path);
	return 0;
}

/*
 * Function that copies the contents of source(file) to destination(directory)
 * Input: source, destination (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int copy(char* source, char* destination) {
	if (copyToDirectory(source, destination) == 1) { // checking if any error has occurred
		free(source);
		free(destination);
		return 1;
	}
	free(source);
	free(destination);
	return 0;
}

/*
 * Function that moves the source file to the destination directory
 * Input: source, destination (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int move(char* source, char* destination) {
	if (moveToDirectory(source, destination) == 1) { // checking if any error has occurred
		free(source);
		free(destination);
		return 1;
	};
	free(source);
	free(destination);
	return 0;
}

/*
 * Function that forces the copying of the contents of source to the destination directory
 * Input: source, destination (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int forcedCopy(char* source, char* destination) {
	if (forceCopyToDirectory(source, destination) == 1) { // checking if any error has occurred
		free(source);
		free(destination);
		return 1;
	};
	free(source);
	free(destination);
	return 0;
}

/*
 * Function that forces the moving of the contents of source to the destination directory
 * Input: source, destination (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int forcedMove(char* source, char* destination) {
	if (forceMoveToDirectory(source, destination) == 1) { // checking if any error has occurred
		free(source);
		free(destination);
		return 1;
	};
	free(source);
	free(destination);
	return 0;
}
