# Object Allocation — Stack, Heap, and Static Storage

## The Four Storage Durations
Every object in a C++ program lives in one of four storage regions. The region determines three things: where the memory comes from, when the object is constructed, and when it is destroyed.

| Storage Duration | Region | Constructed | Destroyed |
|---|---|---|---|
| Automatic | Stack | When declaration is reached | When enclosing scope exits |
| Dynamic | Heap | When `new` is executed | When `delete` is executed |
| Static | Static segment | Once, before `main` runs | Once, when program exits |
| Thread-local | Per-thread static | When thread starts | When thread exits |

Each exists because the other three cannot cover its use case.

##  Content
1. [Stack](./Stack.md)
2. [Dynamic](./Dynamic.md)
3. [Static](./Static.md)
4. [Pre-allocation](./Pre-allocation.md)