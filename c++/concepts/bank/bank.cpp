#include <memory>
#include <iostream>
using namespace std;

class Bank
{
protected:
    static int accno_counter;
    static unique_ptr<Bank> accounts[10];
    string name;
    double balance = 0;
    int use_acc_no = 0;

public:
    Bank()
    {
        use_acc_no = ++accno_counter;
    }

    virtual int openAccount(const string &name)
    {
        if (use_acc_no < 10)
        {
            this->name = name;
            return use_acc_no;
        }
        return 0;
    }

    virtual void showAccount(int accnp) const = 0;

    virtual bool deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            return true;
        }
        return false;
    }

    virtual Bank *search(int acc) = 0;

    virtual bool withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            return true;
        }
        return false;
    }

    virtual ~Bank() = default;

    int getAccountNumber() const { return use_acc_no; }
    string getName() const { return name; }
    double getBalance() const { return balance; }
};
int Bank::accno_counter = 0;
unique_ptr<Bank> Bank::accounts[10] = {nullptr};

class A_Bank : public Bank
{
public:
    A_Bank() = default;

    int openAccount(const string &name) override
    {
        return Bank::openAccount(name);
    }

    void showAccount(int accnp) const override
    {
        if (accnp == use_acc_no)
        {
            cout << "Account Number: " << use_acc_no << endl;
            cout << "Name: " << name << endl;
            cout << "Balance: $" << balance << endl;
        }
        else
        {
            cout << "Account not found.\n";
        }
    }

    bool deposit(double amount) override
    {
        return Bank::deposit(amount);
    }

    Bank *search(int acc) override
    {
        if (acc == use_acc_no)
            return this;
        return nullptr;
    }

    // Static method to create and store a new account
    static int createAccount(const string &name)
    {
        for (int i = 0; i < 10; ++i)
        {
            if (!accounts[i])
            {
                accounts[i] = make_unique<A_Bank>();
                return accounts[i]->openAccount(name);
            }
        }
        cout << "Account limit reached.\n";
        return -1;
    }
    // Static method to find an account by account number
    static Bank *findAccount(int accNo)
    {
        for (auto &acc : accounts)
        {
            if (acc)
            {
                Bank *result = acc->search(accNo);
                if (result)
                    return result;
            }
        }
        return nullptr;
    }

    // Optional: show all accounts
    static void showAllAccounts()
    {
        for (auto &acc : accounts)
        {
            if (acc)
            {
                acc->showAccount(acc->getAccountNumber());
                cout << "----------------------\n";
            }
        }
    }
};

// dynamic datastructure like array list is more suitable

int main()
{
    int acc1 = A_Bank::createAccount("A");
    int acc2 = A_Bank::createAccount("B");

    Bank *a = A_Bank::findAccount(acc1);
    Bank *b = A_Bank::findAccount(acc2);

    if (a)
    {
        a->deposit(1000);
        a->withdraw(300);
    }

    if (b)
    {
        b->deposit(500);
        b->withdraw(100);
    }

    A_Bank::showAllAccounts();
}