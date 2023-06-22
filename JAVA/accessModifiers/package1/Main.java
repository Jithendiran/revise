/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package accessModifiers.package1;
// private and protected access modifiers cannot use with class
public class Main {
    // example of same package non-subclass
    public static void main(String[] args) {
        A a = new A();
        a.defaultMethodAClass(); //Class A Default Method
        a.protectedMethodAClass(); // Class A protected method
        a.publicMethodAClass(); // Class A protected method
        //a.privateMethodAClass(); // private method not accessable
        a.accessPrivatedMethodClassA();
        /**
         * Class A Private method accessor
         * Class A private method
         */
        
        B b = new B();
        b.defaultMethodBClass(); //Class B Default Method
        b.protectedMethodBClass(); //Class B protected method
        b.publicMethodBClass();// Class B public method
        b.accessPrivatedMethodClassB();
        /**
         * Class B Private method accessor
         * Class B private method
         */
    }
}
