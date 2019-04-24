// A C program to demonstrate buffer overflow 
// Simple example from https://www.geeksforgeeks.org/buffer-overflow-attack-with-example/

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

#ifndef EXAMPLE_DISABLE_BANNED_INSECURE
#include "banned.h"
#endif //EXAMPLE_DISABLE_BANNED_INSECURE

#pragma pack(push, 1)
typedef struct internal_struct {
    char buffer[5];
    char secret_buffer[9];
} internal_struct_t;
#pragma pack(pop)

int main(int argc, char *argv[])
{ 
	// For this example, pretend that this is a security sensitive value "Password"
	// which we put into our data structure alongside the variable which is vulnerable
	// to a truncated strncpy.
	internal_struct_t storage; 
	char secret_buffer[] = "Password";
	#ifndef EXAMPLE_DISABLE_BANNED_INSECURE
		int res = strncpy_strsafe(storage.secret_buffer, 9, secret_buffer);
	#else
		strncpy(storage.secret_buffer, secret_buffer, 9);
	#endif

	// Now we will use our input buffer, and copy part of it to the destination buffer
	// which is inside our strucuture.
	char input[10] = "123456789"; //plus a null terminating byte
	printf("Secret buffer value= %s\n", storage.secret_buffer);
	printf("Source buffer content= %s\n", input);

	// Copy the user input to buffer within internal_struct_t, truncating it
	// to the length of the buffer to avoid buffer-overflow issues.
	#ifdef EXAMPLE_APPLY_FIX
		// This shows what a developer is encouraged to fix the code to:
		strncpy_strsafe(storage.buffer, 5, input);
		printf("strncpy_strsafe() executed...\n");
	#else
		// This shows the insecure behavior:
		strncpy(storage.buffer, input, 5);
		printf("strncpy() executed...\n");
	#endif
	printf("Destination buffer content= %s\n", storage.buffer);
	printf("Final secret buffer value= %s\n", storage.secret_buffer);

	return 0; 
}
