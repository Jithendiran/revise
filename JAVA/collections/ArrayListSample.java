/**
* Author: E.K.Jithendiran
* Date: 23.02.2023
*/
package collections;

import java.util.ArrayList;
import java.util.Collections;

/**
 * Array list is resizable array
 * It can store objects like Integer, Double, Float, String, ..
 * It can't store primitive type like int, float, double,..
 */
public class ArrayListSample {
    public static void main(String[] args) {
        ArrayList<Integer> ai;
        ai = new ArrayList<>();
        // This type of syntax support only after 1.7 
        // ArrayList<Integer> ai = new ArrayList<>();
        // ArrayList ai = new ArrayList<Integer>();
        // syntax for before 1.7
        // ArrayList<Integer> ai = new ArrayList<Integer>()

        // add element
        // it will append in last
        ai.add(1);
        ai.add(12);
        ai.add(3);
        System.out.println(ai);// [1, 12, 3]
        // to add element in specified index
        ai.add(0, 2);
        System.out.println(ai);// [2, 1, 12, 3]
        // ai.add(7, 2); //java.lang.IndexOutOfBoundsException: Index: 7, Size: 4

        // accessing array list elements
        // System.out.println(ai[0]);// Unresolved compilation problem
        // use get()
        System.out.println(ai.size());// 4
        System.out.println();
        for (int i = 0; i < ai.size(); i++) {
            System.out.println(ai.get(i));
        }
        /**
         * Op
         * 2
         * 1
         * 12
         * 3
         */
        System.out.println();

        // Modify array list
        ai.set(0, 6);
        System.out.println(ai.get(0)); // 6
        System.out.println();

        // remove element of array list
        // remove by index
        ArrayList<String> sa = new ArrayList<>();
        sa.add("he");
        sa.add("ll");
        sa.add("o");
        sa.add("pe");
        sa.add("te");
        sa.add("r");

        sa.remove(2);// o is removed
        for (int i = 0; i < sa.size(); i++) {
            System.out.println(sa.get(i));
        }
        /**
         * he
         * ll
         * pe
         * te
         * r
         */
        System.out.println();
        // remove by value
        sa.remove("r");
        for (int i = 0; i < sa.size(); i++) {
            System.out.println(sa.get(i));
        }
        /**
         * Op
         * he
         * ll
         * pe
         * te
         */
        System.out.println();

        // to remove all elements
        sa.clear();
        System.out.println(sa.size());// 0

        // sort array list
        sa.add("z");
        sa.add("b");
        sa.add("a");
        sa.add("d");
        sa.add("c");
        sa.add("e");
        Collections.sort(sa);
        System.out.println("String array : " + sa); // String array : [a, b, c, d, e, z]

        // for each
        for (String s : sa) {
            System.out.println(s);
        }
        /**
         * Op
         * a
         * b
         * c
         * d
         * e
         * z
         */

        // find if element is find in Array list
        System.out.println(sa.contains("d"));// true
        System.out.println(sa.contains("j"));// false

        // reverse sort
        Collections.sort(sa);
        Collections.reverse(sa);
        for (String s : sa) {
            System.out.println(s);
        }
        /**
         * Op
            z
            e
            d
            c
            b
            a         
         */
    }
}
