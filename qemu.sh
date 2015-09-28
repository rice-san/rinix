#!/bin/sh
set -e
. ./iso.sh

if [ "$1" = "-d" ] 
then
qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom rinix.iso
else
qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom rinix.iso
fi
