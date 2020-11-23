/*
 * NAME : BRYAN HOOI YU ERN
 * STUDENT ID : 30221005
 * START DATE : 24/10/2020
 * DATE LAST MODIFIED : 25/10/2020
 * DESCRIPTION : File that contains the implementations of helper functions that
 *               will be utilized in the main c files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "asgn2.h"

/*
 * Function that simulates the effect of the modulo operator on two positive
 * integers.
 
 * ARGUMENTS : a <- int representing the first integer
 *             b <- int representing the second integer
 *
 * RETURN : r <- result of a (modulo) b
 */
int mod(int a, int b) {
	int r = a % b;
	return r < 0 ? r + b : r;
}

/*
 * Function that reads in the lines of text in the provided file and assigns the
 * data into a Process Control Block (pcb_t struct) accordingly before adding it
 * to the provided array of PCBS.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             buffer <- pointer to an allocated array of chars that will be assigned
 *                       with the read in line of text.
 *             max_buf_size <- an int that represents the maximum size of the buffer
 *             file <- pointer to an opened file for reading
 * 
 * RETURN : i <- int representing the number of lines/processes read in
 */           
int processData(pcb_store *storage, char *buffer, int max_buf_size, FILE *file)
{
	// declarations of the variables required to represent the data for a process
	int i = 0, a_time, s_time, deadline;
	char p_name[11];
	
	// reads line by line terminated by a \n until the end of file is reached
	while (fgets(buffer, max_buf_size, file)) {
		// splits the data from a single line and assigns them to the declared variables
		sscanf(buffer, "%s %d %d %d", p_name, &a_time, &s_time, &deadline);
		
		// dynamically allocate memory for a single process and populating its
		// data attributes accordingly before adding them to the array of PCBS.
		pcb_t *newProcess = (pcb_t *) malloc(sizeof(pcb_t));
		submitProcess(storage, newProcess, i, p_name, a_time, s_time, deadline);
		i++;
	}
	
	return i;
}

/*
 * Function that populates the data attribute values of the given PCB accordingly.
 * The input arguments that are associated with the data attributes of the PCB
 * are taken directly from the text lines of the input file.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             newProcess <- pointer to a PCB to be updated
 *             index <- int representing the position in storage for which the PCB
 *                      will be assigned to.
 *             name <- string representing the process name
 *             arrival <- int representing time in which the process entered the system
 *             service <- int representing the total CPU time required by the process
 *             deadline <- int representing the expected maximum time from the arrival
 *                         to the completion of the process.
 *
 * RETURN : No return value
 */
void submitProcess(pcb_store *storage, pcb_t *newProcess, int index, char *name, int arrival, int service, int deadline)
{
	// following lines just assign the given argument values to their corresponding
	// PCB attributes.
	strcpy(newProcess->process_name, name);
	newProcess->entryTime = arrival;
	newProcess->serviceTime = service;
	newProcess->remainingTime = service;
	newProcess->deadline = deadline;
	newProcess->completionTime = -1;
	newProcess->startTime = -1;
	newProcess->state = READY;
	
	// assigns the pointer to the PCB to the element at position index in storage
	storage->processes[index] = newProcess;
}

/*
 * Function that determines the process in the ready state and having the lowest
 * arrival time among the processes in the input list of PCBs.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 *
 * RETURN : processIndex <- an int corresponding to the index of storage containing
 *                          a process in the ready state with the minimal arrival time,
 *                          50 otherwise.
 */
int nextProcess(pcb_store *storage, const int num_processes)
{
	int processIndex = 50;   // processIndex default value is  50 because storage only has max 10 elements
	int minArrivalTime = (int) INFINITY;
	
	// looping through the processes in storage and assigning the index of the process with the
	// least arrival time to processIndex while ignoring those that aren't in the ready state
	for (int i = 0; i < num_processes; i++) {
		pcb_t *currentProcess = storage->processes[i];
		if (currentProcess->state != EXIT && currentProcess->entryTime < minArrivalTime) {
			minArrivalTime = currentProcess->entryTime;
			processIndex = i;
		}
	}
	
	return processIndex;
}

