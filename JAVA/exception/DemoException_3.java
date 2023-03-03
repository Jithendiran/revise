/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package exception;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class DemoException_3 {
    /**
     * throws keyword is used to supress the error.
     * which means previous calling function or extended class may have code to handle the exception. 
     * In this case we don't need to write dupticate code again so we thows that exception to calling function. 
     * if that function has handler it will handled else throw exception
     * Best way is to use try/catch.
     * It can be used when programmer 100% sure that line will not throw error in this case it can be used to supress the exception.
     * But java compiler throws error, On that time throws can be used 
     */
    public static void main(String[] args)  throws IOException
    {
        BufferedReader bf = new BufferedReader(new InputStreamReader(System.in));
        BufferedReader bf1 = new BufferedReader(new InputStreamReader(System.in));
        String s = bf.readLine();// if exception is throwed there is no handler it will break program
        /**
         * The above line will raise error if the throws IOException is not there
         * It will supress IOException error
         */
        try {
            String s1 = bf1.readLine();
        } catch (IOException e) {
            System.out.println(e);
        }
        finally{
            bf.close();
            bf1.close();
        }
        
        DemoException_3 d1 = new DemoException_3();

        // Ducking Exception
        d1.outerFunc(); //Op:Exception in Inner function
    }

    int innerFunc(int i) {
        i = i + 20;
        try {
            i = innerInFunction(i);
        } catch (Exception e) { // if the exception is not handled here it will check for called function 
          System.out.println("Exception in Inner function");
        }
        return i;
    }

    int innerInFunction(int i) throws ArithmeticException {
        i = i/0; // the exception is handled in innerFunc
        return  i;
    }

    void outerFunc() {
        try {
            innerFunc(1);
        } catch (ArithmeticException e) {
            System.out.println("Exception in Outer function");
        }
    }

}
