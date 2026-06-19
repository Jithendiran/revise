## Module Scanning & Dependency Resolution (C++20)
1. **What Happens**
   The build toolchain scans the source code specifically to look for C++20 module declarations. It parses the dependencies between modules to determine which files must be compiled first. Once the order is established, it compiles Module Interface files into a binary format called a Binary Module Interface (BMI), which uses file extensions like `.gcm` (GCC) or `.ifc` (MSVC).
2. **Why It Happens**
   Traditional `#include` directives cause the same header files to be re-parsed hundreds of times across different source files, severely slowing down compilation. C++20 modules solve this by compiling an interface once into a fast-to-read binary format. However, because modules can import other modules, the compiler cannot guess the build order. It must map out the entire network of module relationships before compiling any individual file.
3. **Impacted Keywords**
   * `module`: Declares that a source file belongs to a specific module or defines the start of a module interface.
   * `export`: Marks a specific function, class, or namespace within a module interface as visible to any external code that imports the module.
   * `import`: Instructs the compiler to load a pre-compiled Binary Module Interface, making all exported symbols from that module available immediately without text copy-pasting.

4. **Direct Modifications Made to the Code**
  * Dependency Graph Generation: The scanner produces an internal dependency map (often structured as a JSON file) detailing the precise compilation topology.
  * Binary Interface Creation: The compiler takes the exported code from module interface files and translates it into a cached, structured binary file (`.gcm`/`.ifc`).
  * Token Isolation: Unlike headers, the symbols imported via import do not bleed into the rest of the file textually; macro definitions from the importing file do not affect the imported module, and macros inside the module do not leak out.

5. **View Output**
  **How to Force the Output Path of Binary Module Interfaces**
  Because C++20 modules generate a Binary Module Interface (BMI), different compilers use unique flags to dictate exactly where these binary artifacts (`.gcm` or `.ifc`) are saved and how they are located during subsequent compilation steps.

  * GCC defaults to creating a hidden directory named `gcm.cache` in the current working directory. To alter this behavior and specify an exact path:
  * To output/save to a specific path: Use the `-fmodules-ts` flag along with `-fmodule-mapper`
      ```sh
          g++ -fmodules-ts -fmodule-header -c MyModule.ixx -o MyModule.o
      ```
  * To completely control the cache path, pass a path mapping string directly to the module mapper:
    ```sh
          g++ -fmodules-ts -fmodule-mapper=|backend-path-or-file -c MyModule.ixx
    ```

## Practical Example

### 1. The Module Interface File
This file defines the module and explicitly exports what we want the outside world to see.
```cpp
// Math.cppm or Math.ixx
// Declares that this file is the primary module interface for 'Math'
export module Math; 

// This function is hidden inside the module; it cannot be seen by main.cpp
int internalHelper(int a, int b) {
    return a + b;
}

// 'export' makes this visible to anyone who imports 'Math'
export int add(int a, int b) {
    return internalHelper(a, b);
}
```
* File name can be anything not necessarly `Math.cppm` or `Math.ixx`
* The name of the module is determined strictly by the `export module <name>;`
* No two modules contains same name, For massive project it will be hard to maintain so c++ allows to use `.`. The dot doesn't actually create folders, it is used to group modules logically so names don't collide. `export module GameEngine.Math;`

// what happens two different modules have same function name

### 2. The Main Program (main.cpp)
This file imports the module. Thanks to Token Isolation, nothing from main.cpp can mess up Math, and vice versa.
```cpp
// main.cpp
#include <iostream>

// Instructs the compiler to load the Binary Module Interface (BMI)
import Math; 

int main() {
    // Works perfectly because 'add' was exported
    std::cout << "5 + 3 = " << add(5, 3) << std::endl; 
    
    // ERROR: This would fail to compile because 'internalHelper' wasn't exported
    // internalHelper(5, 3); 
    
    return 0;
}
```

