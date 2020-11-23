/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Implementations of a set of functions that are used by the functions
 *               in actions.c and parseArguments.c to properly perform most of the
 *               background work/computation necessary to achieve the required functionality
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
#include "utilityFunctions.h"

/*
 * Function that performs the outputting of the contents of logfile.txt onto standard output
 * Input: path <- a pointer to char value allocated on the heap
 * Return: 0 if no errors occur, 1 otherwise
 */
int outputFile(char* path) {
	int infile;
	
	if ((infile = open(path, O_RDONLY)) < 0) { // opens the file specified by path for reading
		char* outString = "Unable to open the file.\n";
		write(2, outString, strlen(outString));
		return 1;
	}
	
	off_t startOfFile = lseek(infile, 0, SEEK_SET);
	off_t endOfFile = lseek(infile, 0, SEEK_END);
	off_t lengthOfFile = endOfFile - startOfFile; // variable that holds the size of the opened file (number of chars)
	lseek(infile, 0, SEEK_SET);
	
	char buffer[lengthOfFile + 100];
	
	if (read(infile, buffer, lengthOfFile) == -1) { // reading the contents of the file onto the buffer
		char* outString = "Unable to read file contents\n";
		write(2, outString, strlen(outString));
		return 1;
	} else {
		if (write(1, buffer, strlen(buffer)) == -1) { // writing the contents of the buffer to standard output
			char* outString = "Unable to display file contents\n";
			write(2, outString, strlen(outString));
			return 1;
		}
	}
	
	close(infile);
	return 0;
}

/*
 * Function that determines if the given argument is an option or not
 * Input: argument <- a pointer to char value
 * Return: 1 if argument is an option, 0 otherwise
 */
int isOption(const char* argument) {
	return strcmp(argument, "-d") == 0 || strcmp(argument, "-M") == 0 || strcmp(argument, "-F") == 0;
}

/*
 * Function that returns a pointer to a buffer that contains the contents of the file specified by sourceFile
 * Input: sourceFile <- a pointer to char value allocated on the heap
 * Return: a pointer to the buffer allocated on the heap if no errors occur, NULL otherwise
 */
char* readFromSource(const char* sourceFile) {
	
	int source;
	if ((source = open(sourceFile, O_RDONLY)) == -1) {
		return NULL;
	}
	
	off_t startOfFile = lseek(source, 0, SEEK_SET);
	off_t endOfFile = lseek(source, 0, SEEK_END);
	off_t lengthOfFile = endOfFile - startOfFile;
	lseek(source, 0, SEEK_SET);
	
	char* buffer = (char *) malloc(sizeof(char) * lengthOfFile + 100); // dynamically allocating memory for the buffer
	
	if (read(source, buffer, lengthOfFile) == -1) {
		return NULL;
	}
	
	close(source);
	return buffer;
}

/*
 * Function that copies the contents of the input buffer to the file specified by newFileName
 * Input: buffer, newFileName <- pointers to char values
 * Return: 0 if no errors occur, 1 otherwise
 */
int copyContents(char* buffer, char* newFileName) {
	
	int newFile = open(newFileName, O_CREAT | O_WRONLY);
	
	if (write(newFile, buffer, strlen(buffer)) == -1) {
		return 1;
	}
	
	chmod(newFileName, 0666); // set the permission bits for the file to read & write for all parties
	close(newFile);
	return 0;
}

/*
 * Function that emulates copyContents but also overwrites the contents of the specified file if necessary
 * Input: buffer, newFileName <- pointers to char values
 * Return: 0 if no errors occur, 1 otherwise
 */
int forceCopyContents(char* buffer, char* newFileName) {
	
	int newFile = open(newFileName, O_CREAT | O_WRONLY | O_TRUNC);
	
	if (write(newFile, buffer, strlen(buffer)) == -1) {
		return 1;
	}
	
	chmod(newFileName, 0666);
	close(newFile);
	return 0;
}

/*
 * Function that checks if the given filename exists in the current working directory
 * Input: filename <- pointers to char values specifiying the name of the file
 * Return: 0 if the file exists, 1 otherwise
 */
