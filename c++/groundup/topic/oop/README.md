## Object-Oriented Programming (OOP)

### The New Problem Created by `struct`
While `struct` solved the data organization problem, it introduced a procedural vulnerability: separation of data and logic.

In a standard structure, the data is entirely passive. Functions that modify the structure exist outside of it:
```cpp
void withdraw(BankAccount& account, double amount) {
    account.balance -= amount; // Direct, unregulated modification
}
```
This caused two critical flaws:
1. Lack of Data Protection: Any part of the program could bypass the `withdraw` function and directly modify `account.balance = -999999;`. There was no mechanism to enforce rules or validate data.
2. Global Namespace Pollution: Functions belonging to different structures lived in the same global space, leading to naming conflicts and disorganized code bases as programs grew.

### Definition and Core Purpose
Object-Oriented Programming (OOP) was invented to solve the fundamental flaw of the `struct` era: the complete separation of data from the functions that manipulate that data.

#### What is OOP?
OOP is a software design approach that shifts the focus from writing sequential instructions (functions first) to modeling self-sufficient entities (data and behavior together).

#### Why Was OOP Invented?
OOP was invented to manage software complexity. As applications grew to millions of lines of code, procedural code became impossible to maintain. A change in one global variable could cause a catastrophic chain reaction of bugs across the entire program. OOP was created to provide isolation, reusability, and predictability.

## Class and Objects

OOP solves software complexity by organizing programs into a hierarchy of concepts modeled after the physical world.

**The Blueprint vs. The Reality**
* The Blueprint(struct/class): A purely conceptual definition. It specifies what data fields an entity will have and what actions it can perform. It occupies no physical memory space.
* The Realization(Object/Instance): The physical entity created in the computer's memory based on the blueprint. It contains actual data and occupies physical space.

### What is a Class?
While an object is the actual physical house built out of brick (RAM), a class is the architectural blueprint drawn on paper. It contains two things:
1. The Shape of the Data: It tells the system exactly what kinds of information the entity will hold.
2. The Size of the Entity: It defines the exact physical footprint the entity will occupy in the computer’s memory.
3. 
The name comes from the concept of classification in science and nature. In the real world, a "class" is a category used to group things that share identical traits like the class Mammal, which defines the shared characteristics of all warm-blooded creatures. A programming class works exactly like this classification. It doesn't represent a single item; instead, it defines the common group rules, data structures, and behaviors that an entire category of objects will share.

### What is an Object?
An object is a dedicated packet of computer memory. This packet holds two things together:
1. Data: The pieces of information that describe something.
2. Actions: The commands that are allowed to change that information.

The name comes from physical objects in the real world. A physical vending machine holds items (data) and has buttons to drop an item (action). A person cannot reach inside to grab a drink directly; the person must press a button. An object works exactly like this vending machine. It protects its internal information by forcing all changes to go through its allowed actions.

#### Object versus Instance
While these two words point to the exact same thing in memory, they explain it from different angles:
* Object: Refers to the physical item sitting inside the computer memory.
* Instance: Refers to the relationship between that item and its blueprint. For example, if there is a blueprint named `BankAccount`, a specific account in memory is called "an instance of the `BankAccount` blueprint."

#### Multi-Instance Mechanics: How One Function Works with Many Objects
A program can create many separate objects from a single blueprint. This creates a problem: how does a single piece of code change the data of the correct object without messing up the others?

**The Memory Setup**

To save computer memory, the system uses two rules:
* Variables are Unique: Every single object gets its own separate memory slot for variables. If three accounts exist, three separate balances exist in memory.
* Functions are Shared: The instructions for an action are identical for every object. To save space, the computer keeps only one copy of the function instructions in memory. All objects share this single copy.

**The Hidden Address Tracker**

Because all objects share the same function instructions, the computer needs a way to tell the function which object is currently using it. Every time an action is called on an object, the computer secretly passes the exact memory location of that object into the function.

### The Conceptual Pillars of OOP

1. **Encapsulation (The Protective Shell)**
    Encapsulation binds data and functions into a single entity and places a protective barrier around them. The internal workings are hidden, and the entity exposes only specific, controlled access points.
    * Why: To prevent external code from corrupting internal data states, ensuring the entity retains total control over its own validity.
    ```cpp
    // =================================================================
    // ENCAPSULATION: Bundling data and methods into a protective shell
    // =================================================================
    CLASS BankAccount {
        
        // DATA
        // 1. HIDDEN DATA: Private variables cannot be accessed or corrupted from the outside. 
        // Even if this variable were marked PUBLIC, the core concept remains the same: the data belongs to the object, and its updates are handled internally by the object's own function rather than an external program. Private is a way to force
        PRIVATE decimal balance = 0.0

        // FUNCTION
        // 2. CONTROLLED GATEWAY: Public functions enforce rules before altering data.
        PUBLIC FUNCTION Deposit(amount) {
            IF amount > 0 THEN
                balance = balance + amount
            ELSE
                PRINT "Error: Invalid Amount"
            ENDIF
        }

        // DATA+FUNCTION = Encapsulation

        // 3. SECURE READ-ONLY ACCESS
        PUBLIC FUNCTION GetBalance() {
            RETURN balance
        }
    };

    // =================================================================
    // EXECUTION
    // =================================================================
    PROGRAM Main {
        BankAccount myAccount;

        // ILLEGAL: This will fail because 'balance' is protected inside the shell.
        // myAccount.balance = -5000.00 

        // LEGAL: External code must go through the secure, public gateway.
        myAccount.Deposit(100.00) 
        
        PRINT myAccount.GetBalance() // Output: 100.00
    }
    ```

    * Step-by-Step Execution
    When the program runs a command to update an account, the computer processes it using these steps:
    1. The program calls the action: `myAccount.Deposit(100)`
    2. The computer looks up the shared `Deposit` function instructions.
    3. The computer secretly hands the memory location of myAccount to that function.
    4. The function executes the math using only the variables found at the memory location of myAccount.
    Because the unique memory location is always passed along, the shared function will only alter the data of the specific object that triggered the action.

    This is the Encapsulation, data and functions are bundled together as a single entity, Here 