### Toolchain Processes
1. Scan & Compile the Module Interface (Generates the BMI)
   First, the compiler must process Math.ixx to create that .gcm binary file you mentioned.
   ```bash
   g++ -std=c++20 -fmodules-ts -c Math.ixx -o Math.o
   ```
   This generates Math.o (the object code) AND creates a local gcm.cache/ directory containing the BMI for the Math module.

```
export module Math; // The primary module name

export import :Geometry; // Bring in the pieces and export them to the user
export import :Algebra;
```

```
export module Math:Algebra;  // Another piece of 'Math'
export int square(int x) { return x * x; }
```

## C++20 Module System: Comprehensive Documentation

---

## 1. Fundamentals of Code Isolation (The "Why")


### The C++20 Module Solution

C++20 introduced modules to replace textual inclusion with a structural component model. Instead of parsing text repeatedly, a module is compiled exactly once into a pre-parsed, structured binary format.

#### Mechanistic Differences: Text vs. Binary Interfaces

The following table contrasts the two compilation models across core operational vectors:

| Evaluation Vector | Legacy Header Model (`#include`) | Modern Module Model (`import`) |
| --- | --- | --- |
| **Ingestion Method** | Physical text copy-pasting by the preprocessor. | Logical loading of a pre-parsed binary symbol table. |
| **Parsing Frequency** | Parsed once per source file where it appears. | Parsed exactly once globally, then cached. |
| **Macro Boundary** | Macros leak across files, risking namespace collision. | Complete isolation; macros do not leak in or out. |
| **Compilation Scaling** | O(N) where N is the total volume of duplicated text. | O(1) for importing the pre-compiled binary interface. |

---

## 2. Independent Syntax Bricks

A module interface is built from three core independent keywords: `module`, `export`, and `import`. Their behavior is structurally rigidly defined.

### The `module` Keyword

The `module` keyword establishes the identity of a compilation unit. It tells the compiler that the file is part of a named structural block rather than a traditional, global source file.

#### Global Module Fragment

1. **The Core Rule of Modules**
    In modern C++,  never allowed to use `#include` inside a module.
    ```cpp
    #include <cstdio> // CRASH! This is illegal in C++20.
    export module MyModule;
    #include <cstdio> // CRASH! This is illegal in C++20.
    ```

2. **2. The Problem**
    Sometimes, programmer has to use an old legacy header file (like <cstdio>) because code needs it to work. But because of Rule #1 `#include` can't just stick it inside the module.

3. **The Solution:**
    To fix this, C++ forces to build an isolated "airlock" at the very top of your file.
    * The line module; opens the airlock.
    * The line export module CoreUtility; closes the airlock.
    
    ```cpp
    module;                   // 1. OPEN THE AIRLOCK.
    #include <cstdio>         // 2. Put the dusty old header inside the airlock.

    export module CoreUtility; // 3. CLOSE THE AIRLOCK. (clean module starts here)

    // 4. Use it safely:
    export void sayHello() {
        std::printf("Hello!"); // Works! module can see inside the airlock.
    }
    ```

5. **Why did we do this?**
    When the compiler builds this file, it lets our module use `std::printf`. But the moment it finishes compiling, it completely deletes `<cstdio>` from memory. When another file imports our module (import CoreUtility;), they only get clean sayHello() function. The old `<cstdio>` header is completely gone and cannot leak out to pollute the rest of our program.

A module interface file must occasionally include legacy header files that do not support modules. To prevent these headers from polluting the module, they must be placed in the **Global Module Fragment**. This zone opens with an unnamed `module;` directive and closes when the named module is declared.

### The `export` Keyword

The `export` keyword acts as an access control gatekeeper. By default, every function, class, variable, or namespace defined inside a module is strictly private to that module. Applying `export` makes the symbol visible to external files that import the module.

#### Permitted Target Syntax

The `export` keyword can be placed directly in front of variable declarations, functions, classes, templates, or entire namespace blocks.

