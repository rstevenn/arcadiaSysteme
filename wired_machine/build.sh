#!/bin/sh
set -ex
gcc -o2 -Wall -Werror src/parse_wiasm.c src/wiasm_to_wbin.c -o bin/wiasm_to_wbin
gcc -o2 -Wall -Werror src/wired_machine.c -o bin/wired_machine

