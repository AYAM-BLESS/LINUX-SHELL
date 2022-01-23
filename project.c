/*
This project consists of designing a C program to serve as a shell interface that accepts user commands and then executes each command in a separate process. A shell interface gives the user a prompt, after which the next command is entered.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#define MAX_LINE 80  
// Function to Run and Execute Commands
void run(char *args[], char *val[]){
pid_t pid;
int run_background = 0;
if (strcmp(val[0], "&") == 0) {
run_background = 1;
}
pid=fork();// Forking of a Process
int status;
if(pid<0){
printf("Error in creating child process\n");
}
if(pid==0){
printf("This is the child\n");
status=execvp(args[0], args);//Execution of child process
if(status==-1){
printf("Error in execution\n");
}
}
else
{
if(run_background){ // Parent process running in background 
printf("Parent process running in background\n");
}
else{
wait(&status);// Parent process waiting for child process to be executed
}
}}
// Function for Input Redirection
void redirectIn(char *fileName)
{
int in = open(fileName, O_RDONLY);
dup2(in, 0);
close(in);
}// Function for Output Redirection
void redirectOut(char *fileName)
{
int out = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0600);
dup2(out, 1);
close(out);
} 
// Function to create Pipe
void createPipe(char *args[], char *val[])
{
int fd[2];
pipe(fd);

dup2(fd[1], 1);
close(fd[1]);

printf("args = %s\n", *args);
run(args, val);
dup2(fd[0], 0);
close(fd[0]);
}

// Function to split commands
void splitcmd(char *cmd, char *args[], char *val[]) {
char *str;
int i=0;
str=strtok(cmd, " \n");// string token
while(str != NULL){
if(*str=='&'){ //if the token is &, then the token is stored in val
val[0]=str;
}
else if (*str == '<') { //Checks if the token is '<'
redirectIn(strtok(NULL, "<"));
}
else if (*str == '>') { // Checks if the token is '>'
redirectOut(strtok(NULL, ">"));
}
else if (*str == '|') { // Checks if the token is '|'
args[i] = NULL;
createPipe(args, val);
i = 0;
}
else
{
printf("token is %s\n", str);
args[i]=str;
val[0]="lol";
i++;
}
str=strtok(NULL, " \n");
}
args[i]=NULL;
}

int main(){
char *args[MAX_LINE/2];
int should_run=1;
char cmd[MAX_LINE/2 +1];
char *val[MAX_LINE/2 +1];
char history[40];

int count;
while(should_run){
printf("osh>");
fflush(stdout);
fgets(cmd, 41, stdin);

/*Executing the last command if the current command is "!!" */
if(strcmp(cmd, "!!\n")==0){
if(count>0){
strcpy(cmd, history);}
else{
printf("NO COMMANDS FOUND IN HISTORY\n");
continue;
}}

splitcmd(cmd, args, val);
printf("argv[0]:%s\n", args[0]);

/* Exiting the shell if the input is "exit" */
if (strcmp(args[0], "exit") == 0) {
should_run = 0;
continue;
}

count++;
strcpy(history,cmd);
run(args, val);
}
return 0;
}
