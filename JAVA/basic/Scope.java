/**
* Author: E.K.Jithendiran
* Date: 22.02.2023
*/
package basic;

public class Scope {
    static void scope(int n) {
        int a = 1;
        int i = 1;
        System.out.println("N is " + n);// N is 1
        // System.out.println(args); not access
        {
            // int i = 2;Duplicate local variable i
            System.out.println("a is : " + a);// a is : 1
        }
        // System.out.println("J is : " + j); j cannot be resolved to a variable
        int j = 0;
    }

    public static void main(String[] args) {
        // args scope is main method
        scope(1);
        int i = 0;
        // for (int i = 0; i < 2; i++) { // Duplicate local variable i
        // System.out.println("For Loop");
        // }
    }
}
