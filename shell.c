#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main ()
{
	char line[1024];		//get command line
	char *argv[1024];		//user command
	char *argv2[1024];		//user command
	char *path = "/bin/";		//set path at bin
	char progpath[200];		//full file path
	int argc;			//arg count
	int pid;
	int i = 0;
	int k, y, x;
	int flag = 1;			
	int length;
	char *token;
	int j;

 	while(1)
	 {
		printf ("\n------THE SHELL------\n");	//print shell prompt

		fgets (line, 1024, stdin);
		length=strlen(line);
		printf("\n line length %d \n",length);	
		if (line[length - 1] == '\n')
		line[length - 1] = '\0';	//if user hits CTRL+C break

	    token = (char*)malloc (10*sizeof(strlen(line))); 
	    
	    token = strtok (line, " \n");	//split command into separate strings
	
	    
	    while (token != NULL)  
		{	
			argv[i]=token;
				
			token = strtok (NULL, " \n");	//continues to split from where it stopped 			
				
		  	if (strcmp(argv[i], "|")==0)	
		    {		
		    	flag = 0;		//when it finds a pipe
		    	printf ("\n pipe found\n");
			  	argv[i]=NULL; //"|" is a symbol not a command 
		    }
		  	else if (strcmp(argv[i], ">>")==0)
		    {
		      	flag = 2;
		      	printf ("\n redirection found\n");
			  	argv[i]=NULL; //">>" is a symbol not a command 
			  
		    }
			i++;			//goes to the next line in the array argv[i]
			printf("\n ekane thn %d epanalhpsh \n",i);
	    }
	
	
	    argv[i] = NULL;		//because execvp needs one NULL in the end for it to be executed 
	
	    argc = i;			
		printf("\n length of args:%d\n",argc); 
				
	    for (i = 0; i < argc; i++)
		{
	    	printf ("\n o argv %s \n", argv[i]);	//print the command 	
		}
		
		    	
	    strcpy (progpath, path);	//copy /bin/ to file path
	    strcat (progpath, argv[0]);	//puts the program into path
	
	  	for (i = 0; i < strlen (progpath); i++)
		{		
			if (progpath[i] == '\n')
			{
				progpath[i] = '\0';
			}
		}
	
		printf("\n progpath:%s \n",progpath); 


//ONLY ONE COMMAND
		if (flag==1)
		{		
			if(!fork()) 
			{
				execvp (progpath, argv);
			}
			waitpid (-1, NULL, 0); //waits all the childen to end in -1
			printf ("Luke ,I am your father");
			
			
		}
		//REDIRECTION
		else if(flag==2)  
		{
			
			if(!fork()) 
			{	
				int fd=open (argv[argc-1], O_WRONLY | O_APPEND | O_CREAT,S_IRWXU); 
				dup2 (fd, 1); 		// stdout
				execvp (progpath, argv);
			}
			waitpid (-1, NULL, 0); 
			printf ("Luke ,I am your father");
		}
		//PIPES
		else if (flag == 0)
		{			
			int fds[2];
			pipe (fds);
			int i;
		
		  	pid_t pid = fork ();
		
			if (pid == 0)
			{			// child process
		      	close (fds[1]);
			    dup2 (fds[0], 0);	//to kanei stdin
			    execvp (progpath, argv);
			    close(fds[0]);
			}
				// Synexizei o Parent process pid==1
			printf ("Luke ,I am your father");
			close (fds[0]);
			dup2 (fds[1], 1);	//to kanei stdout    
			execvp (progpath, argv);
			close(fds[1]);	
		}
		
		printf(" \n flag = %d \n",flag); //gia na to doume
		free(token);
		i=0;
		flag=1; //to restart the loop
	
	} 

	return 0;

}				

  


