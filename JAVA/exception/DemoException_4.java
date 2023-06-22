/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package exception;

class CustomException extends Exception {
    CustomException(String s){
        super(s);
    }
}

class CustomRunTimeException extends RuntimeException {
    CustomRunTimeException(String s){
        super(s);
    }
}

class CustomAirthmeticException extends ArithmeticException {
    CustomAirthmeticException(String s){
        super(s);
    }
}

public class DemoException_4{
    public static void main(String[] args) {
        int i;
        i = 0;
        if(i == 0) {
            throw new CustomAirthmeticException("Custom exception"); //Eror:exception.CustomAirthmeticException: Custom exception
        }
    }
}