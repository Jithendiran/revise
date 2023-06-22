/**
 * Author: E.K.Jithendiran
 * Date: 24.02.2023
 */
package basic;

class A {
    // This key word is reference to object itself
    private int x;
    private int y;
    static private int z;

    A(int x) {
        this.x = x; // here x is function scope variable
        // this.x is calling object's x

        // member functions can call by name or using this
        ping();
        this.ping();
    }

    A(int x, int y) {
        // using this can call constructor
        // A(x); it will raise an error
        this(x); // this will call constructor
        this.y = y;
    }

    void ping() {
        System.out.println("IN ping");
    }

    int getX() {
        return x;
    }

    int gety() {
        return y;
    }

    static void setZ(int z) {
        // this.z; error
        // cannot use this inside static method
        // because static is belongs to class not for objecy
        A.z = z;
    }

    static int getZ() {
        return z;
    }
}

public class This {
    public static void main(String[] args) {
        System.out.println(new A(1).getX());
        /**
         * Op
         * IN ping
         * IN ping
         * 1
         */

        A a = new A(1, 2);
        /**
         * Op
         * IN ping
         * IN ping
         */
        System.out.println(a.getX()); // 1
        System.out.println(a.gety()); // 2
        A.setZ(5);
        System.out.println(A.getZ()); // 5
    }
}