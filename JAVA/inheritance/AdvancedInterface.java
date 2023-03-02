/**
 * Author: E.K.Jithendiran
 * Date: 02.03.2023
 */
package inheritance;

// defining methods requires JAVA 1.8 or greater
// defining methods must be default or static

// improper naming convention only for study purpose
interface Ainterface1 {
    default void show(){
        System.out.println("A interface show");
    }
    
    // defining methods should not over ride Object class methos
    // default boolean equals(Object obj){
    //     return obj;
    // }

    static void staticMethod(){
        System.out.println("Inside static method");
    }
    
}

interface Binterface1{
    default void show(){
        System.out.println("B interface show");
    }
}

class AdvanceInterafceClass1 implements Ainterface1{
    void test() {
        System.out.println("TEst");
    }
}

class AdvanceInterafceClass2 implements Ainterface1,Binterface1 {
    // here both interface has common method
    // java will confuse here to call which method
    // to avoid confusion programmer should implement that ambigious method
    public void show() {
        System.out.println("Show in class 2");
    }
}

class Simple {
    public void show(){
        System.out.println("Show from Simple class");
    }
}

class SimpleTest extends Simple implements Ainterface1 {
    // power of class is greate than interface

     // if show() method is  not public it will throw error 
    // error: The inherited method Simple.show() cannot hide the public abstract method in Ainterface1
}

public class AdvancedInterface {
    public static void main(String[] args) {

        Ainterface1.staticMethod(); // Inside static method

        AdvanceInterafceClass1 aic1 = new AdvanceInterafceClass1();
        aic1.show();// A interface show
        aic1.test();// TEst

        AdvanceInterafceClass2 aic2 = new AdvanceInterafceClass2();
        aic2.show();// Show in class 2

        SimpleTest st1 = new SimpleTest(); 
        st1.show(); // Show from Simple class
    }
}
