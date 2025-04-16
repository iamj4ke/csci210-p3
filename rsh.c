#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for (size_t i = 0; i < N; i++) {
		if (strcmp(cmd, allowed[i]) == 0) {
			return 1;
		}
	}	
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char* argv[21];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';


	// My code
	
	// Tokenizes the command line
	char* tokenizedLine = strtok(line, " ");
	if (tokenizedLine == NULL) {
		continue;
	}
	// Determines if the command (first word of line) is an allowed command
	if (!isAllowed(tokenizedLine)) {
		printf("NOT ALLOWED!\n");
		continue;
	}

	// Determines the command line arguments and places them in args[]
	int argCount = 0;
	while (tokenizedLine != NULL && argCount < 20) {
		argv[argCount] = tokenizedLine;
		argCount++;
		tokenizedLine = strtok(NULL, " ");
	}
	// Sets end of arg list
	argv[argCount] = NULL;

	// Checks if the argument is 'exit'
	if (strcmp(argv[0], "exit") == 0) {
		return 0;
	}

	// Checks if the command is 'help'
	if (strcmp(argv[0], "help") == 0) {
		printf("The allowed commands are:\n");
		for (size_t i = 0; i < N; i++) {
			printf("%d: %s\n", i+1, allowed[i]);
		}
		continue;
	}

	// Checks if the command is 'cd'
	if (strcmp(argv[0], "cd") == 0) {
		// Too many arguments?
		if (argCount > 2) {
			printf("-rsh: cd: too many arguments\n");
		}
		// Chdir error?
		if (chdir(argv[1]) != 0) {
			perror("chdir");
		}
		continue;
	}

	pid_t pid;
	int status;
	posix_spawnattr_t attr;
	posix_spawnattr_init(&attr);

	if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
		perror("spawn failure");
	} else {
		waitpid(pid, &status, 0);
	}

	posix_spawnattr_destroy(&attr);

    }
    return 0;
}
