# The requires-Clause
A clause attached directly to a template, not a bracketed expression body.

## Placement — Two Valid Positions
```cpp
// Position 1: right after the template parameter list
template<typename T>
    requires std::integral<T>
T add(T a, T b) { return a + b; }

// Position 2: trailing, after the function signature
template<typename T>
T add(T a, T b) requires std::integral<T> { return a + b; }
```
Both positions are equivalent. The choice is style — Position 1 is more common for readability with complex conditions.

## What Can Go Inside a requires-Clause
The condition after requires in a clause is any expression that produces a compile-time bool. This can be any of the following forms:

**A concept name:**
```cpp
template<typename T>
    requires std::integral<T>
void f(T);
```

**A logical combination of concepts:**
```cpp
template<typename T>
    requires std::integral<T> && std::signed_integral<T>
void f(T);
```

**A plain compile-time boolean expression — no concept needed at all:**
```cpp
template<typename T>
    requires (sizeof(T) <= 8 && std::is_trivially_copyable_v<T>)
void fastCopy(T* dest, const T* src, std::size_t n) {
    std::memcpy(dest, src, n * sizeof(T));
}
```

### Breaking this exact example down:
```
requires (sizeof(T) <= 8 && std::is_trivially_copyable_v<T>)
   ↑      ↑                 ↑
clause   parenthesized     a second condition, combined with &&
         boolean condition  (a variable_template —  this is
         (uses sizeof       NOT a requires-expression, it's an  
          directly, NO      ordinary constexpr bool value from
          nested requires   the standard library)
          needed because    
          this is a plain
          bool expression,
          not wrapped
          inside a
          requires{} body)
```
Important — why `sizeof(T) <= 8` works directly here, unlike the nested-requirement problem only applies inside a requires-EXPRESSION's body (the {} block), where a bare expression is interpreted as "must compile," not "must be true." A requires-clause is different — its entire condition is always interpreted as a plain boolean value to be evaluated, not as a "does this compile" check. So writing sizeof(T) <= 8 directly after a clause's requires is correct and behaves exactly as expected — no nested requires needed here.

### A requires-expression itself — the "requires requires" pattern:
```cpp
template<typename T>
    requires requires(T t) { t.begin(); t.end(); }
void processContainer(T& container) { }
```

```
requires         requires(T t) { t.begin(); t.end(); }
   ↑                  ↑
requires-CLAUSE    requires-EXPRESSION
(attaches the      (the actual boolean value being
 constraint to      checked — this is exactly the
 the template)       construct)
 ```
This is not a typo or a special combined keyword — it is two separate `requires` uses back to back: the clause needs a boolean value, and a requires-expression is one way to produce a boolean value.

### "Parameters" of a requires-Clause — Answering Directly
A requires-clause itself has no parameter list of its own. The parentheses seen in some examples (`requires (sizeof(T) <= 8)`) are just ordinary grouping parentheses around a boolean expression — not a parameter declaration. Compare:
```cpp
requires (sizeof(T) <= 8)
//       ↑                ↑
//   just grouping   just grouping
//   parentheses —   parentheses —
//   same as writing (a + b) in math
```
Parameters only exist on requires-EXPRESSIONS. If a requires-clause's condition needs a pretend object to write member-access expressions, a nested requires-expression must be used — and that requires-expression is where the parameter list belongs:
```cpp
template<typename T>
    requires requires(T t) { t.begin(); }
    //                ↑
    //        parameter belongs HERE — on the requires-EXPRESSION,
    //        never on the outer requires-CLAUSE
void f(T& x);
```
**When to provide a parameter:**  whenever the check needs to call a member function or use an operator on an instance of the type. If the clause's condition is a plain `sizeof(T)`-style check or a concept name, no parameter and no nested requires-expression is needed at all.

## Every Piece Together
```cpp
#include <concepts>
#include <cstddef>

template<typename T>
concept SmallSerializable = requires(T c) {
    // Simple requirement — does serialize() compile?
    c.serialize();

    // Type requirement — does T::value_type exist?
    typename T::value_type;

    // Compound requirement — does size() compile AND return
    // something convertible to std::size_t?
    { c.size() } -> std::convertible_to<std::size_t>;

    // Compound requirement with noexcept — does clear() compile,
    // return void, AND is it marked noexcept?
    { c.clear() } noexcept -> std::same_as<void>;

    // Nested requirement — is T actually 32 bytes or smaller?
    // (a simple requirement here would be WRONG — see Section 4.4)
    requires sizeof(T) <= 32;
};

// Using it in a requires-clause, combined with another condition:
template<typename T>
    requires SmallSerializable<T> && std::default_initializable<T>
void store(T value) {
    // only compiles for types satisfying BOTH constraints
}
```