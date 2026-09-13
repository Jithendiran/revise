
# requires Expressions (C++20)

A `requires` expression tests whether expressions or types are valid for a given type. Returns true or false at compile time. The building block for concepts

## Two Different Things Share One Keyword

Before any example makes sense, one fact must be separated out: the word `requires` is used for **two grammatically different constructs** in C++20. They look similar and can even be nested inside each other, which is the root of most confusion.

```
requires-CLAUSE       — attaches a constraint to a template
                        does NOT have braces {}
                        example: template<typename T> requires Integral<T>

requires-EXPRESSION    — a value-producing expression that evaluates to
                        true or false at compile time
                        HAS braces {} containing a body
                        example: requires(T t) { t.size(); }
```

* A requires-**clause** is a gate — it decides whether a template participates in overload resolution.
* A requires-**expression** is a boolean-valued expression — it can be used *inside* a requires-clause, or anywhere a compile-time bool is needed (like inside a `concept` definition).



## The requires-Expression — Full Anatomy

```
requires (parameter-list) { requirement-sequence }
   ↑            ↑                    ↑
 keyword    optional            one or more
                                requirements,
                                each ending in ;
```

**The most important fact about a requires-expression:** nothing inside it ever executes. No object is constructed. No function is actually called. The compiler only checks whether the code **would compile** if it were written — it never runs it. The entire thing is evaluated purely at compile time and produces a single `bool`: `true` if every requirement inside is satisfied, `false` otherwise.

```cpp
requires(T t) { t.size(); }

requires(T t) { 5 > 3; }
```

This is a simple requirement. The only check: does `5 > 3` compile?
`5 > 3` is valid C++ for every type T imaginable. It always compiles. So this requires-expression always evaluates to true — for every type T.

The entire requires-expression is a compile-time type check. The compiler reads `5 > 3` and asks one question only:
`"Is this a valid C++ expression?" → YES → requirement satisfied`

except the "evaluation" here happens entirely during compilation, by checking whether code compiles — not by running any code.

The value true that `5 > 3` produces at runtime is never computed. No CPU instruction runs. The compiler is only checking the grammar and types — not evaluating values.

```cpp
template<typename T>
concept AlwaysTrue = requires(T t) {
    5 > 3;
};

template<typename T>
concept AlwaysTrue1 = requires {
    3 > 5;
};

static_assert(AlwaysTrue<int>);         // true
static_assert(AlwaysTrue<std::string>); // true
static_assert(AlwaysTrue<void*>);       // true

static_assert(AlwaysTrue1<int>);         // true
static_assert(AlwaysTrue1<std::string>); // true
static_assert(AlwaysTrue1<void*>);       // true
```
It is valid. It is useless. It constrains nothing.

## The Parameter List — "Pretend" Variables
The parentheses right after `requires` can optionally declare notional parameters — fictional variables that exist only so that member-access and operator expressions can be written naturally in the body. These variables are never constructed. No memory is allocated for them. They are a naming convenience for the compiler's type-checker only.

```cpp
template<typename T>
concept HasSize = requires(T c) {
    //            ↑
    //  'c' is a PRETEND object of type T
    //  it is never actually created
    //  it only exists so 'c.size()' can be written below
    c.size();
};
```

* *When a parameter is needed*: whenever the checks involve calling a member function, using an operator, or otherwise needing an actual object to write the expression against.
    ```cpp
        requires(T a, T b) {
            a + b;        // needs two pretend objects to write a binary operator
        };
    ```
* *When no parameter is needed*: whenever the checks only involve the type itself — no object required.
    ```cpp
        requires {
            typename T::value_type;   // checks something about the TYPE — no object needed
            sizeof(T);                 // sizeof works on a type directly — no object needed
        };
    ```

## The Four Requirement Kinds
The body of a requires-expression is a list of requirements, each ending in a semicolon. There are exactly four kinds. Each kind checks a different thing.
### Simple Requirement — Expression Must Be Valid

**Syntax:**
```
expression;
```
**What is checked** : precisely one thing; does this expression compile for the given type? That is the entire check. Nothing else.

```cpp
template<typename T>
concept HasSize = requires(T c) {
    c.size();   // T must have size()
};

```
The compiler does not look at what `c.size()` returns. It could return `int`, `std::size_t`, `void`, `std::string` — anything at all. As long as writing `c.size()` compiles without error, the requirement passes.

