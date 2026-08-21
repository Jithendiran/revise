# Template tye is template

Start from what is already known.

## What Is Already Known

```cpp
template<typename T>
void function(T v) { }
```

Here `T` is a **type parameter**. The caller passes a type — `int`, `double`, `std::string` — and the compiler fills `T` with that type.

```cpp
function(5);          // T = int
function(3.14);       // T = double
function("hello");    // T = const char*
```

## The New Question — What If T Itself Needs to Be a Template?

Consider `std::vector`. It is not a type. It is a **template** that produces a type when given a type argument:

```
std::vector        — not a type (it is a template)
std::vector<int>   — a type (instantiation of the template)
std::vector<double>— another type
```

Now imagine a function that should work with `std::vector`, `std::list`, `std::deque` — any container — but the *caller chooses which one*.

The caller does not want to pass `std::vector<int>` (a fully formed type). The caller wants to pass `std::vector` (the template itself), and let the function decide what element type to use inside.


### Why the Normal Approach Fails

```cpp
template<typename Container>
void function(Container v) { }
```

This accepts `std::vector<int>` — a complete type. Fine. But what if the function needs to create a new container of a different element type internally? It cannot — it only has the complete type, not the template.

## Template Template Parameter — The Solution

```cpp
template<typename T, template<typename> typename Container>
void function(T v) {
    Container<T> storage;   // create a container of type T
    storage.push_back(v);
}
```

Now `Container` is a **template parameter that holds a template** — not a type. The function receives the template blueprint itself, then instantiates it with `T` inside.


### Reading the Declaration

```cpp
template <
    typename T,
    template <typename> class Container
>
void createAndPrintSingle() {
    // Create the container inside the function
    Container<T> c;
    c.push_back(10);
    c.push_back(20);
    c.push_back(30);

    std::cout << "Created vector of size " << c.size() << ": ";
    for (const auto& val : c) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}
```

Breaking `template<typename> typename Container` apart:

```
template<typename>    ← describes what kind of template Container must be "a template that takes one type parameter"
typename Container    ← Container is the name given to that template
```

So `Container` must be a template that accepts exactly one type parameter. `std::vector` qualifies — it takes one type parameter `std::map` does not qualify — it takes two type parameters.

**Two parameters of template***
```cpp
template <
    typename Key,
    typename Value,
    template <typename, typename> class Container
>
void createAndPrintMap() {
    // Create the map inside the function using the 2-parameter template
    Container<Key, Value> m;
    
    // Insert some elements
    m[1] = "Apple";
    m[2] = "Banana";
    m[3] = "Cherry";

    std::cout << "Created map of size " << m.size() << ":\n";
    for (const auto& pair : m) {
        std::cout << "  " << pair.first << " -> " << pair.second << "\n";
    }
}

// std::map takes 2 type parameters (Key and Value)
createAndPrintMap<int, std::string, std::map>();
```


### Calling the Function

```cpp
createAndPrintMap<int, std::vector>(5);
//                ↑         ↑
//                T = int   Container = std::vector (the template, not std::vector<int>)
```

Inside the function:

```cpp
Container<T> c;
// = std::vector<int> c
```
