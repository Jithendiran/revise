/**
* Author: E.K.Jithendiran
* Date: 20.02.2023
*/
package basic;

import java.util.Scanner;

public class Object {
    public static void main(String[] args) {
        // javac basic/Object.java
        // when program is compile java automatically compile Class.java
        Class c1 = new Class(); // create object for class
        System.out.println("C1 a = " + c1.a + " b = " + c1.b);

        Class c2 = new Class(5);
        System.out.println("C2 a = " + c2.a + " b = " + c2.b);

        String s;
        // System.out.println(s);
        // Error:The local variable s may not have been initialized
        s = "Hello";
        String s1 = s;// s value is copied to s1
        System.out.println("Value pf s : " + s + " Value of s1 : " + s1);
        s1 = "Ji";
        System.out.println("Value pf s : " + s + " Value of s1 : " + s1);

        // Constants in java
        /**
         * Constant values are not changed
         * final keyword is used for constants
         */
        final String AUTHOR_CODE = "Jithendiran";
        // AUTHOR_CODE="Ji"; Error
        System.out.println("Constant value : " + AUTHOR_CODE);

        /**
         * Data Types
         * 
         * Intergers (1,2,-5,...)
         * Real Numbers / Floating point numbers (0.0,1.0, -12.3,'')
         * Characters ('a','5','$',...)
         * String ("Hello","",..)
         * Booleans (true, false)
         * User defind types (class)
         */
        /**
         * DataType Size Range
         * 
         * byte 1 byte -128 to 127
         * 
         * short 2 bytes -32,768 to 32,767
         * 
         * int 4 bytes -2,147,483,648 to 2,147,483,647
         * 
         * long 8 bytes -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
         * 
         * float 4 bytes Stores fractional numbers. Sufficient for storing 6 to 7
         * decimal digits
         * 
         * double 8 bytes Stores fractional numbers. Sufficient for storing 15 decimal
         * digits
         * 
         * boolean 1 bit Stores true or false values
         * 
         * char 2 bytes Stores a single character/letter or ASCII values
         */

        // Intergers
        int n; // isnt is 4 byte (-2147483648 to 2147483647)
        n = 2147483647;
        n++; // overflow so start again from range
        System.out.println("n after increment : " + n); // -2147483648
        // n = 2147483648;
        // Error: The literal 2147483648 of type int is out of range

        // all numbers without decimal point is treated as int
        // long l = 2147483648; error
        long l = 2147483648l; // l or L is appeded to tell compiler it is not an int
        // type conversion
        // byte -> short -> int -> long
        // long can store an int, short and byte because it's size is greater than all
        byte b1 = 1;
        short s2 = 2;
        int i1 = 3;
        long l1 = 4;

        // below converion are ok
        // (implict casting) converting smaller to wider datatype
        l1 = i1 + s2 + b1;
        i1 = s2 + b1;
        s2 = b1;

        // below ate not ok
        // il = l1
        // s2 = i1;
        // b1 = i1;
        System.out.printf("Int Describe :: min value : %d, max value : %d, size : %d, Bytes : %d\n", Integer.MIN_VALUE,
                Integer.MAX_VALUE, Integer.SIZE, Integer.BYTES);

        // Real numbers
        System.out.printf("Double Describe :: min value %f, max value: %f\n", Double.MIN_VALUE, Double.MAX_VALUE);
        // every floating point number is consider as double in java
        float f = 1.34f; // f or F is used to tell compiler as a float

        // character
        // java uses Unicode encoding Scheme
        char c = 'A'; // values Range ['\u0000' to '\uffff'] OR [0 to 65535]
        char c11 = 65, c12 = '\u0041';
        System.out.println("Chararcter : " + c + " " + c11 + " " + c12); // Chararcter : A A A
        int c111 = 'A', c211 = 65, c311 = '\u0041';
        System.out.println("Int to char : " + c111 + " " + c211 + " " + c311);

        // string
        String s22 = "Hello";
        System.out.println("Upper : " + s22.toUpperCase());
        System.out.println("IS empty : " + s22.isEmpty() + " " + s22.isBlank());
        s22 = "";
        System.out.println("IS empty : " + s22.isEmpty() + " " + s22.isBlank());
        s22 = "  ";
        System.out.println("IS empty : " + s22.isEmpty() + " " + s22.isBlank());
        s22 = "Happy Morning";
        // s22[0]; error
        char c13 = s22.charAt(0);
        System.out.println("Index of a : " + s22.indexOf('a'));
        System.out.println("Index of a : " + s22.indexOf("pp"));
        System.out.println("Index of a : " + s22.lastIndexOf('n'));
        // string concat
        // using +
        s22 = s22 + " all";
        System.out.println(s22);
        // op: Happy Morning all
        s22 = s22.concat(" By Jith");
        System.out.println(s22);
        // op: Happy Morning all By Jith

        s22 += '.';
        System.out.println(s22);
        // op: Happy Morning all By Jith.
        // s22.concat('.'); // error
        s22 = "hi" + 5;// here 5 is converted as string
        System.out.println(s22);
        // op: hi5
        System.out.println("Concat " + 1 + 2);
        // op: Concat 12
        System.out.println("Concat " + (1 + 2));
        // op: Concat 3

        /**
         * Primitive types
         * byte, short, int, long, float, double and char
         * These are store simple values
         */
        int j1 = 3;
        int j2 = j1;
        j2++;
        // j1 and j2 has different values
        System.out.printf("J1 %d and J2 %d\n", j1, j2);
        // J1 3 and J2 4
        /**
         * Reference Types
         * String, class
         * These are store complex values like object
         * It will hold address and that address contains actual value
         */
        // strings are immutable
        String s123 = new String("Hello");
        String s1234 = s123;// s1234 holds address of "Hello"
        s1234 = "welcome"; // here reference of old s1234 is removed and created new value
        // that old reference value is colleceted by garbage collector
        System.out.println("string value s123 : " + s123 + " s1234 : " + s1234);
        // string value s123 : Hello s1234 : welcome

        String ss = "HI";
        String sr = ss;
        ss = "JI";
        System.out.println("SS : " + ss + " SR: " + sr); // SS : JI SR: HI

        // Input
        Scanner input = new Scanner(System.in);// system.in is keyboard
        System.out.println("Enter string followed by int");
        s = input.next(); // read string
        i1 = input.nextInt();// read int
        // input.nextByte();input.nextFloat();input.nextDouble();input.nextShort(),...
        // don't have nextChar(); for character
        // to read char input.next().charAt(0);
        System.out.println("String : " + s + " Int : " + i1);
        System.out.println("Enter string with space : " + input.nextLine());
        // next() is read until space nextLine() is read until new line
        input.close();

        // casting
        /**
         * Implicit casting happens automatcally when narrow (small) data type is
         * converted to wide data type
         * eg: int -> double/float/long
         */
        double d1 = 4;
        double d2 = 4.0f;
        long l2 = 12;
        /**
         * Explicit casting is not automatic. when converting from wider to narrow data
         * type eg: double/float/long -> int
         */
        // float f1 = 4.1; 4.1 is double f1 is float
        // int i3 = 8l; // 8 is long 13 is int
        float f1 = (float) 4.1;
        int i3 = (int) 8l;
        int i4 = (int) 4.5;
        System.out.println("F1: " + f1 + " i3: " + i3 + " i4:" + i4);
        // Op: F1: 4.1 i3: 8 i4:4
    }
}

/**
 * Op
 * Default constructor of Class
 * C1 a = 0 b = 3
 * Parameter constructor of Class
 * C2 a = 5 b = 3
 * Value pf s : Hello Value of s1 : Hello
 * Value pf s : Hello Value of s1 : Ji
 * Constant value : Jithendiran
 * n after increment : -2147483648
 * Int Describe :: min value : -2147483648, max value : 2147483647, size : 32,
 * Bytes : 4
 * Int to char : 65 65 65
 * Upper : HELLO
 * IS empty : false false
 * IS empty : true true
 * IS empty : false true
 * Index of a : 1
 * Index of a : 2
 * Index of a : 11
 */