#!/bin/sh
ERROR=$(cppcheck  ../src  >/dev/null)
if [[ -z "${ERROR// }" ]] then
    echo "all OK"
else
    exit 1
fi