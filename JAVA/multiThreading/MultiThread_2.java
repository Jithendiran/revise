/**
 * Author: E.K.Jithendiran
 * Date: 06.03.2023
 */
package multiThreading;

public class MultiThread_2 {
    public static void main(String[] args) {
        // Name, Priority
          Thread t1 = new Thread(new Runnable() {
            public void run(){
                for (int i = 0; i < 5; i++) {
                    System.out.println("R1 : "+Thread.currentThread().getName());
                }
            }
          });
          Thread t2 = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                System.out.println("R2");
            }
      },"T2");
      t1.setName("T1");
      System.out.println("T1 Name : "+t1.getName()+" Id : "+t1.getId()+" Priority : "+t1.getPriority()); // Op: T1 Name : T1 Id : 13 Priority : 5
      System.out.println("T2 Name : "+t2.getName()+" Id : "+t2.getId()+" Priority : "+t2.getPriority()); // Op: T2 Name : T2 Id : 14 Priority : 5

      // Range of thread priority 1 - 10. 1 is low and 10 is high   
      t1.setPriority(1); //Thread.MIN_PRIORITY; -> 1
      t2.setPriority(8);
      System.out.println("T1 Name : "+t1.getName()+" Id : "+t1.getId()+" Priority : "+t1.getPriority()); // Op: T1 Name : T1 Id : 13 Priority : 1
      System.out.println("T2 Name : "+t2.getName()+" Id : "+t2.getId()+" Priority : "+t2.getPriority()); // Op: T2 Name : T2 Id : 14 Priority : 8
      t1.start();
      t2.start();
      /***
       * Op // order will not guarenty
        R1 : T1
        R1 : T1
        R1 : T1
        R1 : T1
        R1 : T1
        R2
        R2
        R2
        R2
        R2
       */
    }
}
