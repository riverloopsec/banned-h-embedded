# Introduction

Developers on embedded systems can include this header file to help them and their colleagues:
- avoid functions which are known to commonly lead to vulnerabilities
- utilize functions designed to help developers be more explicit about their desired behavior
- utilize functions that return data what the function did in edge cases
- by improving buffer handling during development, reduce time spent reviewing/fixing based on alerts from static code analysis tools

Introducing safe libraries to development is nothing new, as was covered in the 2007 presentation on [SDL for Windows Vista](https://www.acsac.org/2007/workshop/Howard.pdf) (slide 7).
However, these basic libraries have been shown to provide significant value - as discussed later in the deck, 41% of bugs that Microsoft knew they removed in Vista early on were due to removal of 'banned' API function calls.

Read below for examples

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

# Frequenently Asked Questions

## Why are some 'n' functions banned?
These variants are sometimes banned because they are tough to call correctly. As Microsoft learned when making the original banned.h, there are numerous errors observed when people call those. The replacement functions attempt to helping fix some issues such as how the default 'n' functions can fail to not null-terminate on overflowed buffers and will not return an error code on overflow.

## Will this make my code 100% secure?
No. Simply replacing a banned function call with a better replacement does not guarantee that the code is secure. You can still use the functions incorrectly, but based on learnings from passed banned.h files and from our team's experience, they should help produce more secure code with less buffer-overruns if used thoughfully.

## What are other methods?
In Windows development, developers get the benefit often other ways than banned.h today, such as using Safe CRT if the compiler supports it (introduced in Visual Studio 2005). This will warn of deprecated functions like this file does, and sometimes change calls to safe variants for the user at compile time.
We do not know of equivalent methods being used in embedded compilers at this time.

## Have another question?
Reach out to us on GitHub Issues or by emailing team [at] riverloopsecurity [com].

# Prior Work

A number of tools exist to look for these 'banned' functions, including some listed below.
These however, are _not_ focused on the unique needs of the embedded development environment.

- Papers about this approach to software security:
  - Howard, Michael and LeBlanc, David. "Writing Secure Code" (2nd Edition). 2003. Available at https://ptgmedia.pearsoncmg.com/images/9780735617223/samplepages/9780735617223.pdf.
    - pg. 157 mentions redefining strcpy to remove it
    - pg. 163 introduces the strsafe.h as a result of the Windows Security Push from 2002
  - Rains, Tim. "Microsoft's Free Security Tools - banned.h". 2013. Available at https://www.microsoft.com/security/blog/2012/08/30/microsofts-free-security-tools-banned-h/.
  - Howard, Michael. "Security Development Lifecycle (SDL) Banned Function Calls". 2012. Available at https://docs.microsoft.com/en-us/previous-versions/bb288454(v=msdn.10).
- Headers to exclude or warn on them at compile time, designed for Windows development:
  - Microsoft ChakraCore, Windows focused banned.h: https://github.com/Microsoft/ChakraCore/blob/master/lib/Common/Banned.h
  - "About Strsafe.h" https://docs.microsoft.com/en-us/windows/desktop/menurc/strsafe-ovw
  - https://github.com/tpn/winsdk-10/blob/master/Include/10.0.16299.0/shared/dontuse.h
  - https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/host/x86_64-w64-mingw32-4.8/+/l-preview/x86_64-w64-mingw32/include/strsafe.h
- Tools to grep the source for them:
  - List coming soon.
- Tools to look for them in compiled binaries:
  - Ghidra script: https://www.vdalabs.com/2019/03/09/automating-ghidra-writing-a-script-to-find-banned-functions/
  - IDAPython script: https://github.com/steph3nsims/banned_functions/blob/master/banned_functions.py
