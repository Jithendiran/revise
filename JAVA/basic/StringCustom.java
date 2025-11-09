/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package basic;

public class StringCustom {
    public static void main(String[] args) {
        // strings literal and new String(..) are immutable
        // which means when strings are defined it will not changed
        // when reassign is happened old values is collected by garbage collector

        // String constant pool
        // 1. Literal creation (uses the String Pool)
        String ss1 = "Hello";
        /**
         * When the above statement is executed "Hello" object is stored in Heap string constant pool memory assume it's addess is 1000
         * and that 1000 is stores in stack that is refered to ss1
         */
        String ss2 = "Hello";
        /**
         * When the above line is executed  it will check is there any exact string in string constant pool
         * if the exact string is matched java will not create new object in heap
         * in this example "Hello" object is matched so ss2 will also store 1000
         */
        // == compare the address
        System.out.println(ss1 == ss2); // compare address in String Constant Pool
        // true
        System.out.println(ss1.equals(ss2)); // it compares the contant
        // true
        ss2 = "Hi";
        /**
         * When the above line is executed. string constant pool will check, There is no match string and new object is created 
         * and reference of that is stored to ss2
         */
        System.out.println(ss1); //Hello
        System.out.println(ss2); // Hi
        System.out.println(ss1 == ss2); // false
        System.out.println(ss1.equals(ss2)); // false

        //------------------------------------------------------------------------------------------------
        // 2. Object creation (uses the regular Heap)
        ss2 = new String("Hello");
        /**
         * new will force to create new entry
         * ss1 is in string constant pool and ss2 is in heap, both are different. so now two "Hello" is stored
         * string constant pool also in heap, but it's a seperate mapping
         *  */ 

        System.out.println(ss1 == ss2); // false
        System.out.println(ss1.equals(ss2)); // true


        String s2 = new String("Hello");
        s2 = s2.concat(" World");
        /*
        You are not changing the original "Hello" object. Instead:
        1. The concat() method creates a brand new String object containing "Hello World".
        2. The variable s2 is reassigned to point to this new "Hello World" object.
        3. The original "Hello" object created by new String("Hello") remains unchanged in the heap, eventually becoming eligible for Garbage Collection if no other variable references it.
        */
        //-------------------------------------------------------------------------------------------------
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
