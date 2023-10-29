package stream;

import java.util.List;
import java.util.Optional;
import java.util.Arrays;
import java.util.stream.Stream;

public class Stream_1 {

    // Stream used in collection
    public static void main(String[] args) {

        List<Integer> list = Arrays.asList(1, 9, 7, 5);

        // stream has craeted of the list items
        Stream<Integer> sdata = list.stream();
        // long count = sdata.count();
        // System.out.println("Count : "+count);
        // count is consumed stream so for each will not work either comment count or
        // foreach
        sdata.forEach(n -> System.out.println(n));
        /**
         * 1
         * 9
         * 7
         * 5
         */

        // sdata.forEach(n -> System.out.println(n)); // Error: stream is already used

        // sort
        Stream<Integer> sorted = list.stream().sorted();
        // .stream() will copy list and convert to stream
        // .sorted() will consume created stream and sort then return new
        sorted.forEach(n -> System.out.println(n));
        /**
         * 
         * 1
         * 5
         * 7
         * 9
         */

        // using stream functional programming concepts can use like map, reduce, filter

        // map
        Stream<Integer> mapdata = list.stream().map(n -> n * 2);
        mapdata.forEach(n -> System.out.println(n));
        /**
         * 2
         * 18
         * 14
         * 10
         */
        System.out.println("\n");
        // filter
        Arrays.asList(1, 2, 3, 4, 5).stream().filter(n -> n % 2 == 0).forEach(n -> System.out.println(n));
        /**
            2
            4
         */
        // reduce
        Integer sum = Arrays.asList(1, 2, 3, 4, 5).stream().reduce(0, (a, b) -> a + b );
        System.out.println("Sum : "+sum); // Sum : 15
    }
}
