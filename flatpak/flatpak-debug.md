# Debug Flatpak

**Host machine**
1.  `flatpak-builder --user --force-clean --state-dir=/tmp/flatpak-state /tmp/build-dir ./org.example.Factorial.json`
    
    This will build the programs

    --user -> this is take user installed runtime, provider (this is optional)

    --force-clean -> each time while running it will clear the artifact folder

    --state-dir=/tmp/flatpak-state -> This the place where it will download it's dependencies,..

    /tmp/build-dir -> This is the artifact folder, where builded binary will found

    ./org.example.Factorial.json -> this the manifest file


2.  `flatpak-builder --share=network  --run /tmp/build-dir/ ./org.example.Factorial.json bash`
    
    This will not build the program, instead this will use the already builded one and give with environment to run

    --share=network-> This is more important for debugging from host, because application will run in a flatpak (isolated container), --share=network will share the network between host and flatpak

    --run -> will run the command mentioned, here the command is `bash`, bash will start the bash shell

    /tmp/build-dir/ -> Already builded artifact

    ./org.example.Factorial.json -> this the manifest file

    bash -> this is the user command

    Now terminal is in flatpak container, the builded app is available in `/app/bin`

**Flatpack container**

3.  `gdbserver :12345 /app/bin/factorial` 

    ```
    [📦 org.Gnome.SDK flatpak]$ gdbserver :12345 /app/bin/factorial 
    Process /app/bin/factorial created; pid = 7
    Listening on port 12345
    ```

    gdbserver This will start the gdb server, with port `12345`
    This is wait until client connect

**Host machine**

4.  `gdb`

    Open a new terminal and start gdb client from host machine

5.  `(gdb) target remote :12345`
    
    This will connect with the flatpak container

6.  `(gdb) symbol-file /tmp/build-dir/files/lib/debug/bin/factorial.debug`

    This will attach the debug symbol

7.  `(gdb) dir /media/ssd/Project/revise/flatpak/src`

    This will set the folder for source path

8.  `(gdb) b main.c:factorial:8`

    This will place the breakpoint

9.  `(gdb) c`

    This will continue the program

**Install in host**
**Host**

Till now we haven't installed the app, now will install the app

10. `flatpak-builder --user --force-clean --state-dir=/tmp/flatpak-state --install /tmp/build-dir org.example.Factorial.json`

    --install This will install the Factorial in host application

11. `flatpak list`

    This will list the installed application from host

    ```sh
    jidesh@jidesh-MS-7E26:/media/ssd/Project/revise/flatpak$ flatpak list
    Name                                          Application ID                          Version     Branch         Origin              Installation
    Resources                                     net.nokyan.Resources                    1.8.0       stable         flathub             system
    Factorial                                     org.example.Factorial                               master         factorial-origin    user
    Mesa                                          org.freedesktop.Platform.GL.default     25.1.7      24.08          flathub             system
    Mesa                                          org.freedesktop.Platform.GL.default     25.1.7      24.08          flathub             user
    Mesa (Extra)                                  org.freedesktop.Platform.GL.default     25.1.7      24.08extra     flathub             system
    Mesa (Extra)                                  org.freedesktop.Platform.GL.default     25.1.7      24.08extra     flathub             user
    openh264                                      org.freedesktop.Platform.openh264       2.5.1       2.5.1          flathub             system
    openh264                                      org.freedesktop.Platform.openh264       2.5.1       2.5.1          flathub             user
    Builder                                       org.gnome.Builder                       48.2        stable         flathub             system
    GNOME Application Platform version 48         org.gnome.Platform                                  48             flathub             system
    GNOME Application Platform version 48         org.gnome.Platform                                  48             flathub             user
    GNOME Software Development Kit version 48     org.gnome.Sdk                                       48             flathub             system
    GNOME Software Development Kit version 48     org.gnome.Sdk                                       48             flathub             user
    ```    
    

12. `flatpak run --devel --command=sh --filesystem=/media/ssd/Project/revise/flatpak --share=network org.example.Factorial`
    
    This will run the installed application

    run -> run the application, here the application is org.example.Factorial

    --devel -> run in developer mode

    --command=sh -> this execute the command in application flatpak conteiner

    --filesystem=/media/ssd/Project/revise/flatpak -> attach this file system to flatpak

    --share=network -> This will share the network with host machine

**Faltpak**

13. gdbserver :12345 /app/bin/factorial

    This will start the gdb server in flatpak, we can connect with gdb client from host

---

Below is failure method need more info
```

HOST
1. flatpak-builder --user --force-clean --state-dir=/tmp/flatpak-state /tmp/build-dir org.example.Factorial.json
2. flatpak-builder --share=network  --run /tmp/build-dir/ ./org.example.Factorial.json factorial
now you may see factorial output in the screen

HOST
3. sudo sysctl -w kernel.yama.ptrace_scope=0
4. gdb -p <pid-of factorial>
4. file /tmp/build-dir/files/bin/factorial
5. symbol-file /tmp/build-dir/files/lib/debug/bin/factorial.debug 
7. set sysroot /var/lib/flatpak/runtime/org.gnome.Sdk/x86_64/48/active/files
8. set debug-file-directory /var/lib/flatpak/runtime/org.gnome.Sdk/x86_64/48/active/files/lib/debug
9. set solib-search-path /var/lib/flatpak/runtime/org.gnome.Sdk/x86_64/48/active/files/lib/x86_64-linux-gnu
6. dir /media/ssd/Project/revise/flatpak/src
9. break main
10.run

```
