/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package basic;

public class StringCustom {
    public static void main(String[] args) {
        // strings are immutable
        // which means when strings are defined it will not changed
        // when reassign is happened old values is collected by garbage collector

        // String pool
        String ss1 = "Hello";
        /**
         * When the above statement is executed "Hello" object is stored in Heap string pool memory assume it's addess is 1000
         * and that 1000 is stores in stack that is refered to ss1
         */
        String ss2 = "Hello";
        /**
         * When the above line is executed  it will check is there any exact string in string pool
         * if the exact string is matched java will not create new object in heap
         * in this example "Hello" object is matched so ss2 will also store 1000
         */
        ss2 = "Hi";
        /**
         * When the above line is executed. string pool will check, There is no match string and new object is created 
         * and reference of that is stored to ss2
         */
        System.out.println(ss1); //Hello
        System.out.println(ss2); // Hi
        ss2 = new String("Hello");
        /**
         * new will force to create new entry
         *  */ 


        /**
         * To make string as mutable
        * StringBuffer/ String Builder will be used
        */
        
        // string buffer is thread safe
        // Thread safe means when one thread is writing the value other thread needs to wait
        StringBuffer sb = new StringBuffer(); // or new StringBuffer("Hi");
        sb.append("hi");
        System.out.println(sb);// hi

        sb.insert(2, " hello");
        System.out.println(sb); // hi hello

        sb.replace(0, 2, "Hi");
        System.out.println(sb);// Hi hello

        // string builder also has all the above methods but it is not thread safe
        StringBuilder sd = new StringBuilder(""); // or new StringBuffer("Hi");
        sd.append("hi");
        System.out.println(sd);// hi

        sd.insert(2, " Welcome");
        System.out.println(sd); // hi Welcome

        sd.replace(0, 2, "Hi");
        System.out.println(sd);// Hi Welcome
    }
}
