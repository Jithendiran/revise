/**
 * Author: E.K.Jithendiran
 * Date: 06.03.2023
 */
package multiThreading;

class Storage {
    private int value = 0;
    private boolean isConsuming = false;

    /**
     * This function is responsible for producing value
     * @param value
     * @throws InterruptedException
     */
    public synchronized int put(int i) throws InterruptedException {
        while(isConsuming) {
            wait();
            // wait() method must be synchronized
        }
        value = i;
        isConsuming = true;
        notify(); // notifying consumer
        return value;
    }

    /**
     * This function is responsible to consume value
     * @return
     * @throws InterruptedException
     */
    public synchronized int get() throws InterruptedException {
        while(!isConsuming) {
            wait();
            // wait() method must be synchronized
        }
        isConsuming = false;
        notify(); // notifying producer
        return value;
    }
}

class Producer extends Thread {
    Storage s;
    int limit = 10;
    Producer(Storage s) {
        this.s = s;
        this.start();
    }

    public void run() {
        int i = 0;
        while(i <= limit) {
            try {
                System.out.println("produced : "+s.put(i++));
                Thread.sleep(800);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

class Consumer extends Thread {
    Storage s;
    int limit = 10;
    Consumer(Storage s) {
        this.s = s;
        this.start();
    }

    public void run() {
       while(limit >= 0) {
           try {
            System.out.println("consumed : "+s.get());
            limit--;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
       }
    }
}
public class multiThread_4 {
    public static void main(String[] args) {
        Storage s = new Storage();
        new Producer(s);
        new Consumer(s);
        /**
         * Op
            Produced : 0
            Consumed : 0
            Produced : 1
            Consumed : 1
            Produced : 2
            Consumed : 2
            Produced : 3
            Consumed : 3
            Produced : 4
            Consumed : 4
            Produced : 5
            Consumed : 5
            Produced : 6
            Consumed : 6
            Produced : 7
            Consumed : 7
            Produced : 8
            Consumed : 8
            Produced : 9
            Consumed : 9
            Produced : 10
            Consumed : 10
         */
    }
}
