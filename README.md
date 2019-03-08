rinix
=====

### What is this repository for? ###

This repository is for a Unix-like operating system in development for the x86 architecture.

This project is currently at version **0.0.5** (In Super-Alpha)

### How do I get set up? ###

Basically just download the repo and place anywhere you like.

#### Requirements ####

Currently configured to work on a Mac with MacPorts. Shell scripts need to be modified to compile on linux, for the most part, removing the "g" prefix should do it.

To build the operating system you have to install a **cross-compiler** and add it to you PATH shell variable. The cross compiler should be **i686-elf**.

For the most part referring to osdev.org should give you what you need.

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

As it is, qemu.sh is configured to optionally wait for a connection (usually from gdb).

To do this pass the flag '-d' like this:

` ./qemu.sh -d `

### Contribution guidelines ###

Because this a personal project, I am currently not accepting contributions. If you have a suggestion for some reason, I guess you could send it to me. Somehow.
