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
    `$ make olddefconfig`   
    `$ make menuconfig`  
    `$ make prepare`  

4. compile the kernel  
    `make -j$(nproc)  V=1`  
    j is for number of threads  
    V=1 for enable verbose (for better error detect)  

5. Install kernel 
    `$ make modules_install`  
    `$ make modules_prepare`  
    `$ make headers_install`  
    `$ ln -sf /lib/modules/$(uname -r)/build /usr/src/linux-headers-$(uname -r)` # 

    ln -sf  /lib/modules/6.8.0/build /usr/src/linux-headers-6.8.0/
    This command will install the modules in  /lib/modules/<version> path

    Path to aware
    // /boot  
    // /usr/lib/modules/  
    // lib/modules  
    // /usr/src/  

6. Copy the kernel image  
    `$ sudo cp arch/x86/boot/bzImage /boot/vmlinuz-<kernel_version>`  


7. Copy the config file to boot location  
    `$ cp .config /boot/.config-<version>` 

8. Make initram file  
    `$ sudo mkinitcpio -k <kernel_version> -g /boot/initramfs-<kernel_version>.img`  
        or  
    `$ sudo mkinitramfs -o initrd.img-6.8.0-debug 6.8.0`  
        or   
    `$ sudo update-initramfs -c -k <kernel_version> `
 

9. Copy system map file  
    `$ cp System.map /boot/System.map-6.8.0`  

10 Make grub config   
   `$ grub-mkconfig -o /boot/grub/grub.cfg`  
    `$ update-grub`  

# Kernel Debug  
https://docs.kernel.org/dev-tools/gdb-kernel-debugging.html  

1. In .config these options needs to be enable
```
# Debug
CONFIG_DEBUG_INFO=y
CONFIG_DEBUG_INFO_SPLIT=y
CONFIG_GDB_SCRIPTS=y
CONFIG_KGDB=y
CONFIG_FRAME_POINTER=y  #CONFIG_UNWINDER_ORC needs to off
CONFIG_KGDB_SERIAL_CONSOLE=y
CONFIG_DEBUG_INFO_REDUCED=n
CONFIG_DEBUG_INFO_DWARF5=y

# Module cert
CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_FORCE=n
CONFIG_SYSTEM_TRUSTED_KEYS=""
CONFIG_SYSTEM_TRUSTED_KEYRING=n
CONFIG_SYSTEM_EXTRA_CERTIFICATE=n
```

2. Make
```
$ make -j12
```

3. Qemu start up
```
$ qemu-system-x86_64 -kernel ./arch/x86_64/boot/bzImage -append "nokaslr" -gdb tcp::1234 -S  # nokaslr will diable random addressing  
// -S will pause the execution until debugger connects, with out this flag also GDB will connect but kernel will start, only hit which debug point will come it is currently executing or later 
```
Qemu will wait until client connect

4. Client debugger  
    4.1. Command line gdb  
        `$ gdb vmlinux`  
        `(gdb) target remote :1234 # Connect with qemu`  
        `(gdb) hbreak start_kernel # adding break point`  
        `(gdb) c                   # continue booting`  
    4.2 Vscode gdb
        open code in linux-6.8 folder  
```
    {
    "version": "0.2.0",
    "configurations": [
            {
                "name": "Kernel Debugging",
                "type": "cppdbg",
                "request": "launch",
                "program": "${workspaceFolder}/vmlinux",
                "miDebuggerServerAddress": ":1234",
                "miDebuggerPath": "/usr/bin/gdb",
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}",
                "externalConsole": true,
                "MIMode": "gdb",
                "logging": {
                    "engineLogging": true
                }
            }
        ]
    }
```
        place debug points  
        click debug play icon in debug section  

5. make modules_install
6. mkinitramfs -o initrd.img-6.8.0-debug 6.8.0
7. qemu-system-x86_64 -kernel arch/x86_64/boot/bzImage -initrd initrd.img-6.8.0-debug -m 2G -gdb tcp::1234 -S
8. play debug in vscode
9.  To add debug option in qemu GUI
    https://www.redhat.com/en/blog/debugging-kernel-qemulibvirt

    `$ virsh list --all # note the name`  
    `$ virsh edit --domain <domain name which taken in list>`  
    Modify domain type='kvm' tag like below
    `<domain xmlns:qemu="http://libvirt.org/schemas/domain/qemu/1.0" type="kvm">`
    
    Then add the argumants inside `</domain>`
```
    <qemu:commandline>
      <qemu:arg value="-gdb"/>
      <qemu:arg value="tcp::1234"/>
     <!-- <qemu:arg value="-S"/> use less--> 
    </qemu:commandline>  
    <os>
      ...
      <kernel>/media/ssd/Linux/linux-6.8/arch/x86/boot/bzImage</kernel>
      <initrd>/media/ssd/Linux/linux-6.8/initrd.img-6.8.0</initrd>
      <cmdline>nokaslr root=UUID=f369a887-6e38-4eea-b7de-619281310a85</cmdline>
      ...
    </os>
```  

# Debug Lodable module
    To debug modules Kernel Debug steps are required  

