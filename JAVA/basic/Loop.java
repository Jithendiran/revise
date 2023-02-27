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

         // labeled break
         labelbreak:
         for (int j2 = 0; j2 < 5; j2++) {

            for (int k = 0; k < 5; k++) {
                if(k == 3 && j2 == 2) {
                    break labelbreak; // it is labeled break
                    /**
                     * without label it will only break inner loop
                     * when a label is passed it break loop of afer label located
                     */
                }
                System.out.println("j2 = "+j2+" k = "+k);
            }
         }
         /**
          * j2 = 0 k = 0
          * j2 = 0 k = 1
          * j2 = 0 k = 2
          * j2 = 0 k = 3
          * j2 = 0 k = 4
          * j2 = 1 k = 0
          * j2 = 1 k = 1
          * j2 = 1 k = 2
          * j2 = 1 k = 3
          * j2 = 1 k = 4
          * j2 = 2 k = 0
          * j2 = 2 k = 1
          * j2 = 2 k = 2
          */
          // the above 2 loops are breaked 

          for (int j3 = 0; j3 < 5; j3++) {
              
              labelbreak:
            for (int k = 0; k < 5; k++) {
                if(k == 3 && j3 == 2) {
                    break labelbreak; // it is labeled break
                }
                System.out.println("j3 = "+j3+" k = "+k);
            }
         }
         /**
          * j3 = 0 k = 0
          * j3 = 0 k = 1
          * j3 = 0 k = 2
          * j3 = 0 k = 3
          * j3 = 0 k = 4
          * j3 = 1 k = 0
          * j3 = 1 k = 1
          * j3 = 1 k = 2
          * j3 = 1 k = 3
          * j3 = 1 k = 4
          * j3 = 2 k = 0
          * j3 = 2 k = 1 // k = 3 is missing because label is used above inner loop so after this loop is contibue as usual 
          * j3 = 2 k = 2
          * j3 = 3 k = 0
          * j3 = 3 k = 1
          * j3 = 3 k = 2
          * j3 = 3 k = 3
          * j3 = 3 k = 4
          * j3 = 4 k = 0
          * j3 = 4 k = 1
          * j3 = 4 k = 2
          * j3 = 4 k = 3
          * j3 = 4 k = 4
          */

          first:
         for (int i1 = 0; i1 < 3; i1++) {
            for (int j1 = 0; j1< 3; j1++){
               if(i1 == 1){
                  continue first;
                  /**
                   * Labeled continue is above 1st loop so when condition matched it will continue from 1st loop
                   */
               }      
               System.out.println("[i1 = " + i1 + ", j1 = " + j1 + "]");
            }
         }
         /**
          * [i1 = 0, j1 = 0]
          * [i1 = 0, j1 = 1]
          * [i1 = 0, j1 = 2]
          * [i1 = 2, j1 = 0]
          * [i1 = 2, j1 = 1]
          * [i1 = 2, j1 = 2]
          */
    }
}
