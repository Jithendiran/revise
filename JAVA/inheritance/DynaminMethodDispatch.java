/**
 * Author: E.K.Jithendiran
 * Date: 01.03.2023
 */
package inheritance;

public class DynaminMethodDispatch {
    // runtme polymorphism
    public static void main(String[] args) {
        GrandParent gp = new GrandParent();
        /**
         * Op
           Constructor of Grand parent
         */
        Parent p = new Parent();
        /**
         * Op
           Constructor of Grand parent
           Constructor of parent
         */
        Child c = new Child();
        /**
         * Op
            Constructor of Grand parent
            Constructor of parent
            Constructor of child        
         */

        /**
         * dynamin method dispatch
         *  Method overriding is one of the ways in which Java supports Runtime Polymorphism.
         *  Dynamic method dispatch is the mechanism by which a call to an overridden method is resolved at run time, rather than compile time.
         * 
         * Syntax
         *  SuperClass obj = new SubClass();
         */
        
         GrandParent gp1;
         gp1 = gp; // it will link only at run time
         // The above line is like `gp1 = (GrandParent) gp;` it is type casting. It is Upcasting
         gp1.grandParent(); // Grand parent method
         gp1.sameClassName(); //Grand Parent same class

         gp1 = p;
         gp1.sameClassName();
         /**
          * Op
              Grand Parent same class
              Parent same class
          */

          //gp1.parent(); //Error:The method parent() is undefined for the type GrandParent
          /**
           * The above calling is error because object is Parent class and storing reference is GrandParent
           * because GrandParent class has the structure for sameClassName method 
           * there is no structure for parent method in GrandParent 
           */

           gp1.grandParent(); // Grand parent method

           gp1 = c;
           gp1.sameClassName();
           /**
            * Op
                Grand Parent same class
                Parent same class
                Child same class
            */
        
            Child c1;
            // c1 = p; //Error: Type mismatch: cannot convert from Parent to Child
            // only super class can store ref of sub class and access super class structred method of super and sub class

            // upcasting
            Parent p1 = new Child();
            p1.sameClassName();
            /**
             * Op
                Grand Parent same class
                Parent same class
                Child same class
             */
            //Parent p2 = new GrandParent();// error

            //Child c2 = p1;// error 
            /**
             *  The above line are trying to store super class variable to subclass variable. it is not possible
             *  But we can do an trick here called down casting
             *  Lets take this Parent p1 = new Child();Child c2 = p1; 
             *  p1 object is Child class but reference to Parent class
             *  so Child c2 = p1; will raise error because sub class trying to store reference of parent calss but actually it is object of sub class
             *  we can use downcasting to escape from error
             *    Child c2 = (Child)p1;
             */ 
            Child c2 = (Child)p1; // Downcating
            c2.child(); //Child method
            c2.sameClassName();
            /**
             * Op
                Grand Parent same class
                Parent same class
                Child same class
             */
            c2.parent(); //Parent
            // The above line is also possible because Child extends Parent

    }
}