int fileExists(const char* filename) {

	struct stat buffer;
	int exist = stat(filename, &buffer); // checks if the file can be accessed
	
	if (exist == 0) {
		return 1;
	} else {
		return 0;
	}
	
}

/*
 * Function that performs the copying of the contents of sourcePath (file) to the dirPath (directory)
 * Input: sourcePath, dirPath (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int copyToDirectory(char* sourcePath, char* dirPath) {
	
	char* sourceFileName = basename(sourcePath); // extracts the exact file name from the directory specified
	if (strlen(sourceFileName) == strlen(sourcePath) && strcmp(sourceFileName, "logfile.txt") != 0) {
		return 1;
	}
	
	char* buffer = readFromSource(sourcePath);
	if (buffer == NULL) {
		return 1;
	}
	
	if (chdir(dirPath) == -1) { // changes the current working directory to the one specified by dirPath
		return 1;
	}
	
	if (fileExists(sourceFileName)) { // checks if the extracted file name exists in the new directory
		return 1;
	}
	
	if (copyContents(buffer, sourceFileName) == 1) {
		return 1;
	}
	
	free(buffer);
	return 0;
}


/*
 * Function that performs the moving of the sourcePath (file) to the dirPath (directory)
 * Input: sourcePath, dirPath (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int moveToDirectory(char* sourcePath, char* dirPath) {
	
	char* sourceFileName = basename(sourcePath); // extracts the exact file name from the directory specified
	if (strlen(sourceFileName) == strlen(sourcePath) && strcmp(sourceFileName, "logfile.txt") != 0) {
		return 1;
	}
	
	char* buffer = readFromSource(sourcePath);
	if (buffer == NULL) {
		return 1;
	}
	
	if (chdir(dirPath) == -1) { // changes the current working directory to the one specified by dirPath
		return 1;
	}
	
	if (fileExists(sourceFileName)) { // checks if the extracted file name exists in the new directory
		return 1;
	}
	
	if (copyContents(buffer, sourceFileName) == 1) {
		return 1;
	}
	
	if (unlink(sourcePath) == -1) { // removes the file specified by sourcePath
		return 1;
	}
	
	free(buffer);
	return 0;
}

/*
 * Function that performs the copying of the sourcePath (file) to the dirPath (directory) regardless if the file already exists or not
 * Input: sourcePath, dirPath (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int forceCopyToDirectory(char* sourcePath, char* dirPath) {
	
	char* sourceFileName = basename(sourcePath); // extracts the exact file name from the directory specified
	if (strlen(sourceFileName) == strlen(sourcePath) && strcmp(sourceFileName, "logfile.txt") != 0) {
		return 1;
	}
	
	char* buffer = readFromSource(sourcePath);
	if (buffer == NULL) {
		return 1;
	}
	
	if (chdir(dirPath) == -1) { // changes the current working directory to the one specified by dirPath
		return 1;
	}
	
	if (forceCopyContents(buffer, sourceFileName) == 1) {
		return 1;
	}
	
	free(buffer);
	return 0;
}

/*
 * Function that performs the moving of the sourcePath (file) to the dirPath (directory) regardless if the file already exists or not
 * Input: sourcePath, dirPath (both are pointers to char values allocated on the heap)
 * Return: 0 if no errors occur, 1 otherwise
 */
int forceMoveToDirectory(char* sourcePath, char* dirPath) {
	
	char* sourceFileName = basename(sourcePath); // extracts the exact file name from the directory specified
	if (strlen(sourceFileName) == strlen(sourcePath) && strcmp(sourceFileName, "logfile.txt") != 0) {
		return 1;
	}
	
	char* buffer = readFromSource(sourcePath); 
	if (buffer == NULL) {
		return 1;
	}
	
	if (chdir(dirPath) == -1) { // changes the current working directory to the one specified by dirPath
		return 1;
	}
	
	if (forceCopyContents(buffer, sourceFileName) == 1) {
		return 1;
	}
	
	if (unlink(sourcePath) == -1) { // removes the file specified by sourcePath
		return 1;
	}
	
	free(buffer);
	return 0;
}
