/**
* Author: E.K.Jithendiran
* Date: 20.02.2023
*/
package basic;

public class Class {
    /**
     * Class is an blueprint of object
     * Everything is class in java
     */
    int a; // data member default value 0
    int b = 3;

    Class() {
        System.out.println("Default constructor of Class");
    }

    Class(int a) {
        this.a = a;
        System.out.println("Parameter constructor of Class");
    }

    // member function
    public void sampleMethod(String s) {
        System.out.println("Print : " + s);
    }
}
