/**
 * Author: E.K.Jithendiran
 * Date: 01.03.2023
 */
package inheritance;

class A{
    void ping(){
        System.out.println("A overridable ping");
    }

    final void ping(int i){ // this method can't be over ride because of final
        System.out.println("A non-overridable ping : "+i);
    }
}

class B extends A{
    void ping() {
        System.out.println("B overridable ping");
    }

    // Error: java.lang.IncompatibleClassChangeError: class inheritance.B overrides final method inheritance.A.ping(I)V
    // void ping(int i){
    //     System.out.println("A non-overridable ping : "+i);
    // }
}

// final key word is used in class 
final class C extends A{
    // c class no more extendable
    void ping() {
        System.out.println("C overridable ping");
    }
}

// error:Exception in thread "main" java.lang.Error: Unresolved compilation problem: The type D cannot subclass the final class C
// class D extends C {}


/**
 * final key word can be used with
 *  - Class, Method, Variable 
 */

public class Final extends A {
    public static void main(String[] args) {
        final int CONSTANT_I = 9;
        //CONSTANT_I = 8;  The final local variable CONSTANT_I cannot be assigned.
        B b = new B();
        b.ping(); //B overridable ping
        b.ping(CONSTANT_I); //A non-overridable ping : 9

        C c = new C();
    }
}
