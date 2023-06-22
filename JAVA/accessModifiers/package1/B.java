/**
* Author: E.K.Jithendiran
* Date: 28.02.2023
*/
package accessModifiers.package1;

public class B {
    private void privateMethodBClass() {
        System.out.println("Class B private method");
    }

    public void publicMethodBClass() {
        System.out.println("Class B public method");
    }

    protected void protectedMethodBClass() {
        System.out.println("Class B protected method");
    }

    void defaultMethodBClass() {
        System.out.println("Class B Default Method");
    }

    public void accessPrivatedMethodClassB() {
        System.out.println("Class B Private method accessor");
        privateMethodBClass();
    }
}
