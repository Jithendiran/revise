/**
 * Author: E.K.Jithendiran
 * Date: 06.03.2023
 */
package multiThreading;

class C1 extends Thread{
    // when object of this class call's start internally run will called
    // multithreding program must be inside run method and extends Threads
    public void run()  {
        for (int i = 0; i < 5; i++) {
            System.out.println("C1");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}


class C2 implements Runnable{
    // Runnable is the Interface for multithreading
    public void run() {
        for (int i = 0; i < 5; i++) {
            System.out.println("C2");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}


public class MultiThread_1{
    // By default all the program in java is single thread known as main thread
    public static void main(String[] args) throws Exception {
        C1 c1 = new C1();
        C2 c2 = new C2();
        System.out.println("Started Run");
        c1.run();
        c2.run();
        // When calling run itself it will not multithread
        /**
         * Op
            C1
            C1
            C1
            C1
            C1
            C2
            C2
            C2
            C2
            C2
         */
        System.out.println("Started start");
         c1.start();
        //  //c2.start(); implements does not have start
         Thread tc2 = new Thread(c2);
         tc2.start();
         /**
          * Op
                C1
                C2
                C1
                C2
                C1
                C2
                C1
                C2
                C1
                C2
          */
          c1.join();
          tc2.join();
          System.out.println("Started annonyms class");
          // Runnable is an functional interface
          Runnable r1 = new Runnable() {
            public void run(){
                for (int i = 0; i < 5; i++) {
                    System.out.println("R1");
                    try {
                        Thread.sleep(500);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
          };

          Runnable r2 = () -> {
                for (int i = 0; i < 5; i++) {
                    System.out.println("R2");
                    try {
                        Thread.sleep(500);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
          };

          Thread t1 = new Thread(r1);
          tc2 = new Thread(r2);
          t1.start();
          tc2.start();
          /**
           * Op
                R1
                R2
                R1
                R2
                R1
                R2
                R1
                R2
                R1
                R2
           */
          System.out.println("T1 is alive : "+t1.isAlive()); // Op: T1 is alive : true
          c1.join();
          t1.join();
          tc2.join();
          System.out.println("T1 is alive : "+t1.isAlive()); //Op: T1 is alive : false

          /**
           * Join is used to wait main thread untill respective sub thread finish their execution
           */
           System.out.println("Last");
    }
}