```cpp
export module Inventory;

export int maximumItems = 100; // Exporting a variable

export void calculateStock() {} // Exporting a function

export class ItemCarrier {}; // Exporting a class

export namespace DataMetrics { // Exporting an entire namespace block
    int track() { return 0; }
    void reset() {}
}

```

### The `import` Keyword

The `import` keyword tells the compiler to look up a pre-compiled module binary by its logical name, loading its exported symbols instantly into the current file.

```cpp
import Inventory; // Loads the binary symbol table of the Inventory module

int main() {
    calculateStock(); // Accessible due to 'export' in the module
}

```

---

## 3. Dependency Mapping & The Translation Pipeline

Because modules can import other modules, the compiler cannot guess the order in which files must be compiled. A strict multi-step sequence must occur to translate code containing modules.

### Phase 1: Module Scanning

Before any compilation occurs, a scanning tool reads all source files. It does not compile the code; it only searches for the keywords `export module` and `import`.

#### The Dependency Topology (JSON Generation)

The scanner records which file defines which module, and which files depend on those modules. It outputs an internal map—frequently formatted as a P1689 standard JSON file—detailing the exact topological sorting order required for compilation.

### Phase 2: Compilation of Module Interfaces

Using the dependency map, the compiler identifies independent module interfaces (modules that do not import anything else) and compiles them first.

#### Generation of Binary Module Interfaces (BMI)

Compiling a module interface yields two distinct artifacts:

1. **Object File (`.o` or `.obj`):** Contains the compiled machine code instructions for execution.
2. **Binary Module Interface (BMI):** A highly optimized, tokenized representation of the exported code structure. Different compilers output different file extensions for this binary format:
   * **GCC:** Outputs `.gcm` files.
   * **MSVC:** Outputs `.ifc` files.
   * **Clang:** Outputs `.pcm` files.


### Phase 3: Compilation of Consumer Code

Once the BMIs are generated, the consumer files (like `main.cpp`) are compiled. When the compiler encounters an `import` statement, it pauses, reads the corresponding `.gcm` or `.ifc` file directly from the disk cache, populates its internal symbol table, and completes compilation of the consumer object file.

### Phase 4: Linking

The linker combines all generated object files (`.o` / `.obj`) into a single executable binary, resolving the physical memory addresses of the functions called across the module boundaries.

---

## 4. Module Namespaces & Architectural Layouts

As software projects grow, module names must be organized to prevent name collisions across independent code packages.

### Architectural Rule: Global Uniqueness

Every primary module name in a program must be completely unique. If two files independently declare `export module Core;`, the build system will fail during the scanning phase due to a duplicate module identifier.

### Hierarchical Naming via Dots (`.`)

To prevent collisions, C++ allows logical grouping using a dot separator (`.`). The dot has no physical meaning to the compiler—it does not create directories or paths on the hard drive—but it allows developer teams to structure names safely.

```cpp
// File A: Network system
export module CompanyName.System.Network;

// File B: Graphics engine
export module CompanyName.System.Graphics;

```

These are processed as two entirely distinct, independent BMIs.

### Module Partitions (The `:` Operator)

When a single module becomes too large for one file, it can be split into structural units called **partitions** using a colon (`:`). Partitions are invisible to the end consumer; they exist purely to manage file sizes internally.

#### Rules of Partitions

1. Partitions can only be imported by files belonging to the *same* module.
   ```cpp
   // Inside Math:Geometry file
    export module Math:Geometry; 

    import :Algebra; // Geometry belongs to Math, so it can import Algebra.

    export double doSomeGeometryMath() {
        return square(4); // Works perfectly
    }
   ```
   ```cpp
   // Inside main.cpp
    import Math:Algebra; // ERROR! main.cpp does NOT belong to the Math module.

    int main() {
        return 0;
    }
    ```
2. The primary module interface file must import and re-export all partitions to make them available to external users.

#### Implementation Pattern for Partitions

```
               +-----------------------+
               |  export module Math;  | <--- Primary Interface
               +-----------------------+
                /                     \
   export import :Algebra;     export import :Geometry;
              /                         \
+------------------------------+   +-------------------------------+
| export module Math:Algebra;  |   | export module Math:Geometry;  |
+------------------------------+   +-------------------------------+

```

