# C++ Mastery — From C to Production-Ready C++

A structured self-study repository tracking the progression from procedural C programming to high-performance, modern C++ (C++20/C++23).


---

## Repository Structure

Maintain a clean, deterministic file hierarchy to isolate concepts, verify assembly outputs, and log defensive programming edge cases.

```
cpp-mastery/
│
├── README.md                           <- Master curriculum tracker
│
├── phase-01-c-to-cpp-basics/
├── phase-02-infrastructure-tooling/
├── phase-03-oop-fundamentals/
├── phase-04-memory-layout-raii/
├── phase-05-move-semantics/
├── phase-06-polymorphism-internals/
├── phase-07-templates-fundamentals/
├── phase-08-perfect-forwarding/
├── phase-09-stl-containers-iterators/
├── phase-10-closures-functional/
├── phase-11-error-handling-resilience/
├── phase-12-compile-time-programming/
├── phase-13-advanced-metaprogramming/
├── phase-14-concurrency-multithreading/
├── phase-15-architecture-design-patterns/
├── phase-16-low-level-hardware/
├── phase-17-modern-extensions/
└── phase-18-portfolio-milestones/
```

### Individual Sub-topic Directory Blueprint

Each individual sub-topic directory must contain a consistent three-file pattern to ensure comprehensive conceptual mapping and operational testing:

```
phase-XX-topic-name/X.X-subtopic/
├── notes.md         <- Conceptual explanations, architectural notes, and internal designs
├── main.cpp         <- Self-contained executable demonstrating minimal deterministic mechanics
└── gotchas.md       <- Enumerate compiler edge cases, undefined behaviors, and deviations from C assumptions
```

---

## Complete Brick-by-Brick Curriculum

### Phase 1 — From C to C++ Foundations
Leverage existing knowledge of C to introduce core semantic adjustments and compile-time features without entering object-oriented design.
- [ ] 1.1 — C Limitations, Safety Vulnerabilities, and C++ Strategic Design Goals
- [ ] 1.2 — The Modern Compilation Model: Translation Units, Header Resolution, and Introduction to Modules
- [ ] 1.3 — Namespaces, Argument-Dependent Lookup (ADL), and Scope Resolution Operators
- [ ] 1.4 — The Native bool Type and Logical Semantics
- [ ] 1.5 — Uniform Initialization, Direct vs. Copy Initialization, and Narrowing Conversion Defenses
- [ ] 1.6 — Lvalue References (&) vs. Raw Pointers: Syntactic Sugar, Aliasing, and Nullability Constraints
- [ ] 1.7 — Function Overloading Mechanics, Name Mangling, and Linkage Specifications
- [ ] 1.8 — Default Arguments, Declaration vs. Definition Splits, and Evaluation Order
- [ ] 1.9 — Explicit Type Casting: Evolution from C-Style Casts to static_cast, reinterpret_cast, and const_cast
- [ ] 1.10 — Type Inference: auto Deduction Rules, const/volatile Stripping, and Decay Mechanics
- [ ] 1.11 — Range-Based Loop Mechanics, Syntactic Desugaring, and Structural Iteration Basics
- [ ] 1.12 — The One Definition Rule (ODR) across Multiple Translation Units

### Phase 2 — Professional Infrastructure and Tooling
Establish a modern engineering toolchain early to build, debug, profile, and verify every piece of code written across subsequent phases.
- [ ] 2.1 — Modern CMake: Target-Based Architecture, Properties, Visibility, and Dependency Linking
- [ ] 2.2 — GDB Debugging Foundations: Breakpoints, Stack Inspections, Variable Watching, and Core Dumps
- [ ] 2.3 — Compiler Explorer (godbolt.org): Verifying Optimization Levels and Analyzing Assembly Output
- [ ] 2.4 — Runtime Sanitizers: ThreadSanitizer (TSan), AddressSanitizer (ASan), and UndefinedBehaviorSanitizer (UBSan)
- [ ] 2.5 — Static Analysis Automation: Integrating clang-tidy and cppcheck into the Build Pipeline
- [ ] 2.6 — Unit Verification: Writing Deterministic Test Suites with Google Test (gtest)
- [ ] 2.7 — Test-Driven Development (TDD) Workflow Paradigms in Compiled Languages

