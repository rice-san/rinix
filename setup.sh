#!/bin/bash

echo "Adding execute permissions to shell scripts"
chmod u+x ./*.sh
export PATH="$HOME/opt/cross/bin:$PATH" #set this to the path of the 'bin' directory of the cross compiler
echo "Adding cross compiler to shell PATH variable"
echo "Path set to: $PATH"