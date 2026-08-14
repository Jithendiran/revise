# Enumerations

## The Problem — Magic Numbers and Name Collisions

Without enumerations, a set of related named constants must be represented as plain integers or preprocessor macros:

`````cpp
// Magic numbers — meaning is unclear at the call site
void setDirection(int d);
setDirection(2);   // what does 2 mean? north? east? up?

// Macros — no type safety, pollute global namespace
#define RED   0
#define GREEN 1
#define BLUE  2

// Name collision:
#define RED 0        // from colors.h
#define RED 255      // from error_codes.h — redefinition, silent overwrite
`````

Three problems:

1. **No type safety.** Any integer can be passed where a direction or color is expected. The compiler cannot catch `setDirection(999)`.

2. **Name pollution.** Macro names and plain `const int` names live in the global namespace. Every name must be unique across the entire program. `RED`, `GREEN`, `BLUE` in one library collides with the samenames in another.
    * Example 1 
        ```cpp
        #include <iostream>
        #define RED 0
        #define GREEN 1

        int RED = 0;         // Global scope
        const int GREEN = 1; // File scope
        ```
        issue here is `int RED = 0;` will become `int 0 = 0;`
    * Example 2
        ```cpp
        // file: theme.cpp
        int RED = 1;

        // file: alarms.cpp
        int RED = 2; // Linker error: "redefinition of 'RED'"
        ``` 


3. **No debugger visibility.** A debugger shows `2` — not `NORTH` or `BLUE`. The semantic meaning is invisible at runtime.

Enumerations solve all three problems.

## Unscoped Enum — `enum` (C-style, legacy)

The original C-style enumeration. Enumerators are injected directly into the enclosing scope — they leak out of the enum.

```cpp
enum Direction {
    North,   // = 0 (first enumerator defaults to 0)
    East,    // = 1
    South,   // = 2
    West     // = 3
};

Direction d = North;   // VALID: North is in the enclosing scope
int x = North;         // VALID: implicit conversion to int — leaks out
```

### Enumerator Leakage — The Core Problem

```cpp
enum Color  { Red, Green, Blue };
enum Status { Red, Failed };     // COMPILE ERROR: 'Red' already defined
                                  // both enums inject Red into the same scope
```

In C-style enums, all enumerator names go into the surrounding namespace. Two enums with the same enumerator name in the same scope conflict.

### Implicit Conversion to `int`

`````cpp
enum Direction { North, East, South, West };

int x = North;        // VALID: North silently converts to 0
Direction d = 2;      // COMPILE ERROR: int does not implicitly convert back
Direction d = static_cast<Direction>(2);   // VALID: explicit cast required
`````

**issues**
```cpp
enum Direction { North, East, South, West };
enum Color { Red, Green, Blue };

Direction d = North;
Color c = Red;

// The compiler allows this! You are adding a Direction to a Color.
int result = d + c; 

// that was meant to take a raw index or a completely different value.
// since Direction is also a int, compiler allows
void setIndex(int index);
setIndex(North); // Compiles fine, even though North is a direction, not an index.

// A developer reads data from a file or network packet
int rawValue = 999; 

// They force it into the enum type
Direction d = static_cast<Direction>(rawValue);
// 999 is not a valid direction (0 to 3 are). By forcing the integer into the enum, you create an invalid enum state that the compiler can no longer protect you against, leading to crashes or undefined behavior when your code tries to handle a "direction" that doesn't exist.
```

### Underlying Type — Unspecified

The underlying integer type of a plain `enum` is implementation-defined. The compiler chooses `int`, `unsigned int`, or another type based on the range of values. This can cause ABI (Application Binary Interface) issues across compilers.

> Application Programming Interface defines how one source code talks to other source code (what function names and types to write), an ABI defines how compiled machine code talks to other compiled machine code. It dictates the low-level details of how data is laid out in computer memory and how functions pass data to each other at the binary level.

### Explicit Enumerator Values

Any enumerator can be given an explicit value. Subsequent enumerators continue from that value:

```cpp
enum HttpStatus {
    OK          = 200,
    NotFound    = 404,
    ServerError = 500
};

enum Flags {
    None  = 0,
    Read  = 1,       // 0b0001
    Write = 2,       // 0b0010
    Exec  = 4        // 0b0100
};
```

## Scoped Enum — `enum class` (Modern C++, C++11)

`enum class` (also `enum struct` — identical) solves both problems of the unscoped enum: names are scoped to the enum and implicit conversion to `int` is blocked.

```cpp
enum class Direction {
    North,
    East,
    South,
    West
};

Direction d = Direction::North;   // REQUIRED: must qualify with enum name
// Direction d = North;           // COMPILE ERROR: North not in scope
// int x = Direction::North;      // COMPILE ERROR: no implicit conversion
int x = static_cast<int>(Direction::North);   // VALID: explicit only

enum class Color { Red, Green, Blue };

Color c = Color::Red;
// int x = c;                    // COMPILE ERROR: no implicit conversion
int x = static_cast<int>(c);     // VALID: explicit cast
// if (c == 0) { }               // COMPILE ERROR: cannot compare Color to int
if (c == Color::Red) { }         // VALID: compare same enum type
```

### No Leakage — Names Are Scoped

```cpp
enum class Color  { Red, Green, Blue };
enum class Status { Red, Failed };   // VALID: Status::Red and Color::Red
                                      // are different names — no conflict
```

Each enumerator belongs to its enum's scope. `Color::Red` and `Status::Red` are completely separate names.

### Specified Underlying Type