### Phase 3 — Core Object-Oriented Programming
Transition from plain procedural data structs to robust, encapsulated types with strictly managed lifecycles.
- [ ] 3.1 — Plain Old Data (POD) structs vs. Encapsulated class Semantics in C++
- [ ] 3.2 — Access Specifiers (public, private, protected) and the Enforcement of Invariants
- [ ] 3.3 — Object Lifecycle Management: Default, Parameterized, and Explicit Constructors along with Destructors
- [ ] 3.4 — The implicit execution context: 'this' Pointer Mechanics and std::addressof Safety
- [ ] 3.5 — Member Initialization Lists: Preventing Redundant Default Constructions and Handling References/Constants
- [ ] 3.6 — Value Copying Foundations: Deep Copy Constructors and Copy Assignment Operators
- [ ] 3.7 — Resource Duplication Control: Implementing the Rule of Three

### Phase 4 — Memory Layout and Resource Management (RAII)
Connect high-level object scopes directly to low-level hardware structures, allocation segments, and lifetime controls.
- [ ] 4.1 — Object Memory Layout: Struct Alignment, Padding Mechanics, Data Member Reordering, and sizeof Evaluation
- [ ] 4.2 — Dynamic Memory Subsystems: Heap Management via new/delete vs. Standard C malloc/free
- [ ] 4.3 — Resource Acquisition Is Initialization (RAII): Binding System Resource Lifetimes to Stack Frame Allocation
- [ ] 4.4 — Const Correctness: Logical vs. Physical Constness, const Methods, const Pointers, and the mutable Keyword
- [ ] 4.5 — Memory Segment Lifetimes: Static, Thread-Local, Stack, and Dynamic Heap Storage Classes

### Phase 5 — Value Categories and Move Semantics
Resolve the performance bottlenecks associated with resource duplication by defining data transfer semantics.
- [ ] 5.1 — Deep Taxonomy of Value Categories: Lvalues, Rvalues, Glvalues, Prvalues, and Xvalues
- [ ] 5.2 — Rvalue References (&&): Binding Conditions, Semantic Intent, and Overload Resolution
- [ ] 5.3 — Resource Theft Mechanics: Designing Move Constructors and Move Assignment Operators
- [ ] 5.4 — std::move Exposed: Explicit Rvalue Casting, Destruction of Moved-From States, and Misuse Mitigation
- [ ] 5.5 — Unified Resource Management Lifecycle: The Rule of Five and the Modern Rule of Zero
- [ ] 5.6 — Compiler Optimization Overrides: Named/Unnamed Return Value Optimization (RVO / NRVO) and Copy Elision

### Phase 6 — Inheritance and Runtime Polymorphism
Introduce hierarchical code reuse, abstract behavior definitions, and decouple client code from concrete implementations while profiling costs.
- [ ] 6.1 — Base and Derived Class Hierarchies, Memory Layouts, and Construction/Destruction Sequences
- [ ] 6.2 — Virtual Functions and Dynamic Binding: Overriding vs. Hiding Behaviors
- [ ] 6.3 — Under the Hood: Virtual Tables (Vtables), Virtual Table Pointers (vptrs), and Dispatch Overhead Analytics
- [ ] 6.4 — Upcasting, Downcasting, and the Object Slicing Defect: Mechanical Breakdowns
- [ ] 6.5 — Interface Definitions: Pure Virtual Functions, Abstract Base Classes, and Implementation Inversion
- [ ] 6.6 — Context Enforcement: Strategic Deployment of override and final Keywords
- [ ] 6.7 — Memory Leak Preventions: The Absolute Necessity of Virtual Destructors in Polymorphic Hierarchies
- [ ] 6.8 — Complex Topologies: Multiple Inheritance, Object Layout Offsets, and Resolving the Diamond Problem via Virtual Base Classes

### Phase 7 — Templates and Compile-Time Foundations
Shift type definitions and optimization steps from runtime execution to compile-time parametric generation.
- [ ] 7.1 — Function Templates: Parameter Instantiation, Argument Deduction Rules, and Type Substitution
- [ ] 7.2 — Class Templates: Layout Structuring, Multi-Type Generics, and Out-of-Line Method Definitions
- [ ] 7.3 — Specialization Architecture: Total and Partial Template Specializations for Type Tuning
- [ ] 7.4 — Non-Type Template Parameters (NTTP): Passing Integers, Pointers, and Literals as Compile-Time Arguments
- [ ] 7.5 — Metaprogramming Scale: Variadic Templates, Parameter Packs, and Fold Expressions
- [ ] 7.6 — Compile-Time Assertions: Validating Type Assumptions and Constraints via static_assert

