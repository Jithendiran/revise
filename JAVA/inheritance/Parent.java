/**
 * Author: E.K.Jithendiran
 * Date: 01.03.2023
 */
package inheritance;
// example of single level inheritance
public class Parent extends GrandParent{
    int i = 2;
    Parent() {
        System.out.println("Constructor of parent");
    }

    Parent(int i) {
        super(i);// without this super(i) method, Default constructor of GrandParent class will be called  
        // super(i) it is optional
        System.out.println("Constructor of parent int");
    }
    protected void parent() {
        System.out.println("Parent");
    }

    /**
     * This function is exact same function of GrandParent class
     * This concept is called method over riding 
     */
    protected void sameClassName() {
        super.sameClassName(); // It will call Grandparent sameClassName method
        //  super.sameClassName(); It is optional without that line GrandParent sameClassName method is not called 
        System.out.println("Parent same class");
    }
    // overriding
    protected void sameClassName2() {
        System.out.println("Parent same class 2");
    }

    protected int getI(boolean isThisClass){
        if(isThisClass) 
            return i;
        return super.i; // i of GrandParent
    }

    public static void main(String[] args) {
        System.out.println("It is the Example of single level");//     It is the Example of single level
        Parent p = new Parent();
        /**
         * Op
            Constructor of Grand parent
            Constructor of parent
         */
        p.parent();// Parent
        p.grandParent();//  Grand parent method
        p.sameClassName();
        /**
         * Op
            Grand Parent same class
            Parent same class
         */
        p.sameClassName2();
        System.out.println(p.getI()); //1
        System.out.println(p.getI(false));// 1
        System.out.println(p.getI(true));// 2

    }
}