##### Step 1: Create the Algebra Partition File

```cpp
export module Math:Algebra; // Syntax: <PrimaryModuleName>:<PartitionName>
// saying this module belongs to Math but specifically for algebra

export int square(int x) {
    return x * x;
}

```

##### Step 2: Create the Geometry Partition File

```cpp
export module Math:Geometry;
// saying this module belongs to Math but specifically for Geometry

export double calculateArea(double radius) {
    return 3.14159 * radius * radius;
}

```

##### Step 3: Create the Primary Module Interface File

This file shares the base module name and acts as the singular public interface, stitching the partitions together using `export import`.

```cpp
export module Math; // The public-facing module name

export import :Algebra;  // Imports and simultaneously re-exports partition
export import :Geometry; // Imports and simultaneously re-exports partition

```

```cpp
import Math; // Grabs the whole book

int main() {
    square(5);          // Works! (From the Algebra chapter)
    calculateArea(2.5); // Works! (From the Geometry chapter)
}
```
**All partitions are completely destroyed conceptually at the boundary of the module.** They cannot be exposed to the outside world as partitions. They can only be consumed by the outside world via the primary module interface.

### Ambiguity Error
```cpp
// File: Network.cppm
export module CompanyName.System.Network;
export void initialize() {
    // Network specific setup
}

// File: Graphics.cppm
export module CompanyName.System.Graphics;
export void initialize() {
    // Graphics specific setup
}

```

**The Consumer Application (`main.cpp`)**

```cpp
import CompanyName.System.Network;
import CompanyName.System.Graphics;

int main() {
    initialize(); // COMPILE ERROR: Call to 'initialize' is ambiguous
}
```
If no qualification is provided, the compiler **will not call either function**. It forces the developer to resolve the ambiguity manually.

#### The Solution: Architectural Integration with Namespaces
```cpp
export module CompanyName.System.Network;

export namespace Network {
    void initialize() {
        // Compiles as Network::initialize
    }
}

```

```cpp
export module CompanyName.System.Graphics;

export namespace Graphics {
    void initialize() {
        // Compiles as Graphics::initialize
    }
}

```

```cpp
import CompanyName.System.Network;
import CompanyName.System.Graphics;

int main() {
    // Explicitly calls the network initialization logic
    Network::initialize(); 
    
    // Explicitly calls the graphics initialization logic
    Graphics::initialize(); 
}
```
---

## 5. Toolchain Realization (CLI Compiling)

### GCC Toolchain Mechanics

#### File Extension Mapping Override (`-x c++`)

By default, GCC does not recognize custom module extensions like `.cppm` or `.ixx`. It treats them as unknown linker inputs and skips compilation entirely. To prevent this, the `-x c++` flag must be passed to explicitly force the compiler to evaluate the file as C++ source code.

#### Manual BMI Output Path Controls

GCC automatically stores generated `.gcm` files inside a hidden directory named `gcm.cache/` created in the current working directory.

##### Compiling the Module Interface Natively

```sh
g++ -std=c++20 -fmodules-ts -x c++ -c Math.cppm -o Math.o

```

* `-std=c++20`: Instructs the compiler to use the C++20 language standard standard.
* `-fmodules-ts`: Enables the module compilation engine. (required)
* `-x c++`: Forces the compiler to treat `Math.cppm` as a C++ source file.
* `-c`: Compiles the source file into an object file (`Math.o`) without running the linker.

##### Controlling Cache Output Directory Path via Module Mapper

To divert the BMI output away from the default local `gcm.cache/` path into a custom directory path (e.g., `/tmp/my_cache/`), the directory mapping syntax must be supplied directly to the compiler mapper flag:

```sh
g++ -std=c++20 -fmodules-ts -fmodule-to-cmap=/tmp/my_cache/ -x c++ -c Math.cppm -o Math.o

```