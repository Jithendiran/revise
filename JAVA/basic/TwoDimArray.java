/**
* Author: E.K.Jithendiran
* Date: 23.02.2023
*/
package basic;

import java.util.Arrays;

public class TwoDimArray {
    /**
     * Before Start java 2d array review c 2d array concepts
     * Java array is also based on address
     */
    public static void main(String[] args) {
        int[][] ar1;
        ar1 = new int[2][2];
        // int[][] ar1 = new int[2][2];
        ar1[0][0] = 1;
        ar1[0][1] = 2;
        ar1[1][0] = 3;
        ar1[1][1] = 4;

        System.out.println("Array : " + Arrays.toString(ar1));// Array : [[I@1dbd16a6, [I@7ad041f3]
        System.out.println(ar1.length);// 2
        print2DArray(ar1);
        System.out.println();
        /**
         * Op
         * Index : [0][0] = 1
         * Index : [0][1] = 2
         * Index : [1][0] = 3
         * Index : [1][1] = 4
         */

        // Initlize 2d array
        int[][] ar2 = { { 4, 5 }, { 6, 7 } };
        print2DArray(ar2);
        System.out.println();
        /**
         * Index : [0][0] = 4
         * Index : [0][1] = 5
         * Index : [1][0] = 6
         * Index : [1][1] = 7
         */
        int[][] ar3 = getArray();
        print2DArray(ar3);
        System.out.println();
        /**
         * Index : [0][0] = 1
         * Index : [0][1] = 2
         * Index : [1][0] = 3
         * Index : [1][1] = 4
         * Index : [1][2] = 5
         * Index : [2][0] = 6
         * Index : [2][1] = 7
         * Index : [2][2] = 8
         * Index : [2][3] = 9
         * Index : [2][4] = 10
         */

         int[][] ar4 = new int[3][];
         ar4[0] = new int[2];
         ar4[1] = new int[4];
         ar4[2] = new int[3];
        Arrays.fill(ar4[0], 2);
        Arrays.fill(ar4[1], 3);
        Arrays.fill(ar4[2], 4);
        print2DArray(ar4);
        /**
         * Index : [0][0] = 2 
         * Index : [0][1] = 2 
         * Index : [1][0] = 3 
         * Index : [1][1] = 3 
         * Index : [1][2] = 3 
         * Index : [1][3] = 3 
         * Index : [2][0] = 4 
         * Index : [2][1] = 4 
         * Index : [2][2] = 4 
         */
    }

    /**
     * This Function is responsible to print 2d int array
     * 
     * @param arr 2d int array
     */
    public static void print2DArray(int[][] arr) {
        for (int i = 0; i < arr.length; i++) {
            for (int j = 0; j < arr[i].length; j++) {
                System.out.printf("Index : [%d][%d] = %d \n", i, j, arr[i][j]);
            }
        }
    }

    public static int[][] getArray() {
        return new int[][] { { 1, 2 }, { 3, 4, 5 }, { 6, 7, 8, 9, 10 } };
    }
}