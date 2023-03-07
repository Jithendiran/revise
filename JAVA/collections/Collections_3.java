/**
 * Author: E.K.Jithendiran
 * Date: 07.03.2023
 */
package collections;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

// Comparator vs compareTo
class CompareJi implements Comparator<Integer> {

    /**
     * This Function is responsible to compare two objects
     * @param o1 object one
     * @param o2 object two
     * @return 1 if o1 is greater else -1
     */
    @Override
    public int compare(Integer o1, Integer o2) {
        /**
         * o2 > o1 -> -1
         * o1 > o2 -> 1
         * o1 == o2 -> 0
         */
        if (o1 > o2) {
            return 1;
        }
        return -1;
    }

}

class Eg{
    int i;
    Eg(int i) {
        this.i = i;
    }
}

class Eg2 implements Comparable<Eg2> {
    int i;

    Eg2(int i) {
        this.i = i;
    }
   /**
     * This Function is responsible to compare current object with passed object
     * @param o passed ref
     * @return 1 if o is greater else -1
     */
    @Override
    public int compareTo(Eg2 o) {
       return this.i > o.i ? 1 : -1;
    }
}

public class Collections_3 {
    public static void main(String[] args) {
        
        ArrayList<Integer> al1 = new ArrayList();
        al1.add(3);
        al1.add(1);
        al1.add(8);
        al1.add(2);
        al1.add(5);

        CompareJi cji = new CompareJi();

        Collections.sort(al1, cji);
        
        for (Integer i : al1) {
            System.out.println(i);
        }
        /**
         * Op
            1
            2
            3
            5
            8
         */

         ArrayList<Eg> aleg = new ArrayList<>();
         aleg.add(new Eg(5));
         aleg.add(new Eg(3));
         aleg.add(new Eg(1));
         aleg.add(new Eg(2));

         Collections.sort(aleg, ((ob1, ob2) ->  ob1.i > ob2.i ? 1 : -1));
         for (Eg eg : aleg) {
            System.out.println(eg.i);
         }
         /**
          * Op
            1
            2
            3
            5
          */

          ArrayList<Eg2> aleg2 = new ArrayList<>();
          aleg2.add(new Eg2(15));
          aleg2.add(new Eg2(13));
          aleg2.add(new Eg2(11));
          aleg2.add(new Eg2(12));

          Collections.sort(aleg2);
          for (Eg2 eg : aleg2) {
            System.out.println(eg.i);
         }
         /**
          * Op
            11
            12
            13
            15
          */
    }
}