### Phase 8 — Advanced Move Semantics and Perfect Forwarding
Synthesize templates with move operations to design zero-overhead generic parameter transport pipelines.
- [ ] 8.1 — Universal References / Forwarding References (T&& in Deduced Contexts) vs. Standard Rvalue References
- [ ] 8.2 — Reference Collapsing Rules: Quantifying & + &, & + &&, && + &, and && + && Interactions
- [ ] 8.3 — std::forward Demystified: Conditional Rvalue/Lvalue Casting for Generic Argument Preservation
- [ ] 8.4 — Advanced Application: Designing Forwarding Constructors and Avoiding Overload Overlaps

### Phase 9 — The Standard Template Library (STL) and Safe I/O
Utilize standardized high-performance data structures and continuous memory layouts now that structural mechanics are established.
- [ ] 9.1 — Safe Text I/O: Migrating from Legacy iostreams to Type-Safe std::format (C++20) and Zero-Overhead std::print (C++23)
- [ ] 9.2 — Contiguous and Node-Based Sequential Storage: Deep Dive into std::vector, std::array, and std::deque
- [ ] 9.3 — Sorted and Hashed Associative Subsystems: Internals of std::map, std::set, and Lock-Free/Hashed std::unordered_map
- [ ] 9.4 — Iterator Mechanics: Taxonomy (Random Access, Forward, Bidirectional), Category Traversal, and Invalidation Safety
- [ ] 9.5 — High-Performance STL Algorithms: Execution Policies, Range-Based Operations (sort, find, transform, accumulate)
- [ ] 9.6 — Optimized String Subsystems: std::string Allocation Strategies vs. Non-Owning View Slices via std::string_view

### Phase 10 — Closures and Functional Programming
Embed local execution blocks and handle functional tracking entities through local class compilation models.
- [ ] 10.1 — Lambda Expressions Unmasked: The Compiler-Generated Anonymous Functor Object (Closure Type)
- [ ] 10.2 — State Capturing Semantics: Value [=] vs. Reference [&] Side Effects and C++14 Init-Captures for Movable Types
- [ ] 10.3 — Polymorphic Closures: Generic Lambdas (auto Parameters) and Explicit Template Lambda Expressions (C++20)
- [ ] 10.4 — Polymorphic Function Wrappers: Evaluating the Flexibility vs. Runtime Heap/Indirection Overhead of std::function
- [ ] 10.5 — High-Order Architectural Design: Passing, Returning, and Stacking Lambda Expressions

### Phase 11 — Error Handling and Resilience
Develop strict strategies for handling operational failures and enforce predictable recovery paths without sacrificing runtime efficiency.
- [ ] 11.1 — Exception Propagation Mechanics: Stack Unwinding, Object Cleanup Routines, and Cost Realities of throw/catch
- [ ] 11.2 — Exception Safety Classifications: Designing for Basic, Strong, and Nothrow Structural Guarantees
- [ ] 11.3 — The noexcept Attribute: Enforcing Compile-Time Guarantees, Move Optimizations, and Termination Triggers
- [ ] 11.4 — Non-Exceptional Optionals: Managing Partial Returns and Optional States using std::optional (C++17)
- [ ] 11.5 — Functional Failure Chains: Modern Monadic Error Propagation via std::expected (C++23)

### Phase 12 — Compile-Time Programming and Concepts
Harness the compiler as an execution engine to guarantee invariants, evaluate metrics, and construct clean APIs before binary creation.
- [ ] 12.1 — The constexpr Keyword Deep Dive: Execution Context Limits, Constant Expressions, and Compile-Time Struct Validation
- [ ] 12.2 — Compile-Time Constraints: Requiring Literal Evaluation via consteval and Initialization Safety via constinit (C++20)
- [ ] 12.3 — Structural Selection: Evaluating Code Branched at Compile Time using if constexpr Conditional Evaluations
- [ ] 12.4 — Standard Type Introspection: Utilizing Standard Library Meta-Functions inside Header <type_traits>
- [ ] 12.5 — Formal Constraints and Concepts (C++20): Replacing Unstructured Templates with Predicate-Based Interface Overloads

