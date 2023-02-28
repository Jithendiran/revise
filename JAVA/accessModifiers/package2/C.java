/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package accessModifiers.package2;

import accessModifiers.package1.*;

class C extends B{
    // example of different package subclass
    public static void main(String[] args) {
        B b = new B();
        //b.defaultMethodBClass(); // Not accessable
        //b.protectedMethodBClass(); //Not accessable
        b.publicMethodBClass();// Class B public method

        C c = new C();
        //c.defaultMethodBClass();// not accessable
        c.publicMethodBClass();// Class B public method
        c.protectedMethodBClass(); //Class B protected method
        //c.privateMethodBClass();// not accessable
        c.accessPrivatedMethodClassB();
        /**
         * Class B Private method accessor
         * Class B private method
         */
    }
}
