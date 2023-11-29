#!/bin/sh
set -ex 

bin/wiasm_to_wbin test/test.wiasm
bin/wired_machine out.wbin