### Phase 13 — Advanced Metaprogramming and Idioms
Implement industry-standard design templates engineered to handle specialized object lifecycles and compile-time branching.
- [ ] 13.1 — Substitution Failure Is Not An Error (SFINAE): Constructing Type-Specific Code Paths via std::enable_if_t
- [ ] 13.2 — The Pointer to Implementation (PIMPL) Idiom: Eliminating Compilation Cascade Latencies and Hiding Private Layouts
- [ ] 13.3 — Non-Virtual Interface (NVI) Design Pattern: Separating Public API Entry Points from Custom Polymorphic Overrides
- [ ] 13.4 — Custom Runtime Type Erasure: Designing Polymorphic Behaviors without Vtables or Explicit Inheritance Hierarchies
- [ ] 13.5 — Tag Dispatching: Selecting Efficient Algorithm Implementations using Compile-Time Type Classifications
- [ ] 13.6 — Curiously Recurring Template Pattern (CRTP): Achieving Static Polymorphism and Mixin Code Injectors
- [ ] 13.7 — Standardized Heterogeneous Containers: Utilizing Type-Safe Alternatives like std::variant, std::any, and std::tuple

### Phase 14 — Concurrency and Multithreading
Transition from single-core deterministic execution blocks to concurrent, hardware-scheduled parallel memory mutation maps.
- [ ] 14.1 — Execution Management: Operating System Thread Abstractions using std::thread and Automatically Joining std::jthread (C++20)
- [ ] 14.2 — Shared Resource Isolation: Mutex Topologies, Deadlock Defenses, and RAII Locking via std::unique_lock and std::scoped_lock
- [ ] 14.3 — Thread Notification Infrastructures: Signaling State Transitions and Inter-Thread Coordination via std::condition_variable
- [ ] 14.4 — Asynchronous Task Pipelines: Decentralized Computations using std::async, Data Communication via std::future/std::promise
- [ ] 14.5 — Low-Level Hardware Atomic Operations: Interface Rules for std::atomic and the Mechanics of Lock-Free Computations
- [ ] 14.6 — The Formal C++ Memory Model: Quantifying std::memory_order (Relaxed, Acquire, Release, SeqCst) and Happens-Before Chains
- [ ] 14.7 — Concurrency Vector Defects: Eliminating Data Races, Dynamic Deadlocks, Priority Inversions, and Hardware False Sharing

### Phase 15 — Architecture and Design Principles
Integrate structural software engineering principles directly into the design of large-scale, enterprise-ready C++ architectures.
- [ ] 15.1 — Abstract SOLID Principles Translated to Idiomatic C++ Language Topologies
- [ ] 15.2 — Component Design: Constructing Lightweight Frameworks prioritizing Composition over Rigidity-Prone Inheritance
- [ ] 15.3 — Classic Design Micro-Patterns Refined: Type-Safe Singleton, Factory Methods, Strategy Inversion, and Observers

### Phase 16 — Advanced Low-Level Systems and Hardware Optimization
Bridge the software-hardware gap by optimizing cache interactions, handling allocation mechanisms, and debugging undefined behavior.
- [ ] 16.1 — High-Performance Custom Allocation Systems: Architectural Layouts for Arena, Fixed-Pool, and Stack-Based Allocators
- [ ] 16.2 — Binary ABI Controls: Suppressing C++ Symbol Mangling, Name Linkages, and Calling Standards via extern "C"
- [ ] 16.3 — Cache Line Friendliness: Data-Oriented Design Frameworks comparing Structure of Arrays (SoA) vs. Array of Structures (AoS)
- [ ] 16.4 — Small Buffer Optimization (SBO) Mechanics: Eliminating Heap Triggers via Local Internal Array Buffers
- [ ] 16.5 — Low-Level Traps: System volatile (Hardware I/O Indicator) vs. Multithreaded std::atomic (Memory Order Vector)
- [ ] 16.6 — Profiling Infrastructure: Pinpointing Latencies and Resource Leaks with perf, Valgrind, Flame Graphs, and Google Benchmark
- [ ] 16.7 — Undefined Behavior (UB) Deep Dive: Structural Audits for Use-After-Free, Signed Overflow, and Dangling Pointer Violations

### Phase 17 — Modern Extensions (C++20 / C++23)
Leverage the latest structural updates designed to change how modern enterprise scale systems are compiled and processed.
- [ ] 17.1 — Standard Attribute Annotations: Enforcing Direct Intent via [[nodiscard]], [[maybe_unused]], and [[fallthrough]]
- [ ] 17.2 — Structural Unpacking: Initializing Compound Stack Variable Layouts utilizing Structured Bindings
- [ ] 17.3 — Lazy Pipeline Evaluation: Composing Zero-Allocation Functional Pipelines with Standard Ranges and Views (C++20)
- [ ] 17.4 — Stackless Asynchronous Operations: Evaluating Execution Halts and Cooperative Multi-Tasking via Coroutines (C++20)
- [ ] 17.5 — Compilation Subsystem Revolution: Eliminating Textual #include Copying using Standard C++ Modules (C++20)

