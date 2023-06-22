/**
 * Author: E.K.Jithendiran
 * Date: 24.02.2023
 */
package basic;

class Outer {
    int a;
    int x;
    int b;
    Inner inner;
    static int k;
    InnerStatic innerStatic;
    InnerPrivate innerPrivate;

    Outer(int a, int b, int x) {
        this.a = a;
        this.b = b;
        this.x = x;
        inner = new Inner(a + 1, x + 1);
        Inner.y = 1; // access inner static variable
        inner.a = 12; // access inner variable
        // g = 18; inner g will not call like this
        innerPrivate = new InnerPrivate(); // no problem

        // inner static class
        innerStatic = new InnerStatic();
        innerStatic.h = 69;
    }

    void showInner() {
        inner.showVariables();
    }

    void showInnerStatic() {
        innerStatic.show();
    }

    class Inner {
        int a;
        int g;
        private int x;
        static int y;
        final static private int z = 11;

        Inner(int a, int x) {
            this.a = a;
            this.x = x;
        }

        void showVariables() {
            System.out.println("Inner x : " + x);
            System.out.println("Outer x :"); // not able to access outer variable with same name
            System.out.println("Outer b :" + b);
        }

        static void showStaticVariables() {
            System.out.println("Y is " + y);
            System.out.println("Z is " + z);
            System.out.println("K is " + k);
            System.out.println("Static class is : " + InnerStatic.sh);
        }
    }

    private class InnerPrivate {
        int h;
    }

    static class InnerStatic {
        int h;
        static int sh;

        void show() {
            // System.out.println("Outer a : " + a); // error a is non static
            System.out.println("Outer k : " + k + " Inner h : " + h);
        }
    }
}

public class InnerClass {
    public static void main(String[] args) {
        Outer outer = new Outer(1, 2, 3);
        outer.showInner();
        /**
         * Op
         * Inner x : 4
         * Outer x :
         * Outer b :2
         */
        Outer.Inner inner = new Outer(1, 2, 2).new Inner(0, 0);// Inner Class obj
        inner.showVariables();
        /**
         * Op
         * Inner x : 0
         * Outer x :
         * Outer b :2
         */

        // access static
        Outer.k = 45;
        Outer.Inner.y = 3;
        Outer.InnerStatic.sh = 23;
        Outer.Inner.showStaticVariables();
        /**
         * Op
         * Y is 3
         * Z is 11
         * K is 45
         * Static class is : 23
         */
        // Outer.Inner inner1 = Outer.new Inner(0, 0);// error
        // Outer.InnerPrivate = new Outer(0, 0, 0).InnerPrivate(); // error
        /**
         * the above object creation is error bcz that inner class is private
         * for the above class only able to create object inside the outer class
         */

        // access inner static with outer object
        // outer.InnerStatic.sh = 45; // error

        // Outer.InnerStatic ins = new Object().new InnerStatic(); //error
        Outer.InnerStatic innerStatic = new Outer.InnerStatic();
        /**
         * In the above line Object is creatd for outer class
         * then access inner static class by class name
         */
        innerStatic.h = 89;
        innerStatic.sh = 56;
        System.out.println("Static class : " + innerStatic.h); // Static class : 89
        System.out.println("Static class : " + innerStatic.sh); // Static class : 56
        innerStatic.show(); // Outer k : 45 Inner h : 89
        outer.showInnerStatic();// Outer k : 45 Inner h : 89
    }
}
