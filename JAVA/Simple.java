/**
 * Simple
 */
// Name of the class and File name must be same
public class Simple {
    /**
     * It is the main function where control enter's
     * 
     * @param args command line arguments
     */
    public static void main(String[] args) {
        // String must br enclosed inside double quotes
        // It will print and go to new line
        System.out.println("Hello "); // Hello
        System.out.printf("%d %s\n", 4, "Hello 2");// 4 Hello 2
        // print without new line
        System.out.print("Hello3"); // Hello3

        // command line arguments
        if (args.length > 0) {
            /**
             * How to pass arguments?
             * compile: javac Simple.java
             * Run : java Simple args -> args are separated by space and it is optional
             */
            System.out.println("Command line arguments are");
            for (String value : args) {
                System.out.print(value + "\t");
            }
        }
    }

}