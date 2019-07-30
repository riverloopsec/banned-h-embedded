#!/bin/bash

echo "======= Hello World ======="
rm -f hellow_world_*
echo "------- Warning Only -------"
gcc -I.. -DEMBD_STRSAFE_WARN_ONLY -o hello_world_warn hello_world.c
# This would result in a compilation error:
#gcc -I.. -o hello_world_err hello_world.c

echo "======= Overflow ======="
rm -f overflow_*
echo "------- Warning Only - Insecure -------"
gcc -I.. -DEMBD_STRSAFE_WARN_ONLY -o overflow_warn overflow.c
./overflow_warn
echo "------- Without Header - Insecure -------"
gcc -I.. -DEXAMPLE_DISABLE_BANNED_INSECURE -o overflow_insecure overflow.c
./overflow_insecure
echo "------- With Fix Applied -------"
gcc -I.. -DEXAMPLE_APPLY_FIX -o overflow_fixed overflow.c
./overflow_fixed

echo "======= Truncation ======="
rm -f truncation_*
echo "------- Warning Only - Insecure -------"
gcc -I.. -DEMBD_STRSAFE_WARN_ONLY -o truncation_warn truncation.c
./truncation_warn
echo "------- Without Header - Insecure -------"
gcc -I.. -DEXAMPLE_DISABLE_BANNED_INSECURE -o truncation_insecure truncation.c
./truncation_insecure
echo "------- With Fix Applied -------"
gcc -I.. -DEXAMPLE_APPLY_FIX -o truncation_fixed truncation.c
./truncation_fixed
