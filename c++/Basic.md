# padding, alignment

If a memory is arranged as single bank of one-byte width, for int(4bytes) processor needs 4 cycles to read int.
To reduce the cycle. Memory will be arranged as group of 4 bytes

It is faster to access data that is located at a memory address that is divisible by a certain number. Putting your data on such “even” addresses is called memory alignment.

On modern platforms, all basic C types have to be self-aligned. This means that each basic type has to be located at a memory address that is a multiple of its size

--------------------------------------------------------------------
|Data Type |Size in Bytes  |Self-Alignment                         |
|----------|---------------|---------------------------------------|
|char	   |1	           | None (any address is fine)            |
|short	   |2	           | The address has to be a multiple of 2 |
|int	   |4	           | The address has to be a multiple of 4 |
|long	   |8	           | The address has to be a multiple of 8 |
|Pointer   |8	           | The address has to be a multiple of 8 |
--------------------------------------------------------------------

if the address of var is % of it's size should be 0

CPU prefers to read 1, 2, 4, 8, .. bytes in it's multiplied address

for example to read 8 bytes of data, 

if aligned
    address is 1000 - 1007, it is multiplies of 4
    so 1000- 1003 is one cycle and 1004 - 1007 is one cycle

if not aligned
    if address start address is 1001
    1st cycle 0x1000-0x1003 - needed 3 byte
    2nd cycle 0x1004-0x1007 - needed 4 byte
    3rd cycle 0x1008-0x100B - needed 1 byte

    if address start address is 1003
    1st cycle 0x1000-0x1003 - needed 1 byte
    2nd cycle 0x1004-0x1007 - needed 4 byte
    3rd cycle 0x1008-0x100B - needed 3 byte

for char if start address is 1001, it wont read from 1000-1003, it directly read 1001

# job
    Each program executed by the shell is started in a new process.

    All major shells, except the Bourne shell, provide an interactive feature called job
    control, which allows the user to simultaneously execute and manipulate multiple
    commands or pipelines. In job-control shells, all of the processes in a pipeline are
    placed in a new process group or job.

    Each process in a process group has the same integer process group
    identifier, which is the same as the process ID of one of the processes in the group,
    termed the process group leader.

    Job control permits a shell user to simultaneously execute multiple commands (jobs),
    one in the foreground and the others in the background.

    A session is a collection of process groups ( jobs)

    A job in Linux is a process (or a group of processes) that is managed by the shell. This means the shell keeps track of the job and allows you to control it using commands like jobs, fg, bg, and kill.

    Jobs exists only in shell level, process exists in OS level
    A process is just an executing program,  but a job is a shell-managed process (or group of processes).

    jobs - List all the jobs that are running or suspended.
    fg - Bring the job to the foreground.
    bg - Send the job to the background.
    stop or Ctrl + z - Suspend the job.
    kill or Ctrl + c - Terminate the job.

    `ls &` (when & is used it becomes background job)

    every job is a process, but not every process are job

    if ls is executed it is process not a job
    if ls & is executed it is process and job

    Jobs can be single or multiple process

    Single job
    > sleep 100 &

    Job as a process group
    > cat file.txt | grep "hello" | sort &

    This is a process group all have different process id with same process goup id
    > ps -o pid,pgid,command
        2345  2345  cat file.txt
        2346  2345  grep hello
        2347  2345  sort
     each is a uniqe process and shell goups them into single job

    > cat file.txt | grep "hello"
    This not a job


# how to create a job

    ls & 

# IPC

    Inter process communication 
    * pipes 
        *Only works for related process, like parent child when use fork()
    * named pipe
        * Communication between unrelated process
        * Eg: logger 
    * Message queues 
        * multiple process to send and receive structued message
    * Shared memory
    * Sockets
    * Signals

# pointers