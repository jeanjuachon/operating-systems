//
//  Project2Main.c
//  
//
//  Created by Jean Philip Juachon on 1/18/21.
//

//#include "Project2Main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 256

//Create 2 structures
//XYZ: X = ALGO; Y = NUM OF PROCESSES; Z = FOR RR
//ABC: A = PID; B = ARRIVAL TIME; C = EXEC TIME
struct XYZ {
	int algo, numProcesses, timeSlice;
};

struct ABC {
	int pid, arrivalTime, execTime, endTime, waitTime, startTime, execRemaining;
};


int main(){
	/*
	ALGO DEFINER:
	0 = FCFS ; 1 = NON PREEMPTIVE; 2 = PREEMPTIVE ; 3 = RR
	*/
	char filename[255];
	struct XYZ scheduler;
	struct ABC process[255], temp, ready, idle;
	int i, j, k;
	float average = 0.0, totalSum = 0.0;
	FILE *filepointer;
	int num = 0; //initialize to 0, if not, num = num+1 will start with 1
	int divisor;
	printf("Enter the name/path of the file: ");
	scanf("%s", filename);

	//If filename does not exist
	filepointer = fopen(filename, "r");
	if(filepointer == NULL){
		printf("Filename %s does not exist\n", filename);
		exit(1);
	}

	//If filename exists, continue
	//Checking of file contents first
	char buffer[LEN];
	i = 0;
	//printf("File contents: \n");
	//scan the file
	while(fgets(buffer, LEN - 1, filepointer) != NULL){
		sscanf(buffer, "%d %d %d", &process[i].pid, &process[i].arrivalTime, &process[i].execTime);
		//printf("%d %d %d\n", process[i].pid, process[i].arrivalTime, process[i].execTime);
		i = i+1;
		num = num+1; //store the number of lines, testfile is 10
	}

	//Store the first line as the XYZ structure
	scheduler.algo = process[0].pid;
	scheduler.numProcesses = process[0].arrivalTime;
	scheduler.timeSlice = process[0].execTime;

	/*
	printf("Arrangement\n");
	for(i = 0; i < num; i++){
		printf("%d %d %d\n", process[i].pid, process[i].arrivalTime, process[i].execTime);
	}
	*/


	//--------FCFS---------
	if(scheduler.algo == 0){
			//Sort the arrival time of each process
		for(i = 1; i < num; i++){
			for(j = 1; j < num-i; j++){
				if(process[j].arrivalTime > process[j+1].arrivalTime){
					temp = process[j];
					process[j] = process[j+1];
					process[j+1] = temp;
				}
			}
		}
	

		for(i = 1; i < num; i++){
			if(i == 1){
				process[i].startTime = process[i].arrivalTime;
				process[i].waitTime = process[i].startTime;
				process[i].endTime = process[i].arrivalTime + process[i].execTime;
			}else{
				process[i].startTime = process[i-1].endTime;
				process[i].endTime = process[i-1].endTime + process[i].execTime;
				process[i].waitTime = process[i-1].endTime - process[i].arrivalTime; 
			}
		}

		totalSum = 0; 		
		for (i = 1; i < num; i++) {
				printf("P[%d] Start Time: %d End Time: %d | Waiting time: %d\n", process[i].pid, process[i].startTime, process[i].endTime, process[i].waitTime); 
				totalSum += process[i].waitTime;  	
		}
		divisor = num-1; 
		average = totalSum/(float)divisor; 
		printf("Average waiting time: %f\n",average); 
	}//NON PREEMPTIVE SCHEDULING
	else if(scheduler.algo == 1){
		//Sort it by arrival time first because you need to give the cpu to the one on ready state first (process 0)
		for(i = 1; i < num; i++){
			for(j = 1; j < num-i; j++){
				if(process[j].arrivalTime > process[j+1].arrivalTime){
					temp = process[j];
					process[j] = process[j+1];
					process[j+1] = temp;
				}
			}
		}
		//Sort it now by their execution/burst time
		for (i = 2; i <num; i++ ){
			for (j = 2; j < num - i; j++){
				if (process[j].execTime > process[j+1].execTime) {
					temp = process[j]; 
					process[j] = process[j+1]; 
					process[j+1] = temp; 
				} else if (process[j].execTime == process[j+1].execTime){
					if (process[j].arrivalTime > process[j+1].arrivalTime){
					temp = process[j]; 
					process[j] = process[j+1]; 
					process[j+1] = temp; 
					}	
				}
	
			}
		}

		for(i = 1; i < num; i++){
			if(i == 1){
				process[i].startTime = process[i].arrivalTime;
				process[i].waitTime = process[i].startTime;
				process[i].endTime = process[i].arrivalTime + process[i].execTime;
			}else{
				process[i].startTime = process[i-1].endTime;
				process[i].endTime = process[i-1].endTime + process[i].execTime;
				process[i].waitTime = process[i-1].endTime - process[i].arrivalTime; 
			}
		}

		totalSum = 0; 		
		for (i = 1; i < num; i++) {
				printf("P[%d] Start Time: %d End Time: %d | Waiting time: %d\n", process[i].pid, process[i].startTime, process[i].endTime, process[i].waitTime); 
				totalSum += process[i].waitTime;  	
		}
		divisor = num-1; 
		average = totalSum/(float)divisor; 
		printf("Average waiting time: %f\n",average);
	}//FOR PREEMPTIVE SJF
	else if(scheduler.algo == 2){
		//Sort it by arrival time first because you need to give the cpu to the one on ready state first (process 0)
		int count = 0;
		//for()
		//After sorting the arrival time, we need to determine if the execution time of i is greater than the execution time of i+1
		//If it is greater than the execution time of i+1, we then run that row then store the value remaining of i in execRemaining
		for(i = 1; i < num; i++){
			for(j = 1; j < num-i; j++){
				if(process[j].arrivalTime > process[j+1].arrivalTime){
					temp = process[j];
					process[j] = process[j+1];
					process[j+1] = temp;
				}
			}
		}
		//Sort it now by their execution/burst time
		for (i = 2; i <num; i++ ){
			for (j = 2; j < num - i; j++){
				if (process[j].execTime > process[j+1].execTime) {
					temp = process[j]; 
					process[j] = process[j+1]; 
					process[j+1] = temp; 
				} else if (process[j].execTime == process[j+1].execTime){
					if (process[j].arrivalTime > process[j+1].arrivalTime){
					temp = process[j]; 
					process[j] = process[j+1]; 
					process[j+1] = temp; 
					}	
				}
	
			}
		}

		for(i = 1; i < num; i++){
			if(i == 1){
				process[i].startTime = process[i].arrivalTime;
				process[i].waitTime = process[i].startTime;
				process[i].endTime = process[i].arrivalTime + process[i].execTime;
			}else{
				process[i].startTime = process[i-1].endTime;
				process[i].endTime = process[i-1].endTime + process[i].execTime;
				process[i].waitTime = process[i-1].endTime - process[i].arrivalTime; 
			}
		}

		totalSum = 0; 		
		for (i = 1; i < num; i++) {
				printf("P[%d] Start Time: %d End Time: %d | Waiting time: %d\n", process[i].pid, process[i].startTime, process[i].endTime, process[i].waitTime); 
				totalSum += process[i].waitTime;  	
		}
		divisor = num-1; 
		average = totalSum/(float)divisor; 
		printf("Average waiting time: %f\n",average);

		/*
		totalSum = 0; 		
		for (i = 1; i < num; i++) {
				printf("P[%d] Start Time: %d End Time: %d | Waiting time: %d\n", process[i].pid, process[i].startTime, process[i].endTime, process[i].waitTime); 
				totalSum += process[i].waitTime;  	
		}
		divisor = num-1; 
		average = totalSum/(float)divisor; 
		printf("Average waiting time: %f\n",average);
		*/
	
	}
	//FOR ROUND ROBIN
	else if(scheduler.algo == 3){
		//Sort the process by arrival time
		for(i = 1; i < num; i++){
			for(j = 1; j < num-i; j++){
				if(process[j].arrivalTime > process[j+1].arrivalTime){
					temp = process[j];
					process[j] = process[j+1];
					process[j+1] = temp;
				}
			}
		}

		int remainingProcess = num-1;
		while(remainingProcess != 0){
			for(i = 1; i < num; i++){
				if(i == 1){
					process[i].startTime = process[i].arrivalTime;
					if(process[i].execTime <= scheduler.timeSlice){
						process[i].waitTime = process[i].arrivalTime;
						process[i].endTime = process[i].execTime;
						remainingProcess--;
					}else{
						process[i].execRemaining = scheduler.timeSlice - process[i].execTime;
						process[i].waitTime = scheduler.timeSlice;
						process[i].endTime = scheduler.timeSlice;
					}
				}else{
					process[i].startTime = process[i-1].endTime;
					if(process[i].execTime <= scheduler.timeSlice){
						process[i].waitTime = process[i].arrivalTime;
						process[i].endTime = process[i].execTime;
						remainingProcess--;
					}else{
						process[i].execRemaining = scheduler.timeSlice - process[i].execTime;
						process[i].waitTime = process[i].startTime - process[i].arrivalTime;
						process[i].endTime = process[i].startTime + scheduler.timeSlice;
					}
				}
			}
		}

		totalSum = 0; 		
		for (i = 1; i < num; i++) {
				printf("P[%d] Start Time: %d End Time: %d | Waiting time: %d\n", process[i].pid, process[i].startTime, process[i].endTime, process[i].waitTime); 
				totalSum += process[i].waitTime;  	
		}
		divisor = num-1; 
		average = totalSum/(float)divisor; 
		printf("Average waiting time: %f\n",average);



	}





	return 0;

}