```cpp
enum class Color : unsigned char {   // underlying type explicitly specified
    Red,     // = 0
    Green,   // = 1
    Blue     // = 2
};

sizeof(Color);   // guaranteed 1 — unsigned char is 1 byte
```

The underlying type determines:
- The size of the enum in memory
- The range of valid values
- The ABI — useful when the enum is stored in a file, sent over a network, or shared between compilers

**Valid underlying types:** Any integral type — `char`, `unsigned char`, `short`, `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`.

## Unscoped Enum With Specified Underlying Type (C++11)

A plain `enum` can also have an explicitly specified underlying type without becoming an `enum class`. Enumerators still leak into the enclosing scope, but the size and range are now well-defined:

```cpp
enum Direction : uint8_t {
    North = 0,
    East  = 1,
    South = 2,
    West  = 3
};

sizeof(Direction);   // 1 — uint8_t guaranteed
int x = North;       // VALID: still implicitly converts to int (still leaks)
```

This form is useful when interfacing with C code or binary protocols that require specific sizes but cannot use `enum class`.

## Forward Declaration of Enums (C++11)

An enum can be declared without its enumerators when the underlying type is specified. The full definition can appear later.

`````cpp
enum class Status : int;   // forward declaration — underlying type required

void process(Status s);    // function can use Status before it is defined

enum class Status : int {  // full definition — must match underlying type
    OK      = 0,
    Error   = 1,
    Pending = 2
};
`````

Forward declaration is useful when two headers need to reference each other's enum types — breaks circular include dependencies.

An unscoped `enum` without a specified underlying type cannot be forward-declared — the compiler cannot know its size without seeing all values.

## `using enum` — Importing Enumerators (C++20)

C++20 allows importing all enumerators of an `enum class` into the current scope using `using enum`. This brings the convenience of unscoped enums without their collision problem.

```cpp
enum class Color { Red, Green, Blue };

void paintRed() {
    using enum Color;        // import all enumerators into this scope

    Color c = Red;           // VALID: Red is now directly accessible
    Color d = Blue;          // VALID: Blue too
    // Red is only accessible inside this scope — no global pollution
}

// Color::Red still works everywhere:
Color outside = Color::Red;  // VALID
// Red outside = Red;        // COMPILE ERROR: Red not in this scope
```

Individual enumerators can also be imported:

```cpp
using Color::Red;    // import only Red
Color c = Red;       // VALID
// Color d = Blue;   // COMPILE ERROR: Blue not imported
```

## Enum Operations and Patterns

### Switching on an Enum

```cpp
enum class Direction { North, East, South, West };
// enum class Direction { North, East, South, West, Centre }; // will cause warning or error based on option

void move(Direction d) {
    switch (d) {
        case Direction::North: moveUp();    break;
        case Direction::East:  moveRight(); break;
        case Direction::South: moveDown();  break;
        case Direction::West:  moveLeft();  break;
    }
}
```

If a new enumerator is added to `Direction`, the compiler warns that the switch is not exhaustive — a built-in protection against forgetting to handle new cases. 
**Compile Options**
- `-Wswitch`            : Warning only 
- `-Werror -Wswitch`    : Error

### Enum as Bitmask (Unscoped or with Helper)

Unscoped enums with power-of-two values work as bitmasks naturally:

```cpp
enum Permission {
    None    = 0,
    Read    = 1 << 0,   // 0b0001 = 1
    Write   = 1 << 1,   // 0b0010 = 2
    Execute = 1 << 2    // 0b0100 = 4
};

int perms = Read | Write;          // combine: 0b0011 = 3
bool canRead = (perms & Read) != 0;  // test: true
```

`enum class` blocks the implicit conversion needed for bitwise operations. Overloading the operators makes it work:

```cpp
enum class Permission : unsigned int {
    None    = 0,
    Read    = 1 << 0,
    Write   = 1 << 1,
    Execute = 1 << 2
};

Permission operator|(Permission a, Permission b) {
    return static_cast<Permission>(
        static_cast<unsigned int>(a) | static_cast<unsigned int>(b)
    );
}

Permission operator&(Permission a, Permission b) {
    return static_cast<Permission>(
        static_cast<unsigned int>(a) & static_cast<unsigned int>(b)
    );
}

Permission perms = Permission::Read | Permission::Write;
bool canRead = (perms & Permission::Read) != Permission::None;
```

### Enum to String (Manual)

The language provides no built-in enum-to-string conversion. The standard approach is a helper function:

```cpp
enum class Color { Red, Green, Blue };

const char* toString(Color c) {
    switch (c) {
        case Color::Red:   return "Red";
        case Color::Green: return "Green";
        case Color::Blue:  return "Blue";
        default:           return "Unknown";
    }
}

std::cout << toString(Color::Green);   // "Green"
```

### Iteration Over Enumerators

Enums have no built-in iteration. The common pattern adds a sentinel:

```cpp
enum class Direction {
    North,
    East,
    South,
    West,
    COUNT   // sentinel — total number of enumerators
};

for (int i = 0; i < static_cast<int>(Direction::COUNT); i++) {
    Direction d = static_cast<Direction>(i);
    // process each direction
}
```

## `constexpr` and `constinit` with Enums

Enum values are implicitly `constexpr` — they are compile-time constants:

```cpp
enum class Color : int { Red = 0, Green = 1, Blue = 2 };

constexpr Color defaultColor = Color::Green;   // VALID
static_assert(static_cast<int>(Color::Blue) == 2);  // VALID: compile-time check

constexpr int colorCount = 3;
std::array<int, colorCount> colorValues;   // VALID: used as template argument
```