#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
int main()
{
    // We use two pipes
    // First pipe to send input string from parent
    // Second pipe to send concatenated string from child
 
    int foo1[2];// Used to store two ends of first pipe
    int foo2[2];// Used to store two ends of second pipe
    char input_str[100],ch;
    pid_t p;
    if (pipe(foo1)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(foo2)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    printf("PLEASE ENTER STRING\n");
    scanf("%[^\n]",input_str);
    p = fork();
 
    if (p < 0)
    {
        fprintf(stderr, "fork Failed" );
        return 1;
    }
 
    // Parent process
    else if (p > 0)
    {
        char modified_str[100];
 
        close(foo1[0]);  // Close reading end of first pipe
 
        // Write input string and close writing end of first
        // pipe.
        write(foo1[1], input_str, strlen(input_str)+1);
        close(foo1[1]);
 
        // Wait for child to send a string
        wait(NULL);
 
        close(foo2[1]); // Close writing end of second pipe
 
        // Read string from child, print it and close
        // reading end.
        read(foo2[0], modified_str, 100);
        printf("Converted  string is : %s\n", modified_str);
        close(foo2[0]);
    }
 
    // child process
    else
    {
        close(foo1[1]);  // Close writing end of first pipe
 
        // Read a string using first pipe
        char modified_str[100];
        read(foo1[0], modified_str, 100);
        int k = strlen(modified_str);
        int i;
        for (i=0; i<strlen(input_str); i++)
        {
        	ch = input_str[i];
            if (ch >= 'A' && ch <= 'Z')
            input_str[i] = input_str[i] + 32;
            else if (ch >= 'a' && ch <= 'z')
            input_str[i] = input_str[i] - 32;
		}
		strcpy( modified_str,input_str);
        // Close both reading ends
        close(foo1[0]);
        close(foo2[0]);
 
        // Write converted  string and close writing end
        write(foo2[1], modified_str, strlen(modified_str)+1);
        close(foo2[1]);
        exit(0);
    }
}
