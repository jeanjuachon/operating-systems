#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
//#define GetCurrentDir _getcwd
//#define GetCurrentDir getcwd
#define COMMAND_SIZE 255
#ifdef WINDOWS
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

char msg[COMMAND_SIZE];
const int delayCount = 1e6;
typedef enum { false = 0, true } bool;

typedef enum{
    LEFT, RIGHT, LEFT_CIRCULAR, RIGHT_CIRCULAR
}Direction;

const int shiftLen = 1;
//Creating the marquee requirements (this is just a trial to see the error)
/*
void* marquee(void*a){
    
    printf("%s", a+8);
    return NULL;
}
*/


double factorial(unsigned n) {
    if (n == 1) return n;
    return n * factorial(n-1);
}

void delay(int n) {
    int i;
    for (i = 0; i < n; i++) {
        factorial(10);
    }
}

bool isCircularShift(Direction dir) {
    return (dir == LEFT_CIRCULAR) || (dir == RIGHT_CIRCULAR);
}

void shift(char *a, size_t distance, Direction dir) {
    char *source, *dest, *bumpFrom, *rotateTo, temp;
    size_t n, aLen = strlen(a);

    if (distance > 0) {
        if ((dir == RIGHT) || (dir == RIGHT_CIRCULAR)) {
            source = a; dest = a+1;
            bumpFrom = &a[aLen-1]; rotateTo = &a[0];
        } else {
            source = a+1; dest = a;
            bumpFrom = &a[0]; rotateTo = &a[aLen-1];
        }
        for (n = 0; n < distance; n++) {
            temp = *bumpFrom;
            memmove(dest,source,aLen-1);
            if (isCircularShift(dir) == true) {
                *rotateTo = temp;
            } else {
                *rotateTo = ' ';
            }
        }
    }
}


void *marquee(void *s0){
    size_t len = strlen(s0)*10, i ,j;
    char *s;
    //Direction RIGHT_CIRCULAR;

    if(len > 0){
        s = calloc(len+1, sizeof(char));
        memset(s, ' ', len);
        memcpy(s, s0, strlen(s0));

        //scrolling
        for(i = 0; i < 5; i++){
            for(j = 0; j < len; j++){
                //*(strchr(s0,'\n')) = '\0';
                printf("\r%s",s0 +8);
                fflush(stdout);
                shift(s, shiftLen, RIGHT);
                delay(delayCount);
            }
        }
    }
    return NULL;
}



int main(int argc, char* argv[]){
    
    char command[COMMAND_SIZE] = {0};
    time_t t;
    time(&t);
    char pwd[FILENAME_MAX];
 	GetCurrentDir(pwd, FILENAME_MAX);
    pthread_t thread1;

    do{
         #ifdef _WIN32
    	printf("Windows 32-bit OS>");

    	#elif _WIN64
   	 	printf("Windows 64-bit OS>");

    	#elif __APPLE__
    	printf("Mac OS>");

    	#elif __linux__

    	printf("Linux OS>");
    	#endif
        fflush(stdout);
        fgets(command,sizeof(command), stdin);
        

        if(strncmp(command, "say", 3) == 0) {
            printf("%s", command+4);
        }else if(strncmp(command, "cls", 3) == 0){
        	system("clear");
        }else if(strncmp(command, "time", 4) == 0){
        	printf("%s", ctime(&t));
        }else if(strncmp(command, "pwd", 3) == 0){
        	printf("%s\n", pwd);
        }
        else if(strncmp(command, "marquee", 6) == 0){
             pthread_t thread1;
             //*(strchr(s0,'\n')) = '\0';
             pthread_create(&thread1,NULL,marquee,&command); //threadid, NULL, void pointer function, input on function
             //pthread_join(thread1, NULL);      
        }	
        else if(strncmp(command, "dir",3) ==0){
	        DIR *mydir;
		    struct dirent *myfile;
		    struct stat mystat;

		    char buf[512];
            time_t t = mystat.st_ctime;
            struct tm lt;
            localtime_r(&t, &lt);
            char timbuf[80];
            strftime(timbuf, sizeof(timbuf), "%c", &lt);


		    mydir = opendir(".");
		    while((myfile = readdir(mydir)) != NULL)
		    {
		        sprintf(buf, "%s", myfile->d_name);
		        stat(buf, &mystat);
		        printf("File Name: %s\n\n", myfile->d_name);
		        printf("File size: %lld bytes\n",mystat.st_size);
		        printf("Created on: %s\t\n",timbuf);
		    }
		    closedir(mydir);		
			}
		else{
			printf("Unknown command, please input a valid command\n");
		}
    } while (strncmp(command, "exit", 4));
    return 0;
}
