/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package exception;

public class DemoException_1 {
    public static void main(String[] args) {
        int i,j;
        i = 10;
        j = 2;
        System.out.println(i/j);// 5
        j = 0;
        int[] a = new int[2];
        // System.out.println(i/j); // exception: java.lang.ArithmeticException: / by zero
        // any thing divide gy 0 is infinity. There is no infinity concept in Java. so it will throw exception

        // to handle exception cover critical state ment with try block
        try {
            System.out.println("BF");
            System.out.println(i/j);
            System.out.println("AF");
        } catch (ArithmeticException ae) {
            System.out.println("Fall in Airthmetic exception");
        } 
        System.out.println("Outside try block");
        /**
         * Op
            BF
            Fall in Airthmetic exception
            Outside try block
         */

        try {
            System.out.println("BF");
           System.out.println(a[3]);
            System.out.println("AF");
        } catch (ArithmeticException ae) {
            System.out.println("Fall in Airthmetic exception");
        } catch(Exception e){
             /**
             * Exception class is base class of all exception
             * If any excetion is not caught in above catch this block will caught 
             */
            // if this Exception block is not written program will broke
            // This Exception block must be in last block of catch 
            System.out.println("Exception : "+e);
        }
        finally{
            /**
             * This block will run alwasy. Either exception or not 
             */
            System.out.println("Final block");
        }
        System.out.println("Outside try block");
        /**
         * Op
            BF
            Exception : java.lang.ArrayIndexOutOfBoundsException: Index 3 out of bounds for length 2
            Final block
            Outside try block
         */

         try {
            System.out.println(1/0);
         } catch (ArithmeticException e) {
            System.out.println("IN ae exception");
            //System.out.println(1/0); // it will throw exception it will no caught on outer cath
            try {
                System.out.println(1/0);
            } catch (Exception e1) {
                System.out.println("Inner exception");
            }
         }
         catch(Exception e){
            System.out.println("Outer exception");
         }
         /**
          * Op
            IN ae exception
            Inner exception
          */
    }
}
