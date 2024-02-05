#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

#include	"unp.h"

#define TIME_INTERVAL  1 // sec

/*  Targets: 
    1. Detect Run time error (Core Dumped)
    2. Write the result to LOG file
*/

/*  Have Done: 
    1. C & C++ langauges
    2. AC, WA, TLE, segmentation fault
    3. ASCII art (Clients' Terminal)
    4. Score & Scoreboard
    5. Multiple users (user id)
    6. Unlimited testing times
    7. 5 quetions with answers
    8. Security --> caesar cipher & atbash cipher
*/

void atbash_decrpyt(char* input){
    for(int i = 0; i < strlen(input); i++){
        if(input[i] <= 'Z' && input[i] >= 'A'){
            input[i] = 'Z' - input[i] + 'A';
        }else if(input[i] <= 'z' && input[i] >= 'a'){
            input[i] = 'z' - input[i] + 'a';
        }
    }
}

void caesar_decrpyt(char* input){
    char key = 'c';
    for(int i = 0; i < strlen(input); i++){
        input[i]-= key; 
    }
}

void x_run(int sockfd, char* ID) {
    
    char file_name[100], file_out[100], file_output[100], command[500];

    while(1){

        pid_t pid;
        ssize_t		n;
        char rcvline[MAXLINE * 10], quesions_num[MAXLINE];
        Readline(sockfd,quesions_num, MAXLINE);

        /* print your ID name */
        printf("ID: %s\t", ID);


        /* which questions students want to answer */
        printf("Problem number: %c\t",quesions_num[0]);

        char no_file[MAXLINE], type[MAXLINE];
        Read(sockfd,no_file,sizeof(no_file));
        if(!strcmp(no_file,"oof\n")){
            exit(0);
        }bzero(no_file,MAXLINE);

        Read(sockfd, type, sizeof(type));
        
        if(strcmp(type, "C\n") == 0){
            sprintf(file_name, "%s.c", ID);
            sprintf(file_out, "%s.out", ID);
            sprintf(file_output, "%s.txt", ID);

        }else if(strcmp(type, "C++\n")== 0){
            sprintf(file_name, "%s.cpp", ID);
            sprintf(file_out, "%s.out", ID);
            sprintf(file_output, "%s.txt", ID);
        }


        /* Source Code File */ 
        FILE* compile_file = fopen(file_name, "w"); 

        char encode_typee[20];
        n = Read(sockfd, encode_typee, sizeof(encode_typee));

        // printf("encode %s", encode_typee);

        bzero(rcvline, sizeof(rcvline));
        n = Read(sockfd, rcvline, sizeof(rcvline));
        
        /* which kind of encryption */
        if(strcmp(encode_typee, "1\n") == 0){
            caesar_decrpyt(rcvline); 
        }else{
            atbash_decrpyt(rcvline);
        }

        rcvline[n] = '\n';

        /* Check whether the source code receive correctly */
        // printf("Recvline: \n %s\n",rcvline);

        fprintf(compile_file, "%s", rcvline);
        fclose(compile_file);

        /* Fork the first child process to compile the code */ 
        if ( (pid = Fork()) == 0) {
            if(strcmp(type, "C\n") == 0){
                if((execlp("gcc", "gcc", file_name, "-o", file_out, NULL) < 0)){
                    perror("Error in execlp");
                    fflush(stdout);
                    exit(0);
                }
            }else{
                if((execlp("g++", "g++", file_name, "-o", file_out, NULL) < 0)){
                    perror("Error in execlp");
                    fflush(stdout);
                    exit(0);
                }
            }
        } else if (pid < 0) {
            perror("Error forking");
            fflush(stdout);
            exit(0);
        }
        
        wait(NULL);
        
        /* Sent segmentation fault error to the client. If there is no user.out -> compile error */
        FILE *file = fopen(file_out, "r");
        if(file == NULL){
            fflush(stdout);
            if ( (pid = Fork()) == 0)  {
                printf("\033[0;31mSegmentation Fault\n\033[0;31m");
                Writen(sockfd, "Segmentation Fault\n", sizeof("Segmentation Fault\n")); 
                bzero(command, sizeof(command));
                sprintf(command, "rm %s", file_name);
                execlp("sh", "sh", "-c", command, NULL);
            }
            fflush(stdout);
            wait(NULL);
            printf("\033[0m");
            sleep(0.5);
            Writen(sockfd, "0.0000\n", sizeof("0.0000\n"));

            /* close the child process or not */
            char conti[MAXLINE];
            bzero(conti, sizeof(conti));
            Read(sockfd, conti, MAXLINE);
            if(!strcmp(conti,"break\n")){
                // printf("Quitting......\n");
                exit(0);
            }else{
                continue;
            }
        }            
    
        /* Calculate the time for TLE */
        struct timeval start, end; 
        gettimeofday(&start, 0);

        /* input file */ 
        char inpfile[256];
        sprintf(inpfile,"./test/testcase/input%c.txt",quesions_num[0]);

        /* Fork the second child process to execute the code */ 
        if ( (pid = Fork()) == 0)  {
            bzero(command, sizeof(command));
            sprintf(command, "./%s < %s > %s", file_out, inpfile , file_output);
            if((execlp("sh", "sh", "-c", command, NULL)) < 0){
                perror("Error in execlp");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("Error forking");
            exit(EXIT_FAILURE);
        }

        wait(NULL);

        /* Calculate the time */
        gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        double time = sec + (usec/1000000.0);
        double pre_score = time*1000000.0;


        /* Read and print the content of the output file */
        FILE* output_file = fopen(file_output, "r");
        char ansfile[256];
        sprintf(ansfile,"./test/answer/answer%c.txt",quesions_num[0]);
        FILE *answer_file = fopen(ansfile, "r");

        if (output_file == NULL || answer_file == NULL) {
            perror("Error opening files for reading");
            exit(EXIT_FAILURE);
        }

        char *output = NULL, *answer = NULL;
        size_t output_sz = 0, answer_sz = 0; 
        int wrong = 0, quetions = 0;
        double score = 0;

        while((getline(&output, &output_sz, output_file) != EOF) && (getline(&answer, &answer_sz, answer_file)) != EOF) {
            if(strcmp(output, answer) != 0){
                wrong++;
            }
            quetions++;
        }

        /* Calculate the score based on runtime. */
        if(wrong == 0){
            score = (1000000 - pre_score) / 10000;
        }else{
            double ratio = (double)(wrong) / (double)(quetions);
            printf("wrong:%d questions:%d ratio:%f\n", wrong, quetions, ratio);
            score = 100 * (1 - ratio);
        }

        fclose(output_file);
        fclose(answer_file);


        if(time > TIME_INTERVAL){
            printf("\033[0;31mTLE\t\033[0;31m");
            Writen(sockfd, "TLE\n", sizeof("TLE\n"));  
            score = 0.0;  

        } else if(wrong == 0){
            printf("\033[0;32mAC\t\033[0;32m");
            Writen(sockfd, "AC\n", sizeof("AC\n"));
        }
        else{
            printf("\033[0;31mWA\t\033[0;31m");
            Writen(sockfd, "WA\n", sizeof("WA\n"));
        }
        printf("\033[0m");

        sleep(0.5);

        /* Send how many points you got */
        char points[50];
        sprintf(points, "Score: %.4f\n", score);
        printf("%s", points);
        Writen(sockfd, points, sizeof(points));

        /* Remove all the file that generate during compile */
        if ( (pid = Fork()) == 0)  {
            bzero(command, sizeof(command));
            sprintf(command, "rm %s %s %s", file_name, file_out, file_output);
            execlp("sh", "sh", "-c", command, NULL);
        } else if (pid < 0) {
            perror("Error forking");
            exit(EXIT_FAILURE);
        }

        wait(NULL); 

        /* close the child process or not */
        char conti[MAXLINE];
        bzero(conti, sizeof(conti));
        Read(sockfd, conti, MAXLINE);
        if(!strcmp(conti,"break\n")){
            // printf("Quitting......\n");
            exit(0);
        }

    }

}

int main(int argc, char **argv)  {

	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT+3);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
    
    printf("ScoreBoard:\n"); 

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        int n;
        char ID[50];
        n = Read(connfd, ID, sizeof(ID));
        ID[n] = '\0';

		if ( (childpid = Fork()) == 0) {	
			Close(listenfd);	
			x_run(connfd, ID);	
			exit(0);
		}
		Close(connfd);		
	}
}
