/*
 * Author: Max Neverov <maxsnev@gmail.com> 
 * Date: February 4, 2016
 * Description:
 *  Provides the ability to change the hostid of a machine
 *  using the sethostid(long) function from the unistd.h
 *  header file. Additionally, you can check the current
 *  hostid using the gethostid() function.
 *
 * Usage:
 *   hostidtool -g (get host id)
 *   hostidtool -s <hostid> (set host id. hostid is a
 *                           8-digit hexadecimal number,
 *                           with lowercase letters a-f)
 *
 */

#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include <stdint.h>

// Print the current host id
void printHostID() {
	long id;
	
	id = gethostid();
	printf("Current hostid: %08x\n", id);
} 

// Print usage guide
void printUsage() {
	printf("USAGE: hostidtool [-g(et)/-s(et)] <hostid>\n");
	printf("Format of <hostid> must be lowercase hexadecimal of length 8, with no leading 0x\n");
}

// Verify the input
int checkHostID(char* input) {
	// Check the length
	if (strlen(input) != 8) {
		fprintf(stderr, "ERROR: <hostid> argument must be of length 8.\n");
		return 1;
	}
	
	// Check for proper characters
	while (*input) {
		if (*input < 48 || *input > 102 || (*input  > 57 && *input < 97)) {
			fprintf(stderr, "ERROR: Format of <hostid> must be lowercase hexadecimal form.\n");
			return 1;
		}
		input++;
	}
	return 0;
}

int main(int argc, char** argv) {
	
	// Verify argument count
	if (argc != 2 && argc != 3) {
		printUsage();
		return 1;
	}

	// If the argument is -g
	if (strcmp(argv[1], "-g") == 0) {
		printHostID();	
	}

	// If the argument is -s
	else if (strcmp(argv[1], "-s") == 0) {
		printf("Checking input...\n");

		// If input check passes
		if (checkHostID(argv[2]) == 0 ) {
			printf("Input OK\n");
			int32_t id;
			sscanf(argv[2], "%x", &id);
			
			// Set hostid
			printf("Attempting to set host id...\n");
			
			// sethostid(int) expects a 32-bit integer input
			int result;

			result = sethostid(id);
			
			//Verify return
			if (result == 0) {
				printf("Sethostid successful.\n");
				printHostID();
			}
			else {
				fprintf(stderr, "ERROR: Sethostid error. (%i)\n", result);
				return 1;
			}
		}
		else {
			fprintf(stderr, "ERROR: Input error.\n");
			return 1;
		}

	}

	// Any other case
	else {
		printUsage();
		return 1;
	}
	return 0;
}