```cpp
struct A { int    size() { return 1; } };   // returns int
struct B { void   size() {}          };   // returns void
struct C { std::string size() { return ""; } };  // returns std::string

static_assert(HasSize<A>);   // true — c.size() compiles, return type irrelevant
static_assert(HasSize<B>);   // true — c.size() compiles, return type irrelevant
static_assert(HasSize<C>);   // true — c.size() compiles, return type irrelevant

struct D { };   // no size() member at all
static_assert(!HasSize<D>);  // false — c.size() does NOT compile
```

`HasSize` accepts `A`, `B`, and `C` equally — the simple requirement only asks "does this compile," not "what does it produce."

### Type Requirement — Type Must Exist

**Syntax:**
```
typename type-name;
```
**What is checked**  does this named type exist and is it well-formed? No object needed — this checks the type system directly.

```cpp
template<typename T>
concept HasValueType = requires {
    typename T::value_type;
};

truct HasIt   { using value_type = int; };
struct HasntIt { };

static_assert(HasValueType<HasIt>);     // true — HasIt::value_type exists
static_assert(!HasValueType<HasntIt>);  // false — HasntIt::value_type does not exist
```

### Compound Requirement — Valid AND Return Type Constrained

`decltype` — What It Is First

Before compound requirements can make sense, `decltype` must be established.

`decltype` is a keyword that asks the compiler one question:
```
"What TYPE would this expression produce if it were evaluated?"
```
It never evaluates the expression. It never runs any code. It only inspects the type.

```cpp
int x = 5;
decltype(x)      // answer: int
decltype(x + 1)  // answer: int   (int + int = int)
decltype(x * 2.0)// answer: double (int * double = double)

std::string s = "hello";
decltype(s.size())  // answer: std::size_t  (that is what size() returns)
decltype(s)         // answer: std::string
```
The expression inside `decltype(...)` is never executed. The compiler just reads it and figures out what type it would produce.

```cpp
int i = 0;
decltype(i++)   // answer: int
                // i is NOT actually incremented — never runs
```

**Syntax:**

```
{ expression } noexcept[optional] -> type-constraint[optional];
```

This is the form that combines braces `{}` with `->`. Both symbols need separate, precise explanations.

What `{}` means: the braces mark the boundary of the expression being checked. 
Everything inside `{}` is the expression. This is required syntax whenever a *return-type constraint* (`->`) follows — the braces exist specifically to separate the expression from the `->` part that comes after it.

**What is checked — TWO separate things, in order:**
```
Check 1: Does the expression inside {} compile?
         (exactly the same check as a simple requirement)
             │
             ▼ if it compiles
Check 2: Does the TYPE of that expression satisfy the concept named after ->?
```

```cpp
template<typename T>
concept Sizeable = requires(T c) {
    { c.size() } -> std::convertible_to<std::size_t>;
    // size() must exist AND return type must convert to size_t
};
```
Critically — `->` is NOT a plain return type slot. It must be followed by a `concept name`, never a `plain type`. This is a common mistake:
```cpp
// ILLEGAL — std::size_t is a TYPE, not a CONCEPT
{ c.size() } -> std::size_t;
// COMPILE ERROR: type-constraint must name a concept

// LEGAL — std::convertible_to is a CONCEPT that takes std::size_t as an argument
{ c.size() } -> std::convertible_to<std::size_t>;

// LEGAL — std::same_as is also a concept
{ c.size() } -> std::same_as<std::size_t>;
```

What actually happens behind `->`: the type of the expression `(decltype((c.size())))` is silently inserted as the first template argument of the concept named after `->`. So: `{ c.size() } -> std::convertible_to<std::size_t>;` really means: `std::convertible_to<decltype((c.size())), std::size_t>`

`std::convertible_to<From, To>` checks whether From converts to To. The expression's own type fills the From slot automatically — that is the entire mechanism behind the `->` syntax.

