#!/bin/sh
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
	$MAKE -C $PROJECT clean
done

rm -rfv sysroot
rm -rfv isodir

if [ "$1" != "-i" ] 
then
rm -rfv rinix.iso
fi