/*
 * Function that determines the process in the ready state and having the lowest
 * arrival time among the processes in the input list of PCBs.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 *
 * RETURN : processIndex <- an int corresponding to the index of storage containing
 *                          a process in the ready state with the minimal arrival time,
 *                          50 otherwise.
 */
int nextPriorityProcess(pcb_store *storage, const int num_processes) 
{
	int processIndex = 50;   // processIndex default value is  50 because storage only has max 10 elements
	int minPriority = (int) INFINITY;
	
	// looping through the processes in storage and assigning the index of the process with the
	// lowest deadline to processIndex while ignoring those that aren't in the ready state
	for (int i = 0; i < num_processes; i++) {
		pcb_t *currentProcess = storage->processes[i];
		if (currentProcess->state != EXIT && currentProcess->deadline < minPriority) {
			minPriority = currentProcess->deadline;
			processIndex = i;
		}
	}
	
	return processIndex;
}

/*
 * Function that determines which process (if any) in the given list of PCBs that
 * has the same arrival time as the given input time value.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 *             time <- constant int that represents a particular time in seconds
 * 
 * RETURN : processIndex <- an int corresponding to the index of storage containing
 *                          a process with the same arrival time as the given input,
 *                          50 otherwise.
 */ 
int processEntry(pcb_store *storage, const int num_processes, const int time)
{
	int processIndex = 50;   // processIndex default value is  50 because storage only has max 10 elements
	
	// looping through the processes in storage until a process with the same
	// arrival time as the given input is found.
	for (int i = 0; i < num_processes; i++) {
		if (storage->processes[i]->entryTime == time) {
			processIndex = i;
			break;
		}
	}
	return processIndex;
}

/*
 * Function that places a given process represented by its index in storage
 * in the running state.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             processId <- int representing the index of the process in storage
 *                          to be handled.
 *             time <- constant int that represents a particular time in seconds
 *
 * RETURN : 1 if the given process is placed in the running state, 0 otherwise
 */
int handleRunningProcess1(pcb_store *storage, const int processId, const int time)
{
	if (processId != 50 && storage->processes[processId]->state == ENTEREDNREADY) {
		printf("Time %d: %s is in the running state.\n", time, storage->processes[processId]->process_name);
		storage->processes[processId]->startTime = time;
		storage->processes[processId]->state = RUNNING;
		storage->processes[processId]->remainingTime--;
		return 1;
	}
	return 0;
}

/*
 * Function that places a given process represented by its index in storage
 * in the running state.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             processId <- int representing the index of the process in storage
 *                          to be handled.
 *             time <- constant int that represents a particular time in seconds
 *
 * RETURN : 1 if the given process is placed in the running state, 0 otherwise
 */
int handleRunningProcess2(pcb_store *queue, const int processId, const int time)
{
	if (processId != 50 && queue->processes[processId]->state == ENTEREDNREADY) {
		printf("Time %d: %s is in the running state.\n", time, queue->processes[processId]->process_name);
		
		// only updates the startTime once
		if (queue->processes[processId]->startTime == -1) {
			queue->processes[processId]->startTime = time;
		}
		
		queue->processes[processId]->state = RUNNING;
		queue->processes[processId]->remainingTime--;
		return 1;
	}
	return 0;
}

/*
 * Function that iterates through each process in the given list of PCBs, performs
 * some calculations on their data attributes to produce the required values, and
 * writing them onto a text file.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 *
 * RETURN : 1 if any errors occurred during execution, 0 otherwise
 */
