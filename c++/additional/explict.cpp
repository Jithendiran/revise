#include <iostream>
#include <string>

using namespace std;

// --- Class without 'explicit' (Allows Implicit Conversions) ---
struct ImplicitLogger
{
    string logEntry;

    // Single-argument constructor without 'explicit'
    ImplicitLogger(const string& entry) : logEntry(entry)
    {
        cout << "ImplicitLogger created with: " << entry << endl;
    }
    
    void print() const {
        cout << " [Implicit] Log: " << logEntry << endl;
    }
};

// Function that takes an ImplicitLogger object by value
void processImplicit(ImplicitLogger logger)
{
    logger.print();
}

// ------------------------------------------------------------------

// --- Class with 'explicit' (Prevents Implicit Conversions) ---
struct ExplicitLogger
{
    string logEntry;

    // Single-argument constructor WITH 'explicit'
    // This prevents the compiler from using this constructor for implicit conversions.
    explicit ExplicitLogger(const string& entry) : logEntry(entry)
    {
        cout << "ExplicitLogger created with: " << entry << endl;
    }

    void print() const {
        cout << " [Explicit] Log: " << logEntry << endl;
    }
};

// Function that takes an ExplicitLogger object by value
void processExplicit(ExplicitLogger logger)
{
    logger.print();
}

// ------------------------------------------------------------------

int main()
{
    cout << "--- 1. Testing ImplicitLogger (NO explicit) ---" << endl;
    
    // a) Direct Initialization (Always fine)
    ImplicitLogger il1("Entry from Direct Init");
    
    // b) Copy Initialization (Implicit conversion allowed)
    // The string literal "Entry for Copy Init" is AUTOMATICALLY converted to an ImplicitLogger object.
    cout << "Attempting Copy Initialization..." << endl;
    // ImplicitLogger il2 = "Entry for Copy Init"; // This may fail in latest compiler, but allowed in older compilers
    
    // c) Function Call (Implicit conversion allowed)
    // The string literal "Entry for Function Call" is AUTOMATICALLY converted to an ImplicitLogger object to match the function parameter.
    cout << "Attempting Function Call Conversion..." << endl;
    // processImplicit("Entry for Function Call"); // This may fail in latest compiler, but allowed in older compilers 

    cout << "\n--- 2. Testing ExplicitLogger (WITH explicit) ---" << endl;
    
    // a) Direct Initialization (Always fine)
    ExplicitLogger el1("Entry from Direct Init");
    
    // b) Copy Initialization (Implicit conversion PREVENTED)
    // ExplicitLogger err1 = "Entry for Copy Init"; // <-- **COMPILER ERROR**
    // To make this work, you must be explicit:
    cout << "Attempting Copy Initialization (requires explicit call)..." << endl;
    ExplicitLogger el2 = ExplicitLogger("Entry for Copy Init");
    
    // c) Function Call (Implicit conversion PREVENTED)
    // processExplicit("Entry for Function Call"); // <-- **COMPILER ERROR**
    // To make this work, you must be explicit:
    cout << "Attempting Function Call Conversion (requires explicit call)..." << endl;
    processExplicit(ExplicitLogger("Entry for Function Call"));
    
    return 0;
}

// g++ explict.cpp -o /tmp/explict -std=c++11