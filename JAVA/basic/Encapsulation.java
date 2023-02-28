/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package basic;

class AB {
 private int i;// i can be private or non private but that should be modify only by function
 // private is best practice
 void setI(int i) {
    this.i = i;
 }
 int getI() {
    return i;
 }
}

public class Encapsulation {
    /**
     * Encapsulation is defined as the wrapping up of data under a single unit. 
     * It is the mechanism that binds together code and the data it manipulates. 
     * Another way to think about encapsulation is, that it is a protective shield that prevents the data from being accessed by the code outside this shield. 
     */
    // Simple definition is
    // The data member of a class must be modified only by method
    /**
     * Advantage of encapsulation
     * Data Hiding
     *   It is a way of restricting the access of our data members by hiding the implementation details. 
     *   Encapsulation also provides a way for data hiding. The user will have no idea about the inner implementation of the class. 
     *   It will not be visible to the user how the class is storing values in the variables.
     *   The user will only know that we are passing the values to a setter method and variables are getting initialized with that value.
     */
    public static void main(String[] args) {
        AB a = new AB();
        a.setI(4);
        System.out.println(a.getI());// 4
    }
}
