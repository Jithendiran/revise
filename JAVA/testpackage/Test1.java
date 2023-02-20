package testpackage;

public class Test1 {
    public static void main(String[] args) {
        System.out.println("Main of test one");
        ping();
        // simpleMethod();
        // error: Cannot make a static reference to the non-static method
    }

    /**
     * It is public method to ack user
     */
    public static void ping() {
        System.out.println("Ping in Test 1");
        // simpleMethod(); // Error : will not work like this need object
        Test1 t = new Test1();
        t.simpleMethod(); // simple method in Test 1
    }

    public void simpleMethod() {
        System.out.println("simple method in Test 1");
    }
}

/**
 * op
 * Main of test one
 * Ping in Test 1
 * simple method in Test 1
 */