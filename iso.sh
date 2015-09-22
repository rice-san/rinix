#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/rinix.kernel isodir/boot/rinix.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "rinix" {
		multiboot /boot/rinix.kernel
}
EOF
grub-mkrescue -o rinix.iso isodir