2.  **Abstraction (The Simplified Interface)**
    Abstraction hides complex background implementation details and displays only the essential features required to interact with the entity.
    * Why: To reduce mental effort for the programmer. One does not need to understand internal combustion mechanics to drive a car; one only needs to understand the steering wheel and pedals.
    
    *This is were private comes into play*
    ```cpp
    // =================================================================
    // ABSTRACTION: Hiding internal complexity; showing only what is needed.
    // =================================================================
    CLASS CoffeeMachine {

        // HIDDEN COMPLEXITY: The user does not need to see or manage these internal steps.
        PRIVATE FUNCTION BoilWater() { ... }
        PRIVATE FUNCTION GrindBeans() { ... }
        PRIVATE FUNCTION FilterCoffee() { ... }

        // THE SIMPLIFIED INTERFACE: The single button exposed to the outside world.
        PUBLIC FUNCTION MakeCoffee() {
            BoilWater()
            GrindBeans()
            FilterCoffee()
            PRINT "Coffee is ready!"
        }
    }

    // =================================================================
    // EXECUTION
    // =================================================================
    PROGRAM Main {
        CoffeeMachine machine = NEW CoffeeMachine()

        // THE PROGRAMMER EXPERIENCE: Highly simplified.
        // One direct command hides three internal processes.
        machine.MakeCoffee() 
        
        // ILLEGAL: External code cannot call internal operations directly.
        // machine.GrindBeans() <-- ERROR
    }
    ```

3. **Inheritance (The Hierarchy of Specialization)**
    Inheritance allows a new blueprint to absorb the attributes and behaviors of an existing blueprint, creating a "parent-child" relationship.
    * Why: To eliminate redundant code. If a generic blueprint for "Vehicle" exists, a blueprint for "Truck" can inherit everything from "Vehicle" and add only the specific attributes unique to trucks (like cargo capacity).
    ```cpp
    // =================================================================
    // INHERITANCE: Reusing a base blueprint to create a specialized one.
    // =================================================================

    // Parent Class (Base Blueprint)
    CLASS Vehicle {
        PUBLIC string brand
        
        PUBLIC FUNCTION StartEngine() {
            PRINT brand + " engine started."
        }
    }

    // Child Class (Derived Blueprint)
    // Truck automatically gains the 'brand' variable and 'StartEngine' function.
    CLASS Truck EXTENDS Vehicle {
        PUBLIC decimal cargoCapacity // New specific feature unique to trucks
    }

    // =================================================================
    // EXECUTION
    // =================================================================
    PROGRAM Main {
        // Create an instance of the child class
        Truck myTruck = NEW Truck()

        // 1. Using features inherited from the parent
        myTruck.brand = "Ford"
        myTruck.StartEngine() // Output: Ford engine started.

        // 2. Using features unique to the child
        myTruck.cargoCapacity = 5000.0
    }
    ```

4. **Polymorphism (Context-Based Behavior)**
    Polymorphism allows the same interface or command to adapt its behavior based on the specific type of object receiving it.
    * Why: To allow uniformity in code. A system can issue a generic render command to a list of shapes, and each shape automatically executes its own specific drawing logic without the main program needing to know the exact shape type beforehand.
    ```cpp
    // =================================================================
    // POLYMORPHISM: Uniform commands with object-specific behavior.
    // =================================================================

    // Parent Class (Base Blueprint)
    CLASS Shape {
        // Defines a uniform command that every shape must have.
        PUBLIC VIRTUAL FUNCTION Render() {
            // Base definition (empty or generic)
        }
    }

    // Child Class 1
    CLASS Circle EXTENDS Shape {
        // Customizes the command for circles
        PUBLIC OVERRIDE FUNCTION Render() {
            PRINT "Drawing a smooth 360-degree curve."
        }
    }

    // Child Class 2
    CLASS Square EXTENDS Shape {
        // Customizes the command for squares
        PUBLIC OVERRIDE FUNCTION Render() {
            PRINT "Drawing four equal sides with 90-degree corners."
        }
    }

    // =================================================================
    // EXECUTION
    // =================================================================
    PROGRAM Main {
        // Uniformity: Mixing different shapes into one generic list
        LIST canvas = [ NEW Circle(), NEW Square() ]

        // UNIFORMITY IN ACTION:
        // The main program does not check if an item is a circle or a square.
        // It issues the identical "Render" command to every item.
        FOR EACH shape IN canvas {
            shape.Render()
            // 1st item output: Drawing a smooth 360-degree curve.
            // 2nd item output: Drawing four equal sides with 90-degree corners.
        }
    }
    ```