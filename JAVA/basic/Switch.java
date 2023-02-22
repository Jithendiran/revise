/**
* Author: E.K.Jithendiran
* Date: 22.02.2023
*/
package basic;

public class Switch {
    public static void main(String[] args) {
        // int c = -1;
        char c = 97;
        switch (c) {
            case 1:
                System.out.println("1");
                break;
            // case -1: // only work when c is integer
            // System.out.println("-1");
            // break;
            case 'a': // if char c = 97 || 'a'
                System.out.println("a");
                break;
            case 65 | 66: // case will not work
                System.out.println("65 | 66");
                // case 1.2:
                // break;
                // case "hi":
                // break;
            default:
                System.out.println("Default");
                break;
        }
    }
}
