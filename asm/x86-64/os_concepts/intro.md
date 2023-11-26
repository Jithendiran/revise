[ref](https://neosmart.net/wiki/mbr-boot-process/)
[video](https://www.youtube.com/watch?v=7D4qiFIosWk)

Regardless of the computer or operating system, standard (“IBM-compatible”) desktop PCs and laptops all power on and start up using one of two ways: the traditional BIOS-MBR method and the newer UEFI-GPT method, used by the latest versions of Windows, Linux, and Mac OS X on newer PCs, laptops, and tablets. This article summarizes the process by which traditional BIOS PCs load an operating system, covering the basics and details of the BIOS, MBR, and bootsector.

## How BIOS load boot loader ?
    1. Power ON
        BIOS init perform POST test of hardware, Then show BIOS window, Then go to next step
    2. MBR
        BIOS calls the code stored in the MBR at the start of disk 0, search for boot signature if found go to next step else search for next device connected
    3. Load
        MBR loads the code from the bootsector of the active partition
    4. Boot loader
        Bootsector loads & runs the bootloader from it's file system

## BIOS
    The BIOS work is to detect and map connected hard disks. This list now comes in handy, as the BIOS will load a very small program from the first hard disk to the memory and tell the CPU to execute its contents, handing off control of the computer to whatever is on the hard drive and ending its active role in loading your PC. This hard drive is known as “the boot device,” “startup disk,” or “drive 0” and can usually be picked or set in the BIOS setup.

    Then BIOS loads the first 512 bytes from the hard drive of the selected boot device – these 512 bytes are what is commonly known as the MBR, or the Master Boot Record.

## The Master Boot Record (MBR) [ref](https://www.youtube.com/watch?v=1T26DpuKnVs)
    The MBR is the first and most important component on the software side of things in the boot procedure on BIOS-based machines. Every hard disk has an MBR, and it contains several important pieces of information.

    1. Bootsignature
        On IBM-compatible PCs (basically, everything) the final two bytes of the 512-byte MBR are called the boot signature and are used by the BIOS to determine if the selected boot drive is actually bootable or not. 
        On a disk that contains valid bootstrap code, the last two bytes of the MBR should always be 0x55 0xAA
        If the last two bytes of the MBR do not equal 0x55 and 0xAA respectively, the BIOS will assume that the disk is not bootable and is not a valid boot option – in this case, it will fall back to the next device in the boot order list (as configured in the BIOS setup)

    2. Partition Table
        First MBR contains partition table. which is an index of up to four partitions that exist on the same disk

    3. Bootstrap code
        The first 440bytes of these 512 bytes can contain literally anything – the BIOS will load it and execute its contents as-is, kicking off the bootloader procedure.

        The job of the bootstrap code segment in the MBR is pretty simple: look up the active partition from the partition table, and load that code into the memory for execution by the CPU as the next link in the boot chain.

        It is 1st bootloader process. The active partition contain's 2nd stage booloader
        Bootstrap code is depend on OS
        consider linux OS with GRUB 
        Bootstrap code in MBR points to /boot partition in hard disk then load the GRUB boot loader

   
    Once control of the PC has been handed-off from the BIOS to the bootstrap code in the MBR and from the MBR to the bootstrap code in the partition bootsector, and from there there to the executable boot files on the active partition, the actual logic involved in determining which operating system to load, where to load it from, which parameters/options to pass on to it, and completing any interactions with the user that might be available, the actual process of starting the operating system begins.