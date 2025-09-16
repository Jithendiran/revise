GIO is a library that builds on top of GObject and GLib.
GIO is the modern I/O and file abstraction layer in GLib (used by GTK).

| Area                | GIO Feature                     | Example                                        |
| ------------------- | ------------------------------- | ---------------------------------------------- |
| File handling       | `GFile`                         | Open, copy, move, delete files                 |
| Asynchronous I/O    | `GInputStream`, `GOutputStream` | Read/write without blocking UI                 |
| IPC / D-Bus         | `GDBus`                         | App-to-app communication                       |
| Application actions | `GAction`, `GSimpleAction`      | Menus, keyboard shortcuts                      |
| Virtual file system | GVfs / `GFile`                  | Works with local, remote (FTP, SMB, ...) files |

compile: ``gcc file_check.c -o check_file `pkg-config --cflags --libs gio-2.0``