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