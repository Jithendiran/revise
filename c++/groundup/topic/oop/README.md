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

## The Conceptual Pillars of OOP

OOP solves software complexity by organizing programs into a hierarchy of concepts modeled after the physical world.

**The Blueprint vs. The Reality**
* The Blueprint(struct/class): A purely conceptual definition. It specifies what data fields an entity will have and what actions it can perform. It occupies no physical memory space.
* The Realization(Object/Instance): The physical entity created in the computer's memory based on the blueprint. It contains actual data and occupies physical space.

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

2.  **Abstraction (The Simplified Interface)**
    Abstraction hides complex background implementation details and displays only the essential features required to interact with the entity.
    * Why: To reduce mental effort for the programmer. One does not need to understand internal combustion mechanics to drive a car; one only needs to understand the steering wheel and pedals.
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