
## requires Expressions (C++20)

A `requires` expression tests whether expressions or types are valid for a given type. Returns true or false at compile time. The building block for concepts

### Simple Requirement — Expression Must Be Valid

```cpp
template<typename T>
concept HasSize = requires(T c) {
    c.size();   // T must have size() — no return type check
};
```

### Type Requirement — Type Must Exist

```cpp
template<typename T>
concept HasValueType = requires {
    typename T::value_type;
};
```

### Compound Requirement — Valid AND Return Type Constrained

```cpp
template<typename T>
concept Sizeable = requires(T c) {
    { c.size() } -> std::convertible_to<std::size_t>;
    // size() must exist AND return type must convert to size_t
};
```

### Nested Requirement — Boolean Condition Inside

```cpp
template<typename T>
concept SmallType = requires {
    requires sizeof(T) <= 8;   // T must fit in 8 bytes
};
```

### Standalone requires Clause

```cpp
template<typename T>
    requires (sizeof(T) <= 8 && std::is_trivially_copyable_v<T>)
void fastCopy(T* dest, const T* src, std::size_t n) {
    std::memcpy(dest, src, n * sizeof(T));
}
```
