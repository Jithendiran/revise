/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package accessModifiers.package2;

import accessModifiers.package1.*;

public class Main {
    // example of different package non-subclass
    public static void main(String[] args) {
        // A a = new A(); // A is default class of another package
        B b = new B();
        // b.defaultMethodBClass(); // default is not accessable
        // b.protectedMethodBClass(); // protected is not accessable
        b.publicMethodBClass(); // Class B public method
        //b.privateMethodBClass(); // private method like this is not accessable
        b.accessPrivatedMethodClassB();
        /**
         * Class B Private method accessor
         * Class B private method
         */
    }
}