int writeResults(pcb_store *storage, const int num_processes)
{
	FILE *outputFile;  // declare a pointer to a FILE object
	
	// return 1 if an error occurs while opening the file
	if ((outputFile = fopen("scheduler-result.txt", "w")) == NULL) {
		return 1;
	}
	
	// for each process in storage, calculate the wait time, turnaround time, and if
	// the deadline has been met or not, and writing each result in a single line along
	// with the process name onto the file. Return 1 if any errors occurred during writing.
	for (int i = 0; i < num_processes; i++) {
		pcb_t *currentProcess = storage->processes[i];
		int waitTime = currentProcess->startTime - currentProcess->entryTime;
		int turnaroundTime = currentProcess->completionTime - currentProcess->entryTime;
		int deadline_met = turnaroundTime <= currentProcess->deadline ? 1 : 0;
		if (fprintf(outputFile, "%s %d %d %d\n", currentProcess->process_name, waitTime, turnaroundTime, deadline_met) < 0) {
			return 1;
		}
	}
	
	// return 1 if an error occurs while closing the file
	if (fclose(outputFile)) {
		return 1;
	}
	
	return 0;
}

/*
 * Function that simulates process scheduling based on the First-Come-First-Serve
 * scheduling algorithm on the processes in the given list of PCBs.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 * 
 * RETURN : No return value
 */

/*
 * Function that assigns PCBs to an array of PCBs based on their arrival times. Makes
 * a call to nextProcess() in order to retrieve the index of the next PCB to be placed
 * in the queue.
 *
 * ARGUMENTS : queue <- pointer to the pcb_store object that stores the PCBs like a queue
 *             storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 * 
 * RETURN : No return value
 */
void addToQueue(pcb_store *queue, pcb_store *storage, const int num_processes)
{
	// iterates num_processes times and in each iteration, assigns a PCB to its
	// correct slot in the queue.
 	for (int i = 0; i < num_processes; i++) {
 		int processId = nextProcess(storage, num_processes);
 		pcb_t *selectedProcess = storage->processes[processId];
 		queue->processes[i] = selectedProcess;
 		selectedProcess->state = EXIT;
 	}
 	
 	// change every PCB's state back to ready
 	for (int i = 0; i < num_processes; i++) {
 		queue->processes[i]->state = READY;
 	}
} 

/*
 * Function that determines the next process to run after the given process. It
 * traverses through the queue in a reverse circular order in accordance with the
 * round-robin algorithm.
 *
 * ARGUMENTS : queue <- pointer to the pcb_store object that stores the PCBs like a queue
 *             running <- int representing the index of the previous running process
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 * 
 * RETURN : processId <- an int corresponding to the index of queue containing
 *                       a process in the enterednready state, 50 otherwise.
 */
int nextInQueue(pcb_store *queue, const int running, const int num_processes)
{
	int processId = 50; // processId default value is  50 because storage only has max 10 elements
	int found = 0; // flag to be checked against to determine if there exists a process
	               // in the queue in the enterednready state.
	
	// check for the existence of a process in the enterednready state in the queue
	for (int i = 0; i < num_processes; i++) {
		if (queue->processes[i]->state == ENTEREDNREADY) {
			found = 1;
			break;
		}
	}
	
	// if one or more processes are in the enterednready state, traverse the queue in
	// reverse circular order and select the index of the first valid process.
	if (found) {
		for (int i = mod(running - 1, num_processes); i < num_processes; i = mod(i - 1, num_processes)) {
			if (queue->processes[i]->state == ENTEREDNREADY) {
				processId = i;
				return processId;
			}
		}
	} else {
		return processId;
	}
}

/*
 * Function that simulates First-Come-First-Serve process scheduling on the input
 * array of processes. It utilizes a while loop with each iteration representing
 * the passing of a second.
 *
 * ARGUMENTS : storage <- pointer to the pcb_store object that stores the PCBs
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 * 
 * RETURN : No return value
 */
