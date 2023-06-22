/**
* Author: E.K.Jithendiran
* Date: 22.02.2023
*/
package basic;

import java.util.Arrays;
import java.util.Collections;

public class Array {
    /**
     * Before Start java array review c array concepts
     * Java array is also based on address
     */
    public static void main(String[] args) {
        // array is an class Array
        int[] a;// now a is point to NULL
        a = new int[5]; // now a is point to 5 integer address
        // array values are stored continous in memory

        // we can also declare like this
        // int[] a = new int[5];
        // int a[] = new int[5];
        /**
         * Default values of array
         * 0 for numeric primitive data types
         * \u0000 for char types
         * false for boolean types
         * null for reference types(object, string)
         */
        int b[] = { 1, 2, 3, 4 }; // it is array init

        // below init will throw an error
        // int c[];
        // c = {1,2,3};

        // access array
        a[1] = 1;
        a[2] = 2;
        a[3] = 3;
        a[4] = 4;
        for (int i = 0; i < a.length; i++) {
            System.out.println("a index : " + i + " value  : " + a[i]);
        }
        /**
         * Op
         * a index : 0 value : 0
         * a index : 1 value : 1
         * a index : 2 value : 2
         * a index : 3 value : 3
         * a index : 4 value : 4
         */
        // for each loop
        for (int i : b) {
            System.out.println("B values of array : " + i);
        }

        // to print address of array
        System.out.println("Address of array a is " + a);// Address of array a is [I@1dbd16a6

        // System.out.println(a[6]); java.lang.ArrayIndexOutOfBoundsException
        char[] c = { 'j', 'i', 't', 'h' };
        System.out.println("Char array : " + c);// Char array : [C@7ad041f3
        // System.out.println(c[-1]);java.lang.ArrayIndexOutOfBoundsException

        // array is an object so it is posible to pass by reference
        System.out.println("Befor a inc");
        printArray(a);
        /**
         * Op
         * Befor a inc
         * 0
         * 1
         * 2
         * 3
         * 4
         */
        incrementArrayElement(a);
        System.out.println("After a inc");
        printArray(a);
        /**
         * Op
         * After a inc
         * 1
         * 2
         * 3
         * 4
         * 5
         */

        // Array object methods
        // sorting array
        int a1[] = { 5, 4, 8, -1, 0 };
        char c1[] = { 'z', 'a', 'x' };
        String s[] = { "ab", "cd", "aa" };

        Arrays.sort(a1);
        System.out.println("After int array sort");
        printArray(a1);
        /**
         * Op
         * After int array sort
         * -1
         * 0
         * 4
         * 5
         * 8
         */
        Arrays.sort(c1);
        System.out.println("After char array sort : " + Arrays.toString(c1));
        /**
         * Op
         * After char array sort : [a, x, z]
         */
        Arrays.sort(s);
        System.out.println("After string array sort : " + Arrays.toString(s));
        /**
         * Op
         * After string array sort : [aa, ab, cd]
         */
        char c2[] = { 69, 65, 68, 66 };
        Arrays.sort(c2);
        System.out.println("After int char array sort : " + Arrays.toString(c2));
        /**
         * Op
         * After int char array sort : [A, B, D, E]
         */

        // To sort in descending
        // Arrays.sort(a1, Collections.reverseOrder()); not possible
        Integer a2[] = { 7, 2, 9, 8, -1 };
        Arrays.sort(a2, Collections.reverseOrder());
        System.out.println("After int array rev sort : " + Arrays.toString(a2));
        /**
         * Op
         * After int array rev sort : [9, 8, 7, 2, -1]
         */

        Method[] ma = { new Method(), new Method() };
        // Arrays.sort(ma);
        /**
         * Exception in thread "main" java.lang.ClassCastException: class basic.Method
         * cannot be cast to class java.lang.Comparable (basic.Method is in unnamed
         * module of loader 'app'; java.lang.Comparable is in module java.base of loader
         * 'bootstrap')
         */
        // Methods class has no compareTo function so it leads to error

        // searching
        /**
         * For searching we use binarySearch
         * For this we should short the array
         * If the element is found it will return index of that element
         * else it will return -(insertindex + 1)
         * What is insertindex?
         * insert index is the index if the element is inserted then where it wil be
         * found for eg [1,2,3,5,6] -> if 2 is search it return 1
         * if 5 is search it return 4
         * if 4 is search that element is not present so it will return -3
         * 3 is the index where 4 will be found if it is present
         */

        System.out.println("2 is in : " + Arrays.binarySearch(new int[] { 1, 2, 3, 5, 6 }, 2)); // Op:2 is in : 1
        int a3[] = { 3, 5, 1, 6, 2 };
        Arrays.sort(a3);
        System.out.println("5 is found in " + Arrays.binarySearch(a3, 5)); // Op:5 is found in 3
        System.out.println("4 is found in " + Arrays.binarySearch(a3, 4)); // Op:4 is found in -4

        // Equals
        int[] ar1 = { 1, 2, 3 };
        int[] ar2 = { 1, 2, 3 };
        int[] ar3 = { 2, 1, 3 };
        int[] ar4 = { 9, 1, 2 };
        System.out.println("Ar 2 " + Arrays.equals(ar1, ar2));// Ar 2 true
        System.out.println("Ar 3 " + Arrays.equals(ar1, ar3));// Ar 3 false
        System.out.println("Ar 4 " + Arrays.equals(ar1, ar4));// Ar 4 false

        // for custom class
        Test t1 = new Test(1, 2);
        Test t2 = new Test(5, 6);
        Test t3 = new Test(1, 2);

        Test[] ta1 = { t1, t2 };
        Test[] ta2 = { t1, t2 };
        Test[] ta21 = { t3, t2 };

        Test[] ta3 = { new Test(1, 3), new Test(5, 6) };
        System.out.println("ta1 ta2 " + Arrays.equals(ta1, ta2));// t1 t2 true
        System.out.println("ta1 ta21 " + Arrays.equals(ta1, ta21));// ta1 ta21 false
        System.out.println("ta1 ta3 " + Arrays.equals(ta1, ta3));// t1 t3 false
        System.out.println("T1 and T2 : " + t1.equals(t3));

        // fill array
        int[] ar5 = new int[8];
        Arrays.fill(ar5, 3);
        System.out.println("Ar5 fill : " + Arrays.toString(ar5));// Ar5 fill : [3, 3, 3, 3, 3, 3, 3, 3]

        String[] sa1 = new String[5];
        Arrays.fill(sa1, 2, 4, "Jith");
        System.out.println("saa1 fill : " + Arrays.toString(sa1));// saa1 fill : [null, null, Jith, Jith, null]

        System.out.println("SUM : " + sum(1)); // SUM : 1
        System.out.println("SUM : " + sum(1, 2));// SUM : 3
        System.out.println("SUM : " + sum(1, 2, 10));// SUM : 13
        System.out.println("SUM : " + sum(new int[] { 1, 10, -1 }));// SUM : 10

    }

    private static int[] incrementArrayElement(int[] array) {
        // for (int a : array) {
        // a = a + 1; // it is not reflect in a
        // }
        for (int i = 0; i < array.length; i++) {
            array[i] += 1;
        }
        return array;
    }

    private static void printArray(int[] array) {
        for (int i : array) {
            System.out.println(i);
        }
    }

    /**
     * Eg for vaiable length array
     * Rules:
     * * Only one variable length parameter may specfied in a method
     * * It must be last parameter
     * 
     * @param values it is array of the values
     * @return sum of all the value
     */
    private static int sum(int... values) {
        int sum = 0;
        for (int i : values) {
            sum += i;
        }
        return sum;
    }
}

class Test {
    int x;
    int y;

    Test(int x, int y) {
        this.x = x;
        this.y = y;
    }
}
