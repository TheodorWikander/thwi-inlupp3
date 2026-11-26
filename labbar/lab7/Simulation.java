package labbar.lab7;

import java.util.Random;

public class Simulation {
    private Store store;
    private int time;
    private int intensity;
    private int maxGroceries;
    private int threshholdForNewRegister;
    private Random random;

    private int totalCustomersServed;
    private int totalWaitTime;
    private int maxWaitTime;

    public Simulation(int numRegisters, int intensity, int maxGroceries, int threshholdForNewRegister) {
        this.store = new Store(numRegisters);
        this.time = 0;
        this.intensity = intensity;
        this.maxGroceries = maxGroceries;
        this.threshholdForNewRegister = threshholdForNewRegister;
        this.random = new Random();

        this.totalCustomersServed = 0;
        this.totalWaitTime = 0;
        this.maxWaitTime = 0;
    }
    
    public void step() {
        this.time++;
        this.store.step();

        if(this.random.nextInt(100) < this.intensity) {
            int groceries = this.random.nextInt(this.maxGroceries) + 1;
            Customer newCustomer = new Customer(this.time, groceries);
            this.store.newCustomer(newCustomer);
        }

        if(this.store.getAverageQueueLenght() > this.threshholdForNewRegister) {
            this.store.openNewRegister();
        }

        Queue doneCustomers =this.store.getDoneCustomers();
        while(doneCustomers.lenght() > 0) {
            Customer c = doneCustomers.dequeue();
            int waitTime = this.time - c.getBornTime();

            this.totalCustomersServed++;
            this.totalWaitTime += waitTime;
            if(waitTime > this.maxWaitTime) {
                this.maxWaitTime = waitTime;
            }
        }
    }

    public int getTotalCustomersServed() {
        return this.totalCustomersServed;
    }

    public int getMaxWaitTime() {
        return this.maxWaitTime;
    }

    public double getAverageWaitTime() {
        if(this.totalCustomersServed == 0) {
            return 0.0;
        }
        return (double) this.totalWaitTime / this.totalCustomersServed;
    }

    public String toString() {
        return this.store.toString()
            + "\nTime: " + this.time
            + "\nCustomers served: " + this.totalCustomersServed
            + "\nAvrage waiting time: " + String.format("%.2f", this.getAverageWaitTime())
            + "\nlongest waiting time: " + this.maxWaitTime;
    }

    public static void main(String[] args) throws InterruptedException {
        int numRegisters = 5;
        int intensity = 30;
        int maxGroceries = 10;
        int threshholdForNewRegister = 3;
        int steps = 200;

        Simulation sim = new Simulation(numRegisters, intensity, maxGroceries, threshholdForNewRegister);
        for (int i = 0; i < steps; i++) {
            sim.step();
            System.out.print("\033[2J\033[;H");
            System.out.println(sim);
            Thread.sleep(500);
        }
    }
}
