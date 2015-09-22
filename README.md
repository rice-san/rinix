rinix
=====

### What is this repository for? ###

This repository is for a Unix-like operating system in development for the x86 architecture.

We are currently at version **0.0.3** (In Alpha)

### How do I get set up? ###

Basically just download the repo and place anywhere you like.

#### Requirements ####

Hopefully you have a linux machine to run and build this, I haven't tested this anywhere else.

To build the operating system you have to install a **cross-compiler** and add it to you PATH shell variable. The cross compiler should be **i686-elf**.

Also you should install **grub-mkrescue** and **qemu**.

Run this in the shell:

` sudo apt-get install grub-mkrescue `

` sudo apt-get install qemu-system-i386 `

You should be all set after that.

#### Building and running ####


Once you've done that, you should clean the folders of junk files (e.g. object files and extra files from editors). To do that, go to the root of the project and run this in the shell:

` ./clean.sh `

So to build it run:

` ./build.sh `

Create an iso file of the image like this:

` ./iso.sh `

And then start qemu by writing:

` ./qemu.sh `

##### Note about qemu #####

As it is, qemu is configured to wait for a connection (usually from gdb).

To prevent this behavior, you need to change a line in **qemu.sh**.

Change this line:

` qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom rinix.iso `

To this:

` qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom rinix.iso '

### Contribution guidelines ###

Because this a personal project, I am currently not accepting contributions. If you have a suggestion for some reason, I guess you could send it to me. Somehow.

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact