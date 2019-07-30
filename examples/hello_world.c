#include <stdio.h>
#include <string.h>

#ifndef EXAMPLE_DISABLE_BANNED_INSECURE
#include "banned.h"
#endif //EXAMPLE_DISABLE_BANNED_INSECURE

int main()
{
   printf("Hello, World!\n");
   char message[] = "Hello, World!\n";
   unsigned int len = strlen(message);
   char dest[len+1];
   strncpy(dest, message, len);
   printf("%s", dest);
   return 0;
}
