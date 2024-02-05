#include	"unp.h"
#include  <string.h>
#include  <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
#include <time.h>

char source_file_name[MAXLINE];


void ph(){
    printf(" ██████╗ ███╗   ██╗██╗     ██╗███╗   ██╗███████╗         ██╗██╗   ██╗██████╗  ██████╗ ███████╗\n");
    printf("██╔═══██╗████╗  ██║██║     ██║████╗  ██║██╔════╝         ██║██║   ██║██╔══██╗██╔════╝ ██╔════╝\n");
    printf("██║   ██║██╔██╗ ██║██║     ██║██╔██╗ ██║█████╗           ██║██║   ██║██║  ██║██║  ███╗█████╗  \n");
    printf("██║   ██║██║╚██╗██║██║     ██║██║╚██╗██║██╔══╝      ██   ██║██║   ██║██║  ██║██║   ██║██╔══╝\n");  
    printf("╚██████╔╝██║ ╚████║███████╗██║██║ ╚████║███████╗    ╚█████╔╝╚██████╔╝██████╔╝╚██████╔╝███████╗\n");
    printf(" ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝     ╚════╝  ╚═════╝ ╚═════╝  ╚═════╝ ╚══════╝\n");
}


void clear_screen(){
    printf("\x1B[2J");
};

void set_screen(){
    printf("\x1B[=3h");
};

void atbash_encrpyt(char* input, char* output){
    for(int i = 0; i < strlen(input); i++){
        if(input[i] <= 'Z' && input[i] >= 'A'){
            output[i] = 'Z' - input[i] + 'A';
        }else if(input[i] <= 'z' && input[i] >= 'a'){
            output[i] = 'z' - input[i] + 'a';
        }else{
            output[i] = input[i];
        }
    }
}

void caesar_encrpyt(char* input, char* output){
    char key = 'c';
    for(int i = 0; i < strlen(input); i++){
        output[i] = input[i] + key; 
    }
}

void x_run(int sockfd)
{
    /* input your user ID */
    int n; 
    char uid[50];
    printf("Please input your ID: ");
    n = scanf("%s",uid);
    Writen(sockfd, uid, strlen(uid));

    while(1){
        srand(time(NULL));

        /* which problem to test */
        char class[MAXLINE];
        printf("Please choose what problems you want to test: ");
        n = scanf("%s",class);
        class[1] = '\n';
        writen(sockfd,class,strlen(class));

        set_screen();
        clear_screen();
        ph();

        char question[MAXLINE*10];
        char quefile[256];
        sprintf(quefile,"./test/question/q%c.txt",class[0]);
        FILE* que_file = fopen(quefile,"r");

        char *line0 = NULL;
        size_t len0 = 0;
        while (getline(&line0, &len0, que_file) != EOF) {
            strcat(question, line0);
            bzero(line0, len0);
            len0 = 0;
        }
        printf("%s",question);

        bzero(question,sizeof(question));

        /* input source file name */
        printf("Please input your source file name: ");
        n = scanf("%s",source_file_name);


        /* Check if there is source file */
        char tmp[MAXLINE];
        char submit[MAXLINE*2];
        sprintf(submit,"./submit/%s",source_file_name);
        FILE* compile_file = fopen(submit, "r");
        if (compile_file == NULL) {
            strcpy(tmp, "oof\n");
            writen(sockfd,tmp ,strlen(tmp));
            perror("Error opening file");
        }

        strcpy(tmp, "normal\n");
        writen(sockfd, tmp, strlen(tmp));
        sleep(0.5);

        char code[MAXLINE * 10] = {'\0'}, codee[MAXLINE * 10] = {'\0'}, recvline[MAXLINE], type[20];
        char *line = NULL;
        size_t len = 0;

        /* offer 2 kinds of languages to compile */
        if(source_file_name[strlen(source_file_name)-1] == 'c'){
            sprintf(type, "C\n");
        }else{
            sprintf(type, "C++\n");
        }

        writen(sockfd, type, sizeof(type));
        sleep(0.5);        

        /* Read the source code file */
        while (getline(&line, &len, compile_file) != EOF) {
            strcat(codee, line);
            bzero(line, len);
            len = 0;
        }

        /* Caesar cipher */
        int encode_type = rand();
        encode_type %= 2; 

        char encode_typee[20];
        sprintf(encode_typee, "%d\n", encode_type);
        writen(sockfd, encode_typee, sizeof(encode_typee));

        // printf("encode type: %s", encode_typee);
        
        if(encode_type == 1){
            caesar_encrpyt(codee, code);
        }else{
            atbash_encrpyt(codee, code);
        }

        sleep(0.5);
        
        /* Check the source code is correct*/
        printf("code:\n %s\n",codee);
        /* Check the encrypted source code is correct*/
        printf("encrypt code:\n %s\n",code);

        /* Send the source code to Online Judge */
        writen(sockfd, code, sizeof(code));

        sleep(0.5);

        /* Result */
        Read(sockfd, recvline, sizeof(recvline));
        printf("(result) user %s question %c:\t", uid, class[0]);

        if(strcmp(recvline, "AC\n") == 0){
            printf("\033[0;32m%s\033[0;32m", recvline); 
            printf("\033[0;32m");
            printf("   .--.\n");
            printf("  |o_o |\n");
            printf("  |:_/ |\n");
            printf(" //   \\ \\\n");
            printf("(|     |)\n");
            printf("/'\\_   _/`\\\n");
            printf("\\___)=(___/\n");
            printf("\033[0m");

        }else if(strcmp(recvline, "WA\n") == 0){
            printf("\033[0;31m%s\033[0;31m", recvline); 
            printf(" /\\_/\\ \n");
            printf("( o.o )\n");
            printf(" > ^ < \n");
            printf("\033[0m");

        }else{
            printf("\033[0;33m%s\033[0;33m", recvline); 
            printf("  / \\__\n");
            printf(" (    @\\___\n");
            printf(" /         O\n");
            printf("/   (_____/\n");
            printf("\033[0m");
        }

        char points[50]; 
        n = Read(sockfd, points, sizeof(points));
        printf("%s", points);

        printf("Have another file to test? 1 for yes, 0 for no: ");
        int conti;
        scanf("%d",&conti);

        if(n == 0 || !conti ){
            char cont[] = {"break\n"};
            writen(sockfd, cont, strlen(cont));
            break;
        }else{
            char cont[] = {"continue\n"};
            writen(sockfd, cont, strlen(cont));
        }
        
        set_screen();
        clear_screen();
    } 

};

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress> <ID>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT+3);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	//strcpy(source_file_name, argv[2]);


	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	x_run(sockfd);		/* do it all */

	exit(0);
}
