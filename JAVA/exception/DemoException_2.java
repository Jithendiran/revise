/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package exception;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class DemoException_2 {
    public static void main(String[] args) throws Exception
    {
        int i = 2, j = 2;

        // try resource
        /**
         * advantage of Try resource is bf is the reader resource
         * When the try block completed bf will be deallocated
         * If any exception is throwed bf will be deallocated
         * Catch is optional for try resource
         * It required Java 1.7 or later
         */
        try (BufferedReader bf = new BufferedReader(new InputStreamReader(System.in))) {
            String s = bf.readLine();// Ip: test
            System.out.println(s); // Op: test
            //bf.close(); // no need
        }
        //  catch (Exception e) {
        // }

        try{
            if(i/j == 1) {
                // throw exception manually
                throw new ArithmeticException("Custom msg"); // or throw new ArithmeticException();
            }

        }catch(ArithmeticException e){
            System.out.println("IN Airthmetic exception "+e);
        }
        /**
         * Op
            IN Airthmetic exception java.lang.ArithmeticException: Custom msg
         */
    }
}
