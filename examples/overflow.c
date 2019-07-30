// A C program to demonstrate buffer overflow 
// Simple example from https://www.geeksforgeeks.org/buffer-overflow-attack-with-example/

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

#ifndef EXAMPLE_DISABLE_BANNED_INSECURE
#include "banned.h"
#endif //EXAMPLE_DISABLE_BANNED_INSECURE

int main(int argc, char *argv[])
{ 
	// For this example, pretend that this is a security sensitive value:
	int other_value = 2;
	// Reserve 5 byte of buffer plus the terminating NULL. 
	// should allocate 8 bytes = 2 double words, 
	// To overflow, need more than 8 bytes... 
	char buffer[5];

	char input[10] = "123456789"; //plus a null terminating byte
	printf("Original other value= %d\n", other_value);
	printf("Source buffer content= %s\n", input);

	// copy the user input to mybuffer, without any bound checking
	// the developer could switch to a secure version like srtcpy_s()
	#ifdef EXAMPLE_APPLY_FIX
		// This shows what a developer is encouraged to fix the code to:
		strncpy_strsafe(buffer, 5, input);
		printf("strncpy_strsafe() executed...\n");
	#else
		// This shows the insecure behavior:
		strcpy(buffer, input);
		printf("strcpy() executed...\n");
	#endif
	printf("Destination buffer content= %s\n", buffer);
	printf("Final other value= %d\n", other_value);

	if (other_value != 2) {
		printf("NOTE: The other value variable was overflowed into!!!\n");
	}
	return 0; 
} 
