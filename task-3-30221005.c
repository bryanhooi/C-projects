/*
 * NAME : BRYAN HOOI YU ERN
 * STUDENT ID : 30221005
 * START DATE : 24/10/2020
 * DATE LAST MODIFIED : 25/10/2020
 * DESCRIPTION : This program reads input from a provided or default text file
 *               and models them as processes on a computer system. It then
 *               facilitates the act of Priority-Based process scheduling
 *               on the given processes and writes the resulting data collected
 *               to a text file.
 */  

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "asgn2.h"

/*
 * Main function that gets executed upon a call to the program. Main simulates
 * Priority-Based process scheduling through calls to other functions
 * whose names are defined in the asgn2.h header file. The sequence of events
 * that occur upon execution are as follows: reading of input data from the specified
 * or default text file, organising the data into an array of Process Control Blocks,
 * perform the scheduling through the pbs_scheduler() function, write the results
 * of scheduling into a text file scheduler-result.txt.
 *
 * Arguments: argc <- an integer specifying the number of input arguments provided
 *                    to the call.
 *            char *argv[] <- an array of pointers to string literals that each
 *                            represent the arguments provided.
 * 
 * RETURN : 1 to standard error if any error occurs during execution, 0 otherwise.
 */
int main(int argc, char *argv[])
{
	FILE *dataFile;    // declaring a pointer variable of type FILE
	
	// If block to handle all possible combinations of input arguments to the program call
	if (argc > 2) {    // program doesnt accept more than 1 input argument
		printf("Invalid list of input arguments to program call.\n");
		exit(1);
	} else if (argc == 2) { // opens and reads file specified in argument
		if ((dataFile = fopen(argv[1], "r")) == NULL) {
			printf("Unsuccessful opening of input file.\n");
			exit(1);
		}
	} else {  // opens and reads default text file if no filename specified
		if ((dataFile = fopen("process-data.txt", "r")) == NULL) {
			printf("Unsuccessful opening of input file.\n");
			exit(1);
		}
	}
	
	printf("Successful opening of file.\n");
	
	const int MAX_LINE_SIZE = 30;
	char inputLine[MAX_LINE_SIZE];    // buffer to store each line read in
	pcb_store *process_list;          // struct that stores PCBs as an array of pointers
	
	// call to processData returns the number of separate processes that have been read in
	int num_processes = processData(process_list, inputLine, MAX_LINE_SIZE, dataFile);
	
	// runs the scheduler on the updated list of processes
	pbs_scheduler(process_list, num_processes);
	
	// write the required results onto an output text file. Exits the program
	// if any error occurs.
	if (writeResults(process_list, num_processes) == 1) {
		printf("Unsuccessful writing to output file.\n");
		exit(1);
	}
	
	// loops through each pointer in the array of PCB pointers and deallocating
	// the memory occupied by them.
	pcb_t *current;
	for (int i = 0; i < num_processes; i++) {
		current = process_list->processes[i];
		free(current);
	}
	
	// closing the opened file
	if (fclose(dataFile)) {
		printf("Error closing file.\n");
		exit(1);
	}
	
	return 0;
}
