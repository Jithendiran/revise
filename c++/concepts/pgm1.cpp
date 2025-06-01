#include <iostream>
#include <string>
using namespace std;

class Company {
protected:
    int empId;
    string name;

public:
    Company() {
        cout << "[Company] Constructor called.\n";
    }

    virtual ~Company() {
        cout << "[Company] Destructor called.\n";
    }

    void setEmpData(int id, const string& empName) {
        empId = id;
        name = empName;
    }

    void getEmpData() const {
        cout << "Employee ID: " << empId << ", Name: " << name << endl;
    }

    // Correct virtual function name as requested
    virtual string getEmployeeDepartment() const {
        return "Generic Department";
    }
};

// 🔹 HR Department
class HR : public Company {
public:
    HR() {
        cout << "[HR] Constructor called.\n";
    }

    ~HR() {
        cout << "[HR] Destructor called.\n";
    }

    string getEmployeeDepartment() const override {
        return "Human Resources";
    }

    void conductInterview() const {
        cout << "[HR] Conducting interview.\n";
    }

    void managePayroll() const {
        cout << "[HR] Managing payroll.\n";
    }
};

// 🔹 Finance Department
class Finance : public Company {
public:
    Finance() {
        cout << "[Finance] Constructor called.\n";
    }

    ~Finance() {
        cout << "[Finance] Destructor called.\n";
    }

    string getEmployeeDepartment() const override {
        return "Finance Department";
    }

    void processInvoices() const {
        cout << "[Finance] Processing invoices.\n";
    }

    void generateReport() const {
        cout << "[Finance] Generating report.\n";
    }
};

// 🔹 Support Department
class Support : public Company {
public:
    Support() {
        cout << "[Support] Constructor called.\n";
    }

    ~Support() {
        cout << "[Support] Destructor called.\n";
    }

    string getEmployeeDepartment() const override {
        return "Technical Support";
    }

    void resolveTicket() const {
        cout << "[Support] Resolving support ticket.\n";
    }

    void monitorSystem() const {
        cout << "[Support] Monitoring system.\n";
    }
};

int main() {
    cout << "\n--- Creating HR Employee ---\n";
    Company* emp1 = new HR();
    emp1->setEmpData(1, "A");
    emp1->getEmpData();
    cout << "Department: " << emp1->getEmployeeDepartment() << "\n";

    cout << "\n--- Creating Finance Employee ---\n";
    Company* emp2 = new Finance();
    emp2->setEmpData(2, "B");
    emp2->getEmpData();
    cout << "Department: " << emp2->getEmployeeDepartment() << "\n";

    cout << "\n--- Creating Support Employee ---\n";
    Company* emp3 = new Support();
    emp3->setEmpData(3, "C");
    emp3->getEmpData();
    cout << "Department: " << emp3->getEmployeeDepartment() << "\n";

    cout << "\n--- Deleting Employees ---\n";
    delete emp1;
    delete emp2;
    delete emp3;

    return 0;
}

/*

--- Creating HR Employee ---
[Company] Constructor called.
[HR] Constructor called.
Employee ID: 1, Name: A
Department: Human Resources

--- Creating Finance Employee ---
[Company] Constructor called.
[Finance] Constructor called.
Employee ID: 2, Name: B
Department: Finance Department

--- Creating Support Employee ---
[Company] Constructor called.
[Support] Constructor called.
Employee ID: 3, Name: C
Department: Technical Support

--- Deleting Employees ---
[HR] Destructor called.
[Company] Destructor called.
[Finance] Destructor called.
[Company] Destructor called.
[Support] Destructor called.
[Company] Destructor called.
*/