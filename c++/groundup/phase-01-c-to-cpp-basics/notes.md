# 1.1 — C Limitations, Safety Vulnerabilities, and C++ Strategic Design Goals

### Core Weaknesses of Procedural C
1. **Lack of Type Safety:** `void*` acts as an escape hatch that silences compiler warnings, often leading to illegal memory interpretations at runtime.
2. **No Encapsulation:** Struct members are always public, Anyone and anything can look inside them and change the data.
3. **Macro Pollution:** The C Preprocessor (`#define`) performs literal text substitution. It obeys no scope rules, causes toxic side effects during evaluation, and ruins compiler error diagnostics.
4. **Manual Resource Lifecycle:** No native mechanisms exist to automatically bind a resource's lifetime (memory, file descriptors, locks) to its variable scope.

### C++ Strategic Design Enhancements
* **Strong Type System:** Stricter compile-time checks, type-safe casting operators, and the elimination of implicit `void*` conversions.
* **RAII (Resource Acquisition Is Initialization):** Objects manage resources via constructors and destructors. When an object goes out of scope, its destructor runs deterministically, preventing leaks.
* **Compile-Time Multi-Paradigms:** Introduction of templates, inline functions, and `constexpr` to move computation from runtime to compile time without performance penalties.