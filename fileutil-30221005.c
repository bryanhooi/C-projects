/*
 * NAME: Bryan Hooi Yu Ern
 * STUDENT ID : 30221005
 * DATE CREATED : 10/09/2020
 * LAST UPDATED : 11/09/2020
 * DESCRIPTION : Main file that provides the functionalities as specified by the
 *               assignment.
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
#include "actions.h"
#include "parseArguments.h"

/*
 * Function that determines the action to be performed based on the arguments passed in along with its invocation
 * Input: argc <- int representing number of arguments, argv[] <- array of arguments (pointers to strings)
 * Return: None
 */
int main(int argc, char *argv[]) {
	
	int action = determineAction(argc, argv);
	char* src;
	char* dest;
	char* outString;
	
	switch (action) { // switch statement to handle all possible actions to be performed
		case 1: // display contents of logfile
			if (displayLogfile() == 1) {
				exit(1);
			} else {
				exit(0);
			}
		
		case 2: // display contents of user specified file
			dest = extractDestination(argc, argv);
			if (displayFile(dest) == 1) {
				exit(1);
			} else {
				exit(0);
			}
		
		case 3: // copy
			src = extractSource(argc, argv);
			dest = extractDestination(argc, argv);
			if (copy(src, dest) == 1) {
				outString = "Copy failed. Either source file already exists or destination directory doesn't exist.\n";
				write(2, outString, strlen(outString));
				exit(2);
			} else {
				outString = "Copy Successful\n";
				write(1, outString, strlen(outString));
				exit(0);
			}
			
		case 4: // move
			src = extractSource(argc, argv);
			dest = extractDestination(argc, argv);
			if (move(src, dest) == 1) {
				outString = "Move failed. Either source file already exists or destination directory doesn't exist.\n";
				write(2, outString, strlen(outString));
				exit(2);
			} else {
				outString = "Move Successful\n";
				write(1, outString, strlen(outString));
				exit(0);
			}
			
		case 5: // forced copy
			src = extractSource(argc, argv);
			dest = extractDestination(argc, argv);
			if (forcedCopy(src, dest) == 1) {
				outString = "Force Copy failed. Either source file or destination directory doesn't exist.\n";
				write(2, outString, strlen(outString));
				exit(2);
			} else {
				outString = "Forced Copy Successful\n";
				write(1, outString, strlen(outString));
				exit(0);
			}
			
		case 6: // forced move
			src = extractSource(argc, argv);
			dest = extractDestination(argc, argv);
			if (forcedMove(src, dest) == 1) {
				outString = "Force Move failed. Either source file or destination directory doesn't exist.\n";
				write(2, outString, strlen(outString));
				exit(2);
			} else {
				outString = "Forced Move Successful\n";
				write(1, outString, strlen(outString));
				exit(0);
			}
		
		default: // for action = 0 specifically, invalid set of arguments
			outString = "Invalid arguments.\n";
			write(2, outString, strlen(outString));
			exit(1);
	}
}
