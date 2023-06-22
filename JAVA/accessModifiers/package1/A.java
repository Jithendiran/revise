/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package accessModifiers.package1;

// default class
class A {
    private void privateMethodAClass() {
        System.out.println("Class A private method");
    }

    public void publicMethodAClass() {
        System.out.println("Class A public method");
    }

    protected void protectedMethodAClass() {
        System.out.println("Class A protected method");
    }

    void defaultMethodAClass() {
        System.out.println("Class A Default Method");
    }

    public void accessPrivatedMethodClassA() {
        System.out.println("Class A Private method accessor");
        privateMethodAClass();
    }

    // example of same classs
    public static void main(String[] args) {
        A a = new A();
        a.defaultMethodAClass(); //Class A Default Method
        a.publicMethodAClass(); //Class A public method
        a.protectedMethodAClass(); //Class A protected method
        a.privateMethodAClass();// Class A private method
        a.accessPrivatedMethodClassA();
        /**
         * Class A Private method accessor
         * Class A private method
         */
    }
}
