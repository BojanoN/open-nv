#!/bin/sh
ERROR="$(cppcheck ../src 3>&1 1>&2 2>&3 | tee /dev/stderr)"  
# redirect stderr to ERROR variable while still printing both stdout and stderr
echo "------------------------SUMMARY ------------------------"
if [[ -z "${ERROR// }" ]]
then
    echo "all OK"
else
    # force CICD build to fail if there are error messages
    echo "errors reported, build failing:"
    echo ""
    echo -e "${ERROR}"
    exit 1
fi