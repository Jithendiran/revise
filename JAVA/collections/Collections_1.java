/**
 * Author: E.K.Jithendiran
 * Date: 07.03.2023
 */
package collections;
// Collection is introduced in 1.2 and Generics from 1.5
// Collection is an interface
// Collctions is a class

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Collection;

public class Collections_1 {
    /**
     * Advantage of collection is it has many implementations like (array,list,set,..)
     * It is dynamic type of array
    */
    public static void main(String[] args) {
        Collection c = new ArrayList();
        c.add(27);
        c.add("Ji");

        // fetching
        Iterator iter = c.iterator();
        // System.out.println(iter.next()); // 27
        // System.out.println(iter.next()); // Ji
        //System.out.println(iter.next()); // Exception in thread "main" java.util.NoSuchElementException

        while(iter.hasNext()) {
            System.out.println(iter.next());
        }
        /**
         * Op
            27
            Ji
         */

         for (Object i : c) {
            System.out.println(i);
         }
          /**
         * Op
            27
            Ji
         */

         // Generics is used to mention what type of data in collection
         Collection<Integer> c1 = new ArrayList<Integer>();
         // inside <> is a class we can use inbuilt class or our custom class
         c1.add(1);
         c1.add(2);
         //c1.add('i');  // Error

         // Collections add method is append the element in last
         // if we need to insert element at index then we need to go for List
         List<Integer> cl = new ArrayList<Integer>();
         cl.add(1);
         cl.add(3);
         cl.add(1,2);

         for (int i : cl) {
            System.out.println(i);
         }
         /**
          * Op
                1
                2
                3
          */
         cl.forEach(System.out::println); // Stream API requires 1.8 or greater
         /**
          * Op
                1
                2
                3
          */
    }
}
