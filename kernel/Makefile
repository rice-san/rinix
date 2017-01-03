DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
 
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc
 
ARCHDIR:=arch/$(HOSTARCH)
MEMDIR:=mm
FSDIR:=fs
 
include $(ARCHDIR)/make.config
include $(MEMDIR)/make.config
include $(FSDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
$(KERNEL_MEM_OBJS) \
rinix/rinix.o \
rinix/io_port.o \
rinix/pause.o \
rinix/panic.o \
rinix/debug.o \
 
OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S
 
all: rinix.kernel
 
rinix.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot rinix.kernel
 
$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
clean:
	rm -f rinix.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-arch-headers install-kernel
 
install-headers:
	gmkdir -p $(DESTDIR)$(INCLUDEDIR)
	gcp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-arch-headers:
	gcp -R arch/$(HOSTARCH)/include $(DESTDIR)$(INCLUDEDIR)/arch

install-kernel: rinix.kernel
	gmkdir -p $(DESTDIR)$(BOOTDIR)
	gcp rinix.kernel $(DESTDIR)$(BOOTDIR)
 
-include $(OBJS:.o=.d)