#!/bin/sh
ERROR=$(cppcheck ../src 3>&1 1>&2 2>&3 | tee /dev/stderr)  
# ^ redirect stderr to ERROR variable while still printing both stdout and stderr
if [[ -z "${ERROR// }" ]]
then
    echo "all OK"
else
    # force CICD build to fail if there are error messages
    echo "------------------------------"
    echo "errors reported, build failing"
    exit 1
fi