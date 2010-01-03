#!/bin/bash

# Creates .tar.gz package of specified version.
# Takes one argument - identification of commit

COMMIT="$1"
PREFIX=cu-$COMMIT
FN=cu-$COMMIT.tar.gz
CMD="git archive"
FILES="COPYING COPYING.LESSER cu.c cu.h Makefile check-regressions"

if [ "$COMMIT" = "" ]; then
    echo "Error: you must specify commit which should be packed"
    exit -1;
fi;

if echo "$COMMIT" | grep '^v[0-9]\.[0-9]\+' >/dev/null 2>&1; then
    tmp=$(echo "$COMMIT" | sed 's/^v//')
    PREFIX=cu-$tmp/
    FN=cu-$tmp.tar.gz
fi


$CMD --prefix="$PREFIX" --format=tar $COMMIT -- $FILES | gzip >"$FN"
echo "Package: $FN"