Full worked pass/fail example:
```cpp
struct Good { std::size_t size() { return 5; } };  // returns size_t — convertible to size_t ✓
struct Weird { std::string size() { return ""; } }; // returns string — NOT convertible to size_t ✗
struct Missing { };                                  // no size() at all — Check 1 fails ✗

static_assert(Sizeable<Good>);     // true  — both checks pass
static_assert(!Sizeable<Weird>);   // false — Check 1 passes, Check 2 fails
static_assert(!Sizeable<Missing>); // false — Check 1 fails immediately, Check 2 never even runs
```

**The optional noexcept:**
```cpp
{ c.size() } noexcept -> std::convertible_to<std::size_t>;
//            ↑
//  ADDS a third check: must c.size() ALSO be marked noexcept?
```
```cpp
struct A { std::size_t size() noexcept { return 5; } };    // satisfies noexcept check
struct B { std::size_t size()          { return 5; } };    // does NOT satisfy noexcept check

template<typename T>
concept NoexceptSizeable = requires(T c) {
    { c.size() } noexcept -> std::convertible_to<std::size_t>;
};

static_assert(NoexceptSizeable<A>);    // true
static_assert(!NoexceptSizeable<B>);   // false — c.size() is not noexcept
```

### Nested Requirement — Boolean Condition Inside

**Syntax:**
```
requires constant-boolean-expression;
```

Why this exists — the exact problem it solves:

The most common mistake when first learning `requires` is trying to check a compile-time boolean condition using a simple requirement:
```cpp
// WRONG — does NOT do what it looks like it does
template<typename T>
concept SmallType = requires {
    sizeof(T) <= 8;   // looks like a check... but is it?
};
```
Recall from a simple requirement only checks whether the expression compiles — it never looks at the expression's value.

`sizeof(T) <= 8` is always a perfectly valid, well-formed boolean expression for any type `T` — whether `T` is 1 byte or 1000 bytes.

The expression compiles in every case. As a simple requirement, this check always passes, regardless of the actual size of T.
```cpp
struct Big { char data[1000]; };   // 1000 bytes

static_assert(SmallType<Big>);   // TRUE — even though Big is 1000 bytes!
// This is WRONG behavior — the concept is broken
// sizeof(Big) <= 8 compiles fine (it's valid C++), so the simple
// requirement passes, even though the actual VALUE is false
```
This is the bug that nested requirements exist to prevent.

**The fix — wrap the condition with requires:**
```cpp
template<typename T>
concept SmallType = requires {
    requires sizeof(T) <= 8;   // NOW this checks the VALUE, not just validity
};

struct Small { char data[4]; };    // 4 bytes
struct Big   { char data[1000]; }; // 1000 bytes

static_assert(SmallType<Small>);    // true  — sizeof(Small) = 4, 4 <= 8 is TRUE
static_assert(!SmallType<Big>);     // false — sizeof(Big) = 1000, 1000 <= 8 is FALSE
```
**How to read requires requires**: the first requires starts the requires-expression. Inside its body, a nested requirement is written as the word requires followed by a constant boolean expression and a semicolon. The nested requirement asserts: this condition must evaluate to true, not merely this condition must compile.

```cpp
requires {                    ← outer requires: starts the expression
    requires sizeof(T) <= 8;  ← inner requires: a NESTED REQUIREMENT
    //  ↑
    //  "requires" here means: check the TRUTH of what follows,
    //  not just whether it compiles
};
```
How it executes: the boolean expression after the nested requires is evaluated as a compile-time constant. If it evaluates to true, the nested requirement is satisfied. If false, the whole requires-expression becomes false at that point.

## How the Whole requires-Expression Evaluates
A requires-expression's body can contain any number of requirements of any of the four kinds, in any order. The entire expression evaluates to true only if every single requirement inside is satisfied. If any one requirement fails, the whole expression is false.
```cpp
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;                              // type requirement
    { c.begin() } -> std::input_iterator;                // compound requirement
    { c.end()   } -> std::input_iterator;                // compound requirement
    c.size();                                             // simple requirement
    requires sizeof(T) <= 64;                             // nested requirement
};
```
`Container<T>` is `true` only if all five requirements hold for `T`. Miss even one, and `Container<T>` is `false`.

### Standalone requires Clause

```cpp
template<typename T>
    requires (sizeof(T) <= 8 && std::is_trivially_copyable_v<T>)
void fastCopy(T* dest, const T* src, std::size_t n) {
    std::memcpy(dest, src, n * sizeof(T));
}
```
