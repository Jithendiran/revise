/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package accessModifiers.package1;
// example of same package subclass
// A class is only inherit in this package only
public class Main2 extends A {
    public static void main(String[] args) {
        Main2 m = new Main2();
        m.publicMethodAClass(); // Class A public method
        m.protectedMethodAClass(); //Class A protected method
        m.defaultMethodAClass(); // Class A Default Method
        //m.privateMethodAClass(); // not accessable
        m.accessPrivatedMethodClassA();
        /**
         * Class A Private method accessor
         * Class A private method
         */
    }
}