void fcfs_scheduler(pcb_store *storage, const int num_processes)
{
	int ready_processes = num_processes;  // variable to store the number of processes currently in the ready state
	int running_processes = 0; // variable to store the number of currently running processes
	int current_time = 0; // variable to keep track of the time in seconds
	
	// variables to store the indices of the processes to be entered into the system
	// and the currently running process.
	int entryProcess, currentRunningProcess;
	
	// loop body executes as long as there are processes in the ready state. Each
	// iteration represents the time lapse of 1 second.
	while (ready_processes != 0) {
		sleep(1);
		
		// determines if there are any processes that arrive in the current time and
		// prints out an appropriate message if so along with changing the process's current state.
		entryProcess = processEntry(storage, num_processes, current_time);
		if (entryProcess != 50) {
			printf("Time %d: %s has entered the system.\n", current_time, storage->processes[entryProcess]->process_name);
			storage->processes[entryProcess]->state = ENTEREDNREADY;
		}
		
		// puts any process that has finished execution in the exit state and/or places the next
		// ready process into the running state if possible. Appropriate messages are
		// printed out for each action taken along with the updating of necessary attribute
		// values of the processes.
		if (running_processes) {
			if (storage->processes[currentRunningProcess]->remainingTime == 0) {
				printf("Time %d: %s has finished execution.\n", current_time, storage->processes[currentRunningProcess]->process_name);
				storage->processes[currentRunningProcess]->completionTime = current_time; 
				storage->processes[currentRunningProcess]->state = EXIT;
				ready_processes--;
				running_processes = 0;
				
				currentRunningProcess = nextProcess(storage, num_processes);
				if(handleRunningProcess1(storage, currentRunningProcess, current_time)) {
					running_processes = 1;
				}
				
			} else {
				storage->processes[currentRunningProcess]->remainingTime--;
			}
		} else {
			currentRunningProcess = nextProcess(storage, num_processes);
			if(handleRunningProcess1(storage, currentRunningProcess, current_time)) {
				running_processes = 1;
			}
		}
		
		// one second has passed.
		current_time++;
	}
	
	printf("Scheduling completed.\n");
}

/*
 * Function that simulates Round-Robin process scheduling on the input
 * array of processes. It utilizes a while loop with each iteration representing
 * the passing of a second.
 *
 * ARGUMENTS : queue <- pointer to the pcb_store object that stores the PCBs like a queue
 *             num_processes <- constant int that represents to total number of
 *                              processes in storage.
 * 
 * RETURN : No return value
 */
