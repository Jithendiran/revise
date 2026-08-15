# Inner Classes

## The Problem — Related Types With No Natural Home

Some types exist solely to support another type. A node in a linked list has no meaningful existence outside the list. An iterator for a container is meaningless without the container. Placing these types at namespace scope pollutes the namespace and obscures their relationship.

```cpp
// Without inner classes — pollutes namespace
struct ListNode { int value; ListNode* next; };   // belongs to List, not global scope
struct ListIterator { /* ... */ };                 // belongs to List, not global scope

class List {
    ListNode* head;
public:
    ListIterator begin();
};
```

Inner classes solve this by placing related types inside the class they belong to.

## Basic Inner Class

An inner class (also called a nested class) is a class defined inside another class. It is a full class with its own members, access specifiers, constructors, and destructors.

```cpp
class List {
public:
    // Inner class — lives in List's scope
    class Node {
    public:
        int   value;
        Node* next;

        Node(int v) : value(v), next(nullptr) {}
    };

private:
    Node* head;
    int   size;

public:
    List() : head(nullptr), size(0) {}

    void push(int value) {
        Node* n = new Node(value);   // VALID: List can use Node directly
        n->next = head;
        head = n;
        size++;
    }
};

// Outside List:
List::Node* ptr = new List::Node(42);   // qualified name required
// Node* ptr = new Node(42);            // COMPILE ERROR: Node not in scope here
```

An inner (nested) class can be instantiated independently without needing an object of the outer class.

### Inner Class Syntax and Scoping
An inner class is defined inside the outer class body. From outside the outer class, the inner class is accessed using the scope resolution operator `::`.

## The Access Rules — What Can See What
Before any syntax is shown, the access rules must be precisely established. These are the two rules that govern all inner class behavior:

**Rule 1 — Inner class accessing outer class members:**

An inner class has access to **all** names the outer class has access to — including `private` and `protected` members. No `friend` declaration is needed. This rule applies in C++11 and later.

**Rule 2 — Outer class accessing inner class members:**

The outer class has **no** special access to the inner class's members. The usual access rules apply. If the inner class has `private` members, the outer class cannot access them without a `friend` declaration inside the inner class.

> **The inner class still needs an explicit object:**
>  Rule 1 grants access to the name `outerPrivate`. It does not provide an implicit `this` pointer to an outer object. The inner class must receive the outer object explicitly — through a parameter, a stored reference, or a pointer — to access its instance members.
>
> An outer class *never* has an implicit pointer or reference to an inner class object. Even if grant perimission using friendship (`friend class Outer`; inside the inner class), the outer class still needs an explicit object (passed as a parameter, stored as a member, or created locally) to access the inner class's instance members.

```cpp
#include <iostream>

class Outer {
    int outerPrivate = 10;   // private

public:
    class Inner {
        int innerPrivate = 20;  // private

    public:
        void readOuter(const Outer& o) {
            std::cout << "Outer's private accessed from Inner: " << o.outerPrivate << "\n";  // VALID: inner can access outer private
        }
    };

    void readInner() {
        Inner i;
        // std::cout << i.innerPrivate;   // COMPILE ERROR: outer cannot access
                                          // inner's private — usual rules apply
    }
};

int main() {
    Outer outerObj;
    Outer::Inner innerObj;

    innerObj.readOuter(outerObj); // Pass as reference or pointer
    outerObj.readInner();

    return 0;
}
```

**Fixed**


```cpp
#include <iostream>

class Outer {
    int outerPrivate = 10;

public:
    class Inner {
        int innerPrivate = 20;

        // Grant Outer permission to access Inner's private members
        friend class Outer;

    public:
        // Inner needs an explicit Outer object to access outer's members
        void readOuter(const Outer& o) {
            std::cout << "Inner reading Outer's private: " << o.outerPrivate << "\n";
        }
    };

    // Outer needs an explicit Inner object to access inner's members (thanks to 'friend')
    void readInner(const Inner& i) {
        std::cout << "Outer reading Inner's private: " << i.innerPrivate << "\n";
    }
};

int main() {
    Outer outerObj;
    Outer::Inner innerObj;

    // 1. Inner accesses Outer's private data (requires passing outerObj explicitly)
    innerObj.readOuter(outerObj);

    // 2. Outer accesses Inner's private data (requires passing innerObj explicitly)
    outerObj.readInner(innerObj);

    return 0;
}
```

### Design
C++ was designed: as a language of `containment`, `performance`, and `clear boundaries`.

Unlike languages like Java, C++ nested classes are designed as a *scoping* and *organizational tool*, not an object-oriented containment model. Here is the reasoning behind each part of the behavior:
1. **Why Inner Classes Can Access Outer Private Members (Rule 1)**
    * The Reason: Nesting is a tool for tight collaboration.
    * In C++, a nested class is considered a member of the outer class, just like a function or variable declared inside that class. Because the outer class "owns" the inner class, the language designers decided that the inner class should be treated as part of the family.
    * It acts as a natural extension or helper to the outer class (like a `Node` inside a `LinkedList`). Allowing the helper direct access to the outer class's internals saves you from writing redundant getter/setter boilerplate while keeping those internals hidden from the rest of the world.
2. **Why Outer Classes Cannot Access Inner Private Members (Rule 2)**
    * The Reason: Information hiding and encapsulation hierarchy.
    * Just because a class is defined inside another class doesn't mean the outer class "owns" its internal implementation details. In fact, nested classes are often used to completely hide implementation details from the outer class and the outside world.
    * If the outer class could automatically poke into the private parts of any inner class, it would break encapsulation. The inner class would lose control over its own data integrity. Therefore, the rule enforces that the inner class must explicitly open the door (`friend class Outer`) if it wants to expose its internals to the outer class.
3. **Why There Is No Implicit this Pointer to an Outer Object**
   * The Reason: Memory overhead and object independence.
   * In languages like Java, every inner class object carries a hidden pointer (a "reference to the outer instance") behind the scenes. This means every single inner class object takes up extra memory, even if it never uses the outer object.
   * C++ follows the "zero-overhead principle": *You don’t pay for what you don't use.* C++ doesn't force a hidden outer pointer into every nested class object because you might want an inner class (like a configuration struct, an enum wrapper, or a helper utility) that has nothing to do with any specific outer instance. By requiring you to pass a reference explicitly when you need it, C++ keeps inner class objects lightweight and completely independent.

Inner and outer class object are completely independet of each other. only catch is inner class can access outer's private members 

[Part2](./Access.md)