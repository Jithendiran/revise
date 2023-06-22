/**
* Author: E.K.Jithendiran
* Date: 22.02.2023
*/
package basic;

public class Method {
    // method can be vary based on parameter not by return type
    // this is called method over loading
    public static void ping() {
        System.out.println("IN default ping");
    }

    public static int ping(int n) {
        System.out.println("IN Paramete int ping");
        return n;
    }

    public static void ping(float f) {
        System.out.println("IN Paramete float ping");
    }

    // no error is shown but this function will not call
    // public static float ping(float f) {
    // System.out.println("IN Paramete float, return float ping");
    // }

    public static void ping(float f, float g) {
        System.out.println("IN Paramete float, float ping");
    }

    int i = 1, j = 2;

    public static void main(String[] args) {
        ping(); // IN default ping
        ping(1);// IN Paramete int ping
        ping(1.2f); // IN Paramete float ping
        // automatically convert parameter to float
        ping(1, 2);// IN Paramete float, float ping

        // call by reference
        /**
         * In c/c++ it is done by passing address
         * Java it is not possible to pass address by using '&'
         * In Java it is done by using object
         */
        Method m = new Method();
        m.i = 1;
        m.j = 2;
        System.out.printf("Before Swap  i = %d and j = %d\n", m.i, m.j); // Before Swap i = 1 and j = 2
        callbyRef(m);
        System.out.printf("After Swap  i = %d and j = %d\n", m.i, m.j); // After Swap i = 2 and j = 1
    }

    private static void callbyRef(Method m) {
        int tmp = m.i;
        m.i = m.j;
        m.j = tmp;
    }
}
