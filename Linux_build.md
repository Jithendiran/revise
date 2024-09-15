# Build Linux kernel
1. clone repo  
    https://www.kernel.org/ or https://github.com/torvalds/linux  
2. change directory to newely cloned or downloaded   
    `$ cd /linux-<version>`  
2. Install necessary tools  
    `$ sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison`  

    For best check the necessary software from  
    linux-<version>/Documentation/Changes

3. Configure required kernel modules  
    `$ make menuconfig`

4. compile the kernel  
    `make -j$(nproc)  V=1`  
    j is for number of threads  
    V=1 for enable verbose (for better error detect)  

5. Install kernel 
    `$ make modules_install`  
    This command will install the modules in  /lib/modules/<version> path

6. Copy the kernel image
    `$ sudo cp arch/x86/boot/bzImage /boot/vmlinuz-<kernel_version>`  

7. Make initram file  
    `$ sudo mkinitcpio -k <kernel_version> -g /boot/initramfs-<kernel_version>.img`  
    or   
    `$ sudo update-initramfs -c -k <kernel_version> `

8. Copy the config file to boot location
    `$ cp .config /boot/.config-<version>`  

9. Copy system map file
    `$ cp System.map /boot/System.map-6.8.0`

10 Make grub config 
   `$ grub-mkconfig -o /boot/grub/grub.cfg`  
    `$ update-grub`

error & solutions
-------------------------
1. If Wframe-larger-than error is cauing then add these lines in Makefile and recompile the kernel
```
    # Add this line at the top of the Makefile, or after existing KBUILD_CFLAGS settings
    KBUILD_CFLAGS += -Wframe-larger-than=2048

    # Example of existing flags
    KBUILD_CFLAGS   := -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs \
                    -fno-strict-aliasing -fno-common \
                    -Werror-implicit-function-declaration \
                    -Wno-format-security
                    
    EXTRA_CFLAGS += -Wframe-larger-than=2048
```

2. If kernel is installed but not listed in GRUB boot
    1. Check in `/boot/grub/menu.lst` file boot entry is listed or not, If not make a entry by duplicating and change necessary things for new kernel
    2. check `/boot/grub/grub.cfg` file, if kernel entry not found make a entry by duplicate

11. after this if new kernel is not listed under grub
    1. check modules in `/lib/modules/6.8.0/`, If present modules are installed
    2. check  `/boot/grub/menu.lst`, if modules listed here and not listed in  `/boot/grub/grub.cfg` means entry is not written so make a entry.

```
        menuentry 'Debian8 GNU/Linux' --class debian --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-simple-f369a887-6e38-4eea-b7de-619281310a85' {
        load_video
        insmod gzio
        if [ x$grub_platform = xxen ]; then insmod xzio; insmod lzopio; fi
        insmod part_msdos
        insmod ext2
        search --no-floppy --fs-uuid --set=root f369a887-6e38-4eea-b7de-619281310a85
        echo    'Loading Linux 6.1.0-18-amd64 ...'
        linux   /boot/vmlinuz-6.1.0-18-amd64 root=UUID=f369a887-6e38-4eea-b7de-619281310a85 ro  quiet
        echo    'Loading initial ramdisk ...'
        initrd  /boot/initrd.img-6.1.0-18-amd64
        }
```
        Here  f369a887-6e38-4eea-b7de-619281310a85,/boot/vmlinuz-6.1.0-18-amd64, /boot/initrd.img-6.1.0-18-amd64 -> replace actual

-------
To build Kernel in one machine and run in another machine  
    Copy these things to new machine   
        * vmlinuz-<version> for /boot  
        * initrd.img-<version> for /boot  
        * .config-<version> for /boot  
        * /lib/modules/<version>  
    Make grub update