---

## Phase 18 — Portfolio Milestones

These structural milestones require full synthesis of their prior respective theoretical phases. Build each component completely from scratch without importing standard library equivalents.

### Milestone 1: Custom unique_ptr
* Prerequisite Phases: Phase 3, Phase 4
- [ ] Build a self-contained smart pointer enforcing unique ownership semantics.
- [ ] Integrate custom deleter function objects via template parameter passing.
- [ ] Suppress copy operations while allowing explicit move construction and assignments.

### Milestone 2: Custom shared_ptr and weak_ptr
* Prerequisite Phases: Phase 3, Phase 4, Phase 6
- [ ] Implement a central heap-allocated control block holding individual instance reference counts and weak monitors.
- [ ] Enforce exception-safe atomic counter mutations for execution safety across separate threads.
- [ ] Design cyclic reference mitigation using weak pointer tracking blocks.

### Milestone 3: Custom std::vector
* Prerequisite Phases: Phase 4, Phase 5, Phase 7
- [ ] Manage raw heap allocations using direct storage allocations separate from element construction.
- [ ] Build geometric reallocations (growth factor of 1.5x/2x) that utilize move semantic optimizations if the type is noexcept.
- [ ] Enforce perfect forwarding construction blocks inside an emplace_back API template.

### Milestone 4: Custom std::string with Small Buffer Optimization (SBO)
* Prerequisite Phases: Phase 4, Phase 5, Phase 16
- [ ] Design a stack-allocated internal byte array buffer (e.g., 15 or 23 bytes) that shares space with heap address tracking fields via a union.
- [ ] Implement short character arrays within the local stack to bypass heap allocations.
- [ ] Ensure automatic, transparent migration to dynamic heap allocations when text boundaries exceed internal thresholds.

### Milestone 5: Custom std::function
* Prerequisite Phases: Phase 7, Phase 10, Phase 13
- [ ] Implement a type-safe generic container capable of storing and executing arbitrary callable entities (lambdas, function pointers, functors).
- [ ] Handle internal data capture requirements by building custom type erasure wrappers around a base class template.
- [ ] Apply small buffer optimization techniques to minimize heap allocations for small closures.

### Milestone 6: Automated Thread Pool
* Prerequisite Phases: Phase 14
- [ ] Instantiate a fixed array of background worker loops using std::jthread containers.
- [ ] Coordinate task scheduling across worker loops using a thread-safe task queue guarded by std::condition_variable.
- [ ] Leverage type-erased task objects and futures to return calculated asynchronous results to calling contexts.

### Milestone 7: Low-Latency Memory Ring Buffer / High-Frequency Event Loop
* Prerequisite Phases: Phase 14, Phase 16
- [ ] Construct a lock-free, circular array template for Single-Producer Single-Consumer (SPSC) execution flows.
- [ ] Align atomic indices to hardware cache line thresholds (e.g., std::hardware_destructive_interference_size) to prevent false sharing.
- [ ] Build a tight event loop that utilizes continuous non-blocking polling loops to minimize operating system context switching overhead.


---

## Technical Reference Library

* [cppreference.com](https://en.cppreference.com) — Standard C++ Reference Documentation
* [godbolt.org](https://godbolt.org) — Interactive Assembly Optimization Engine
* [isocpp.org](https://isocpp.org) — Official Standard C++ Foundation Portal
* [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) — Formal Design Rules by Bjarne Stroustrup and Herb Sutter

---

## Out of Scope Topics

For hardware-embedded systems, custom operating system kernels, or high-frequency automated trading applications, supplement this roadmap with independent studies in:
* Operating System Sockets: Bare-Metal POSIX Sockets, Linux epoll Multiplexing, and io_uring Asynchronous Architectures
* Kernel Scheduling Maps: Real-Time Threads, User-Space Context Switching, and System Call Trapping Latencies
* Non-Uniform Memory Access (NUMA): Topology Mapping, Thread Binding Core Affinities, and Inter-Node Bus Overhead Limits
* Hardware Vector Instructions: Streaming SIMD Extensions (SSE) and Advanced Vector Extensions (AVX-512) Intrinsics