/**
 * Author: E.K.Jithendiran
 * Date: 02.03.2023
 */
package inheritance;

/**
 * In java when a method is declared not defined that is abstract method
 * In a class if any one method is abstract that class must be abstract class
 * In abstract class there may be a method that is not abstract
 * Cannot create object for abstract class
 */

 // exmaple 1
 abstract class Aa {
    int i;
    //abstract int j; // error
    void pingEmpty() {
        System.out.println("Ping empty");
    }

   public abstract void pingInt(int i);
   abstract public void pingFloat(float f);
//    abstract public void pingfFloat(float f){ Error:Abstract methods do not specify a body
//         System.out.println("ff : "+f);
//    }
 }

 abstract class Ba extends Aa{
    // This class has no implementation of pingFloat so this class must be abstract
    public void pingInt(int i) {
        System.out.println("Ping "+i);
    }
 }

 class Ca extends Ba{
    // All the abstract methods are implemented 
    // Objact can be created for this class and access all the methods
    public void pingFloat(float f) {
        System.out.println("Ping "+f);
    }
    
    void test(){
        System.out.println("IN test");
    }
 }

// example 2
abstract class Base {
    abstract void show();
}

class SubA extends Base {

    @Override
    void show() {
       System.out.println("It is msg from sub a");
    }

}

class SubB extends Base {

    @Override
    void show() {
       System.out.println("It is msg from sub a");
    }

}

public class Abstract {
    public static void main(String[] args) {
        //A a = new A(); Error: Cannot instantiate the type A
        Ca c = new Ca();
        c.pingEmpty(); //Ping empty
        c.pingInt(1); //Ping 1
        c.pingFloat(6.7f); //Ping 6.7
        c.test();


        // It is annonyms inner class
        Aa ca1 = new Aa() {
            @Override
            public void pingInt(int i) {
               System.out.println("Annonyms Inner class ping "+i);
            }

            @Override
            public
            void pingFloat(float f) {
                System.out.println("Annonyms Inner class ping "+f);
            }
        };

        ca1.pingEmpty(); //Ping empty
        ca1.pingInt(2); //Annonyms Inner class ping 2
        ca1.pingFloat(34f); //Annonyms Inner class ping 34.0


        // upcasting // explained in inheritance/DynaminMethodDispatch.java
        Aa c2 = new Ca();
        c2.pingEmpty(); //Ping empty
        c2.pingInt(3);// Ping 3
        c2.pingFloat(45.7f); //Ping 45.7
        //c2.test();// Error:  The method test() is undefined for the type Aa

        SubA a = new SubA();
        SubB b = new SubB();
        show(a); //It is msg from sub a
        show(b); //It is msg from sub a
    }

    static void  show(Base b) {
        b.show();
    }
}