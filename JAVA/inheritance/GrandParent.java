/**
 * Author: E.K.Jithendiran
 * Date: 01.03.2023
 */
package inheritance;

public class GrandParent {
    int i = 1;
    GrandParent() {
        System.out.println("Constructor of Grand parent");
    }

    GrandParent(int i) {
        System.out.println("Constructor of Grand parent int");
    }
    protected void grandParent() {
        System.out.println("Grand parent method");
    }

    protected void sameClassName() {
        System.out.println("Grand Parent same class");
    }
    protected void sameClassName2() {
        System.out.println("Grand Parent same class 2");
    }
    protected int getI(){
        return i;
    }
}
