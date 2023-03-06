/**
 * Author: E.K.Jithendiran
 * Date: 06.03.2023
 */
package multiThreading;

class Counter {
    int c;
    /**
     * When without synchronized. If multiple thread is accessing the value means it may out dated
     * eg consider 2 threads t1 and t2
     * t1 is accessig the value as 10 and increment to 11
     * t2 also accessing the value parallely as 10 and increment to 11
     * 
     * so in this case c is 2 time incremeneted to 11 only
     */
    void countWithOutSync(){
        c++;
    }
    /**
     * When synchronized. Multiple thread can't access the function block.
     * If one is accessing other has to wait
     * This method is thread safe
     */
    synchronized void countWithSync(){
        c++;
    }
}

class Multi extends Thread {
    boolean isSync;
    Counter c;
    Multi(Counter c) {
        this.c = c;
    }
    public void run(){
        for (int i = 0; i < 1000; i++) {
            if (isSync) {
                c.countWithSync();
            } else {
                c.countWithOutSync();
            }
        }
    }
}

public class MultiThread_3 {
    public static void main(String[] args) throws Exception{
        Counter c = new Counter();
        c.c = 0;
        Multi m1 = new Multi(c);
        Multi m2 = new Multi(c);
        m1.isSync = false;
        m2.isSync = false;

        m1.start();
        m2.start();
        m1.join();
        m2.join();

        System.out.println("C : "+c.c); // C : 1651

        c.c = 0;
        m1 =  new Multi(c);
        m2 =  new Multi(c);

        m1.isSync = true;
        m2.isSync = true;

        m1.start();
        m2.start();
        m1.join();
        m2.join();

        // System.out.println("C : "+c.c);// C : 2000
    }
    
}
