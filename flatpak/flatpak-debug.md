1. `flatpak-builder --user --force-clean --state-dir=/tmp/flatpak-state --install /tmp/build-dir org.example.Factorial.json`
    --install to install the app after building
2. flatpak run org.example.Factorial

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
jidesh@jidesh-MS-7E26:/media/ssd/Project/revise/flatpak$ 
```

3. flatpak run --devel --filesystem=/media/ssd/Project/revise/flatpak org.example.Factorial

# Debugger

1. flatpak run --devel --command=sh --filesystem=/media/ssd/Project/revise/flatpak --share=network org.example.Factorial
    ```
    --share=network is important
    ```
2. gdbserver :12345 /app/bin/factorial
```
[📦 org.example.Factorial flatpak]$ gdbserver :12345 /app/bin/factorial 
Process /app/bin/factorial created; pid = 7
Listening on port 12345
```

in host machine

3. sudo gdb /tmp/build-dir/files/bin/factorial
4. symbol-file /tmp/build-dir/files/lib/debug/bin/factorial.debug
5. target remote :12345
6. dir /media/ssd/Project/revise/flatpak/src
7. break main
8. continue


1. flatpak-builder --share=network --run /tmp/build-dir/ ./org.example.Factorial.json bash
2. gdbserver :12345 /app/bin/factoria
3. Host
```
gdb
dir /media/ssd/Project/revise/flatpak/src
```

1. flatpak-builder --user --force-clean --state-dir=/tmp/flatpak-state /tmp/build-dir org.example.Factorial.json
1. flatpak-builder --share=network --run /tmp/build-dir/ ./org.example.Factorial.json bash
2. Host
```
sudo sysctl -w kernel.yama.ptrace_scope=0
gdb
attach <pid-of-1'st-process>
```

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

success method
```
HOST
1. flatpak-builder --user --force-clean --state-dir=/tmp/flatpak-state /tmp/build-dir org.example.Factorial.json
2. flatpak-builder --share=network  --run /tmp/build-dir/ ./org.example.Factorial.json bash

Flatpak
3. gdbserver :12345 /app/bin/factorial 

HOST
4. gdb
5. (gdb) target remote :12345
6. (gdb) symbol-file /tmp/build-dir/files/lib/debug/bin/factorial.debug
7. (gdb) dir /media/ssd/Project/revise/flatpak/src
8. (gdb) b main.c:factorial:8
9. c
```