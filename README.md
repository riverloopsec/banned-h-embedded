# Introduction

Introducing safe libraries to development is nothing new, as was covered in slide 7 of the 2007 presentation on [SDL for Windows Vista](https://www.acsac.org/2007/workshop/Howard.pdf).
As discussed later in the deck, 41% of bugs they knew they removed in Vista early on were due to removal of 'banned' API function calls.

# Example

Looking at the examples should help a developer understand the usage of this header.

To build them:
```bash
cd examples
clear; ./build.sh
```

If the header isn't used, the program may missbehave or crash:
```bash
$ ./overflow_insecure
Original other value= 2
Source buffer content= 123456789
Abort trap: 6
```

In the example when the library is set to warn, via `EMBD_STRSAFE_WARN_ONLY`, gives warnings at compile time recommending a better function:
```bash
overflow.c:32:3: warning: 'strcpy' is deprecated: This function isn't considered secure. [-Wdeprecated-declarations]
                strcpy(buffer, input);
                ^~~~~~
                strncpy_strsafe
...
```
When this program is run, it is still vulnerable to an overflow:
```bash
$ ./overflow_warn 
Original other value= 2
Source buffer content= 123456789
Destination buffer content= 123456789
Final other value= 959985462
strcpy() executed...
NOTE: The other value variable was overflowed into!!!
```

If the developer changes to use the safe function, the string will be safely truncated and terminated with a null byte:
```bash
$ ./overflow_fixed
Original other value= 2
Source buffer content= 123456789
Destination buffer content= 1234
Final other value= 2
strcpy() executed...
```

Note that simply using `strncpy` in this case would not terminate the buffer, and thus the `printf` may read off the end of the string.


# Prior Work

A number of tools exist to look for these 'banned' functions, including some listed below.
These however, are _not_ focused on the unique needs of the embedded development environment.

- Headers to exclude or warn on them at compile time, designed for Windows development:
  - https://github.com/tpn/winsdk-10/blob/master/Include/10.0.16299.0/shared/dontuse.h
  - https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/host/x86_64-w64-mingw32-4.8/+/l-preview/x86_64-w64-mingw32/include/strsafe.h
- Tools to grep the source for them:
  - List coming soon.
- Tools to look for them in compiled binaries:
  - https://www.vdalabs.com/2019/03/09/automating-ghidra-writing-a-script-to-find-banned-functions/

