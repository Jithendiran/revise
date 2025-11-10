/**
 * Author: E.K.Jithendiran
 * Date: 02.03.2023
 */
package inheritance;

/*
 * Interface 
 * To support multiple inheritance java use interface
 * In interface only declaring method is possible
 * Object is can't created for interfaces
 * 
 */

 /**
  * Types of Interface
  * Marker Interface -> without any method
  * Single Abstract method / Functional Interface -> with only one method
  * Normal Interafce -> with many methods  
  */
 interface Ainterface{
    // This is implicitly 'public static final'
    // data member's accesss specifier are always public static final
    int i = 22;
    //int j; variable must be init
    public int j = 1;
    // by default every method in interface is public abstract
    void ping(); //or public abstract void ping(); // both are same
    public abstract void showInt(int i);
    void pingSame();
 }

 interface Binterface{
    int j = 2;
    void pingB();
    void pingSame();
 }

 interface Cinterface extends Ainterface {
    static int k = 9;
    // defining methods requires JAVA 1.8 or greater
    // defining methods must be default or static
    default void cPing() {
        System.out.println("C ping");
    }

    static void cTest(){
        System.out.println("C test");
    }
    void pingSame();
 }

 /**
  * class to class is extends
  * class to interface is implement
  * interface to interfae is extends
  */
class Implementation implements Ainterface, Binterface {
    // int j = 8;
    @Override
    public void pingB() {
       System.out.println("Ping B");
    }

    @Override
    public void ping() {
        System.out.println("Ping A");
    }

    @Override
    public void showInt(int i) {
        System.out.println("Ping A int : "+i);
    }

    @Override
    public void pingSame() {
        System.out.println("Ping Same method");
    }

} 

class Implementation2 implements Cinterface {

    @Override
    public void ping() {
        System.out.println("Ping A C");
    }

    @Override
    public void showInt(int i) {
        System.out.println("Ping A C int : "+i);
    }

    @Override
    public void pingSame() {
        System.out.println("Ping Same method AC");
    }

}

interface SingleInterface{
    // this is single interface example 
    // means with only one method
    void method();
}

public class Interface {
    public static void main(String[] args) {
        Implementation imp = new Implementation();
        imp.ping();// Ping A
        imp.pingB();// Ping B
        imp.showInt(12);// Ping A int : 12
        imp.pingSame(); //Ping Same method
        System.out.println("Imp i : "+imp.i);//Imp i : 22 // It is not the recomanded way to access, because of static access sepcifier
        System.out.println("Ainterface i : " + Ainterface.i); // Ainterface i : 22

        // System.out.println("J : "+imp.j);// Error: The field imp.j is ambiguous
        // to avoid ambiguous introduce j in Implementation class

        // cannot create object of interface 
        // Ainterface a = new Ainterface();// Error:  Cannot instantiate the type Ainterface
        // but we can make it reference
        
        // This concept is upcasting expalined in inheritance/DynaminMethodDispatch.java
        Ainterface a = new Implementation();
        a.pingSame();//Ping Same method
        a.ping();// Ping A
        System.out.println(a.j);// 1
        // a.j = 7; Error:The final field Ainterface.j cannot be assigned
        // a.pingB();Error

        Implementation2 imp2 = new Implementation2();
        imp2.cPing();// C ping
        // Implementation2.cTest();
        imp2.ping();// Ping A C
        imp2.showInt(3);// Ping A C int : 3
        imp2.pingSame();// Ping Same method AC
        System.out.println("I : "+imp2.i+" J : "+imp2.j+" K: "+imp2.k);// I : 22 J : 1 K: 9
        System.out.println("Cinterface k :"+Cinterface.k);// Cinterface k :9
        Cinterface.cTest();// c test
        //imp2.k = 10; The final field Cinterface.k cannot be assigned

        // annonyms inner class
        Ainterface ainter = new Ainterface() {
            int i = 28;
            @Override
            public void ping() {
                System.out.println("Annonyms Ping a");
            }

            @Override
            public void showInt(int i) {
                System.out.println("Annonyms Ping a int : "+i);

            }

            @Override
            public void pingSame() {
                System.out.println("Annonyms Ping same");
            }

            public void ftest(){ // this is method will not callable because Ainterface interface has not ftest method declare
                System.out.println("Test Ftest");
            }
        };

        ainter.ping();//Annonyms Ping a
        ainter.showInt(25);//Annonyms Ping a int : 25
        ainter.pingSame();// Annonyms Ping same
        // ainter.ftest();Error

        // has an special feature for single interface
        SingleInterface si = new SingleInterface() {
            // this is annonyms inner class
            // when we have only one method in interface we can use shorthand notation or lambda expression for annonyms class
            public void method() {
              System.out.println("Annonyms class single Interface method");
            }
        }; 
        //or
        SingleInterface si2 = () -> { // if any parameter  pass here
            // This implementation require java 1.8 or greater
            // This interface has only one method so this implementation is consider as method defintion
            // si2 is object
            System.out.println("This is annonyms lambda class function");
        };
        // or
        SingleInterface si3 = () -> System.out.println("This is annonyms lambda class function");


        si.method(); // Annonyms class single Interface method
        si2.method(); //This is annonyms lambda class function
        si3.method(); //This is annonyms lambda class function

        // Error
        // Ainterface ai1 = () -> {
        //     /**
        //      *  void ping(); //or public abstract void ping(); // both are same
        //      *  public abstract void showInt(int i);
        //      *  void pingSame();
        //      * here more than one method is there so lambda expresion is not possibles
        //      */
        // };
    }
}