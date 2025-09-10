#!/bin/bash

if [[ $# -ne 5 ]]; then
    echo "$0 <addr> <port> <pass> <nick> <user>"
    exit 1
fi

cat <(echo "PASS $3") <(echo "NICK $4") <(echo "USER $5") - | nc -C $1 $2
