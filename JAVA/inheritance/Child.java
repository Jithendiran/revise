/**
 * Author: E.K.Jithendiran
 * Date: 01.03.2023
 */
package inheritance;
// example of multilevel inheritance
public class Child extends Parent{

    Child() {
        System.out.println("Constructor of child");
    }
    protected void child(){
        System.out.println("Child method");
    }
    /**
     * This function is exact same function of parent class
     * This concept is called method over riding 
     */
    protected void sameClassName() {
        super.sameClassName(); // It will call parent sameClassName method
        // super.sameClassName(); It is optional without that line Parent sameClassName method is not called 
        System.out.println("Child same class");
    }

    // overriding
    protected void sameClassName2() {
        System.out.println("Child same class 2");
    }

    public static void main(String[] args) {
        Child c = new Child();
        /**
         * Op
            Constructor of Grand parent
            Constructor of parent
            Constructor of child
         */
        c.child();// Child method
        c.parent();// Parent
        c.grandParent(); //Grand parent method

        c.sameClassName();
        /**
         * Op
            Grand Parent same class
            Parent same class
            Child same class
         */

        Parent p = new Parent(1);
        /**
         * Op
            Constructor of Grand parent int
            Constructor of parent int
         */
        p.sameClassName2(); //Parent same class 2
        c.sameClassName2(); //Child same class 2
    }
}
