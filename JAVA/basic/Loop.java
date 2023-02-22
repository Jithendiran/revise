/**
* Author: E.K.Jithendiran
* Date: 22.02.2023
*/
package basic;

public class Loop {
    public static void main(String[] args) {
        int i = 5;
        while (i > 0) {
            System.out.println("While i : " + i);
            i--;
        }
        /**
         * Op
         * While i : 5
         * While i : 4
         * While i : 3
         * While i : 2
         * While i : 1
         */
        do {
            System.out.println("IN do while i : " + i);
            i--;
        } while (i > 0);
        // Op
        // IN do while i : 0

        for (int j = 0; j < 5; j++) {
            System.out.println("For loop : " + j);
        }
        /**
         * Op
         * For loop : 0
         * For loop : 1
         * For loop : 2
         * For loop : 3
         * For loop : 4
         */
        int j[] = { 1, 2, 3, 4, 5 };
        for (int j2 : j) {
            System.out.println("For Each : " + j2);
        }
        /**
         * Op
         * For Each : 1
         * For Each : 2
         * For Each : 3
         * For Each : 4
         * For Each : 5
         */

        for (int j2 = 0; j2 < 10; j2++) {
            if (j2 == 2) {
                continue; // it will use only in loops
            }
            if (j2 == 8) {
                break;// it will use in loops and switch
            }
            System.out.println("Bresk and continue : " + j2);
        }
        /**
         * Op
         * Bresk and continue : 0
         * Bresk and continue : 1
         * Bresk and continue : 3
         * Bresk and continue : 4
         * Bresk and continue : 5
         * Bresk and continue : 6
         * Bresk and continue : 7
         */
    }
}
