
GApplication is a high-level API provided by GLib (part of the GNOME platform) that helps manage the lifecycle of a desktop or command-line application. It handles things like:
* Startup and shutdown
* Single-instance applications
* Command-line argument parsing
* DBus integration



1. Single-instance Management
By default, GApplication ensures that only one instance of your app runs at a time. If you try to launch a second one, it will send a message to the first and exit.


| Signal         | Description                                      |
| -------------- | ------------------------------------------------ |
| `startup`      |Initialize things that last for the app's life    |
| `activate`     | Called when the app is launched or re-activated  |
| `open`         | Used to open files (e.g., double-clicking files) |
| `command-line` | For custom CLI handling                          |
| `shutdown`     | Clean-up work before exit                        |

```
main()
  └─ g_application_run()
       ├─ startup
       ├─ activate or command-line
       ├─ [your app runs]
       └─ shutdown (when app quits)

```


Doubts 
what are the life cycle of desktop?
Why it has to handle Startup and shutdown, Single-instance applications, ..


// gcc Gio_4.c -o /tmp/pgm `pkg-config --cflags --libs gio-2.0`