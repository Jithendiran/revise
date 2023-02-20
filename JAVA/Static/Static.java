/**
* Author: E.K.Jithendiran
* Date: 20.02.2023
*/
package Static;

import static java.lang.System.out; // static is because out is an static member
import static Static.Sample.ping;

class A {
    int i; // non static member
    static int j;// static member

    static {
        out.println("Static block in A");
    }

    public void testMethod() {
        /**
         * non static method is able to access static member
         * static member is called by class name
         * it's memory is associated with class it has single memory
         */
        j = -1;
        A.j = -2;

        // i value is vary for object
        i = i + 1;
    }

    public static void staticMethod() {
        // This method is static method meaning it will be called only by class name
        // i is non static member i variable memory is associated with object
        // i = i + 1; Error
        j = -3; // it is possible
    }
}

public class Static {
    static String s;
    static {
        /**
         * It is static block
         * It will be called when a class is loaded by JVM
         * It will be executed before main
         * More than 1 static block is possible
         * It is used for assign value for static member
         */
        out.println("IN Static 1");
    }

    /**
     * Why main function is public static?
     * Java Run time env want to access main function so it is public
     * static is because it will be called directly without object
     * main is the name of the Method, It searched by JVM for execute
     */
    public static void main(String[] args) {

        /**
         * j is an static variable of A class it will be access by class name
         * static variable and method is shared for all
         */

        A.j = 2;
        out.println("A class Static j value is : " + A.j);

        /**
         * i is an non static variable of A class it be access by class object
         * seprate memory for seprate object
         */
        A a1 = new A();
        a1.i = 1;
        A a2 = new A();
        a2.i = 2;
        // two object's are created but only one time static block is executed

        // a1.j; Error :The static field A.j should be accessed in a static way
        // j will not access by objects
        out.println("A class a1 obj i : " + a1.i);
        System.out.println("A class a2 obj i : " + a2.i);

        // A.i; Error

        A.staticMethod(); // j is modified
        out.printf("Values of j = %d after static method is called\n", A.j);

    }

    static {
        System.out.println("IN Static 2");
        s = "Hello";
        ping();// this method is imported
    }
}

/**
 * Op
 * IN Static 1
 * IN Static 2
 * Ping from sample
 * Static block in A
 * A class Static j value is : 2
 * A class a1 obj i : 1
 * A class a2 obj i : 2
 * Values of j = -3 after static method is called
 */