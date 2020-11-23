/*
 * NAME : BRYAN HOOI YU ERN
 * STUDENT ID : 30221005
 * START DATE : 24/10/2020
 * DATE LAST MODIFIED : 25/10/2020
 * DESCRIPTION : Header file that contains the declarations of multiple structs
 *               and functions that will be required by the main c files.
 */

/* Special enumerated data type for process state */
typedef enum {
	READY, ENTERED, ENTEREDNREADY, RUNNING, EXIT
} process_state_t;

/* C data structure used as process control block. The scheduler should create
 * one instance per running process in the system.
 */
typedef struct pcb_t{
	char process_name[11];    // string that identifies the process
	
	/* Times measured in seconds... */
	int entryTime;      // time process entered the system
	int serviceTime;    // the total CPU time required by the process
	int remainingTime;  // remaining service time until completion
	int deadline;       // expected maximum time from the arrival to the completion of the process.
	int completionTime; // time at which process is completed
	int startTime;      // time at which the process begins to be serviced
	
	process_state_t state; // current process state
} pcb_t;

/*
 * C data structure used as array of process control blocks. This array can only
 * fit a maximum of 10 processes as per assignment specifications
 */
typedef struct pcb_store {
	pcb_t *processes[10]; // array of pointers to PCBs
} pcb_store;

int mod(int a, int b);

int processData(pcb_store *storage, char *buffer, int max_buf_size, FILE *file);

void submitProcess(pcb_store *storage, pcb_t *newProcess, int index, char *name, int arrival, int service, int deadline);

int nextProcess(pcb_store *storage, const int num_processes);

int nextPriorityProcess(pcb_store *storage, const int num_processes);

int processEntry(pcb_store *storage, const int num_processes, const int time);

int handleRunningProcess1(pcb_store *storage, const int processId, const int time);

int handleRunningProcess2(pcb_store *queue, const int processId, const int time);

int writeResults(pcb_store *storage, const int num_processes);

void addToQueue(pcb_store *queue, pcb_store *storage, const int num_processes);

int nextInQueue(pcb_store *queue, const int running, const int num_processes);

void fcfs_scheduler(pcb_store *storage, const int num_processes);

void rr_scheduler(pcb_store *queue, const int num_processes);

void pbs_scheduler(pcb_store *storage, const int num_processes);