Setup
-----
Host: Linux mint (linux-6.0.1)
Guest: 
    VM:qemu/kvm
    OS: Debian12 (linux-6.8.0) # dubugable kernel  
    From qemu shared folder is enabled and the driver program is shared between the host and guest
    /* while conect with debugger from Host make sure the driver path is aligned with guest path
    * for example host driver path is /home/jidesh/project 
    * in VM driver path is /media/project
    * while builing from VM the debug symbol (dwo) path is taken from vm as /media/project, This path is written in module.ko file
    * Consider now module is loaded in vm kernel, you are trying to load the symbol object from Host debugger, So it will try to refer /media/project from host
    * but there is no such path in host so it will throw no such file error in gdb
    * To overcome this error make sure mount the shared folder in /home/jidesh/project vm and build the kernel, so by this vm path will match with host
    */


## Debug from gdb cmdline
1. Once the OS in VM is loaded
2. Connect to GDB cmd line debugger
        `$ gdb vmlinux`  
        `(gdb) target remote :<port> # Connect with qemu` 
3. Load the module by `insmod module.ko`
4. Take .text segment address of module by 
        `$ cat /sys/modules/<module_name>/section/.text`  
5. create module nodes and necessary things  
6. Load symbol and place debugg point in gdb  
    `(gdb) add-symbol-file <module.ko> <Module_.text_address>`
    `(gdb) break function_name or line`
    `(gdb) c` // without this OS will be in pause state  
7. Do the steps to caught in debug points from VM

## Debug from VScode
1. install necessary plugins  
2. vscode launch.json file  
```
    {
    "version": "0.2.0",
    "configurations": [
            {
                "name": "Kernel Debugging",
                "type": "cppdbg",
                "request": "launch",
                "program": "${workspaceFolder}/vmlinux",
                "miDebuggerServerAddress": ":1234",
                "miDebuggerPath": "/usr/bin/gdb",
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}",
                "externalConsole": true,
                "MIMode": "gdb",
                "logging": {
                    "engineLogging": true
                },
                "setupCommands": [ // This section only to debug the modules, Before start debug load the module and take the modules .text address from /sys/modules/<module_name>/section/.text and replace the address in below section
                    { // This is not required 
                        "text": "set pagination off",
                        "description": "disable pagination",
                        "ignoreFailures": true
                    },
                    {
                        "text": "add-symbol-file /home/jidesh/Project/revise/OS_concepts/driver/char/scull/sc_1.ko <Module_.text_address>"
                        /*
                        eg:
                        "text": "add-symbol-file <path_of_module_.ko_file> 0xffffffffc0d86000"
                        My module is sc_1.ko, it's  .text is 0xffffffffc0d86000
                        Vscode is opened in /media/ssd/Linux/linux-6.8/ but my module in different location
                        /home/jidesh/Project/revise/OS_concepts/driver/char/scull/sc_1.ko
                        Vscode still able to debug, only thing is in <path_of_module_.ko_file> give /home/jidesh/Project/revise/OS_concepts/driver/char/scull/sc_1.ko
                        in <Module_.text_address> give 0xffffffffc0d86000 
                        Replace with current values 
                        */
                    }
                ],
            }
        ]
    }
```
```
If add-symbol-file is not there in launch.json and still we able to give input to GDB in vscode by debugger console
1. open debug console  
2. In the bottom input text field type `-exec {options}`  
    For eg:
      -exec add-symbol-file /home/jidesh/Project/revise/OS_concepts/driver/char/scull/sc_1.ko address
```


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

3. after this if new kernel is not listed under grub
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

4. If no file system or VFS issue is happening in boot, it might be missing of file system in initrd.img file

To view  initrd.img  
    `$ lsinitramfs -l /boot/initrd.img`

To add particular module
    Add the modules in `/etc/initramfs-tools/modules`
    for eg  
    `nano /etc/initramfs-tools/modules`  
     Add below 2 lines in file and save,  
    `ext4`  
    `jbd2`  
    then rexecute mkinitramfs  

qemu-system-x86_64 -kernel arch/x86_64/boot/bzImage -initrd initrd.img-6.8.0 -append "nokaslr root=UUID=f369a887-6e38-4eea-b7de-619281310a85" -smp 4 -m 2G -gdb tcp::1234 -hda /var/lib/libvirt/images/debian12-1.qcow2

# Debug module
1. Build module
2. $ gdb vmlinux 
3. (gdb) add-symbol-file /home/jidesh/Project/revise/OS_concepts/driver/char/scull/mutex.ko 0xffffffffc0c5d000 
    0xffffffffc0c5d000 is taken from $ cat /sys/module/mutex/section/.text
    (gdb) break scull_write
    (gdb) break scull_read 
    (gdb) c

 4. From qemu 
        $ insmod mutex.ko
        $ mknod /dev/mutex c 27 0
        $ cat /dev/mutex


To include all modules
-------
To build Kernel in one machine and run in another machine  
    Copy these things to new machine   
        * vmlinuz-<version> for /boot  
        * initrd.img-<version> for /boot  
        * .config-<version> for /boot  
        * /lib/modules/<version>  
    Make grub update  

----------  
While building kernel some options may not available in menuconfig  
To enable that manually instead of edit in .config file set using `scripts/config -e {OPTION}` it will enable,  
`scripts/config -d {OPTION}` will disable

eg: `$ ./scripts/config -e CONFIG_DEBUG_INFO_DWARF5`


Links  
-----
https://libvirt.org/formatdomain.html 
https://medium.com/@navaneethrvce/debugging-your-linux-kernel-module-21bf8a8728ba