void rr_scheduler(pcb_store *queue, const int num_processes)
{
	int ready_processes = num_processes;  // variable to store the number of processes currently in the ready state
	int running_processes = 0; // variable to store the number of currently running processes
	int current_time = 0; // variable to keep track of the time in seconds
	int time_quantum_flag = 0;
	
	// variables to store the indices of the processes to be entered into the system
	// and the currently running process.
	int entryProcess, currentRunningProcess = 1;
	
	while (ready_processes != 0) {
		sleep(1);
		
		// determines if there are any processes that arrive in the current time and
		// prints out an appropriate message if so along with changing the process's current state.
		entryProcess = processEntry(queue, num_processes, current_time);
		if (entryProcess != 50) {
			printf("Time %d: %s has entered the system.\n", current_time, queue->processes[entryProcess]->process_name);
			queue->processes[entryProcess]->state = ENTEREDNREADY;
		}
		
		if (running_processes) { 
			if (current_time % 2 == time_quantum_flag) { // time at which the currently running process should be switched out
				if (queue->processes[currentRunningProcess]->remainingTime == 0) { // handling finished processes
					printf("Time %d: %s has finished execution.\n", current_time, queue->processes[currentRunningProcess]->process_name);
					queue->processes[currentRunningProcess]->completionTime = current_time; 
					queue->processes[currentRunningProcess]->state = EXIT;
					ready_processes--;
					running_processes = 0;
					
					// start running the next process in the queue
					currentRunningProcess = nextInQueue(queue, currentRunningProcess, num_processes);
					if (handleRunningProcess2(queue, currentRunningProcess, current_time)) {
						running_processes = 1;
					}
				} else { // place the current process on enterednready state and run the next process
					queue->processes[currentRunningProcess]->state = ENTEREDNREADY;
					currentRunningProcess = nextInQueue(queue, currentRunningProcess, num_processes);
					if (handleRunningProcess2(queue, currentRunningProcess, current_time)) {
						running_processes = 1;
					}
				}
			} else if (queue->processes[currentRunningProcess]->remainingTime == 0) { // handle processes that finished outside the allocated time quantum
				printf("Time %d: %s has finished execution.\n", current_time, queue->processes[currentRunningProcess]->process_name);
				queue->processes[currentRunningProcess]->completionTime = current_time; 
				queue->processes[currentRunningProcess]->state = EXIT;
				ready_processes--;
				running_processes = 0;
				
				// start running the next process in the queue
				currentRunningProcess = nextInQueue(queue, currentRunningProcess, num_processes);
				if (handleRunningProcess2(queue, currentRunningProcess, current_time)) {
					running_processes = 1;
					time_quantum_flag = time_quantum_flag == 1 ? 0 : 1; 
				}
			} else { // maintain the current running process
				queue->processes[currentRunningProcess]->remainingTime--;
			}
		} else { // initialize the scheduler and place the first process into the running state
			if (current_time % 2 == time_quantum_flag) {
				currentRunningProcess = nextInQueue(queue, currentRunningProcess, num_processes);
				if (handleRunningProcess2(queue, currentRunningProcess, current_time)) {
					running_processes = 1;
				}
			}
		}
		
		// one second has passed
		current_time++;
	}
	
	printf("Scheduling completed.\n");
}

void pbs_scheduler(pcb_store *storage, const int num_processes)
{
	int ready_processes = num_processes;  // variable to store the number of processes currently in the ready state
	int running_processes = 0; // variable to store the number of currently running processes
	int current_time = 0; // variable to keep track of the time in seconds
	
	// variables to store the indices of the processes to be entered into the system
	// and the currently running process.
	int entryProcess, currentRunningProcess;
	
	// loop body executes as long as there are processes in the ready state. Each
	// iteration represents the time lapse of 1 second.
	while (ready_processes != 0) {
		sleep(1);
		
		// determines if there are any processes that arrive in the current time and
		// prints out an appropriate message if so along with changing the process's current state.
		entryProcess = processEntry(storage, num_processes, current_time);
		if (entryProcess != 50) {
			printf("Time %d: %s has entered the system.\n", current_time, storage->processes[entryProcess]->process_name);
			storage->processes[entryProcess]->state = ENTEREDNREADY;
		}
		
		// puts any process that has finished execution in the exit state and/or places the next
		// ready process into the running state if possible. Appropriate messages are
		// printed out for each action taken along with the updating of necessary attribute
		// values of the processes.
		if (running_processes) {
			if (storage->processes[currentRunningProcess]->remainingTime == 0) {
				printf("Time %d: %s has finished execution.\n", current_time, storage->processes[currentRunningProcess]->process_name);
				storage->processes[currentRunningProcess]->completionTime = current_time; 
				storage->processes[currentRunningProcess]->state = EXIT;
				ready_processes--;
				running_processes = 0;
				
				currentRunningProcess = nextPriorityProcess(storage, num_processes);
				if(handleRunningProcess1(storage, currentRunningProcess, current_time)) {
					running_processes = 1;
				}
				
			} else {
				storage->processes[currentRunningProcess]->remainingTime--;
			}
		} else {
			currentRunningProcess = nextProcess(storage, num_processes);
			if(handleRunningProcess1(storage, currentRunningProcess, current_time)) {
				running_processes = 1;
			}
		}
		
		// one second has passed.
		current_time++;
	}
	
	printf("Scheduling completed.\n");
}
