package labbar.lab7;

import javax.swing.plaf.synth.SynthPasswordFieldUI;

public class Store {
    private Register registers[];
    private int no_reg;

    public Store(int no_reg) {
        this.registers = new Register[no_reg];

        for (int i = 0; i < this.registers.length; i++) {
            this.registers[i] = new Register();
        }

        if (no_reg > 0) {
            this.registers[0].open();
        }
    }

    public double getAverageQueueLenght() {
        int totalLenght = 0;
        int openRegisters = 0;

        for (Register r : this.registers) {
            if (r.isOpen()) {
                totalLenght += r.getQueueLenght();
                openRegisters++;
            }
        }

        if (openRegisters == 0) {
            return 0;
        } 

        double average = totalLenght / openRegisters;
        return average;
    }

    public void newCustomer(Customer c) {
        Register shortest = null;

        for (Register r : this.registers) {
            if(r.isOpen()) {
                if (shortest == null || r.getQueueLenght() < shortest.getQueueLenght()) {
                    shortest = r;
                }
            }
        }
        if (shortest != null) {
            shortest.addToQueue(c);
        }
    }

    public void step() {
        for (Register r: this.registers) {
            r.step();
        }
    }
    public boolean openNewRegister() {
        for (Register r: this.registers) {
            if (!r.isOpen()) {
                r.open();
                return true;
            }
        }
        return false;
    }

    public Queue getDoneCustomers() {
        Queue doneCustomers = new Queue();

        for (Register r : this.registers) {
            if(r.isOpen() && r.currentCustomerIsDone()) {
                doneCustomers.enqueue(r.removeCurrentCustomer());
            }
        }
        return doneCustomers;
    }

    public String toString() {
        String s = "";
        for (Register r : this.registers) {
            s += r.toString() + "\n";
        }
        return s;
    }

    public static void main(String[] args) {
        Store s = new Store(2);

        Customer c1 = new Customer(0, 2);
        Customer c2 = new Customer(0, 1);
        Customer c3 = new Customer(0, 3);

        System.out.println("----- START -----");
        s.newCustomer(c1);
        s.openNewRegister();
        s.newCustomer(c2);
        s.newCustomer(c3);

        System.out.println(s);

        for (int t = 0; t <= 5; t++) {
            System.out.println("----- Tidssteg " + t + " -----");

            s.step();
            Queue done = s.getDoneCustomers();

            while(done.lenght() > 0) {
                Customer finished = done.dequeue();
                System.out.println("-> En kund blev klar! (Född vid tid: " + finished + ")");
            }

            System.out.println(s);
            System.out.println("Snittkölängd: " + s.getAverageQueueLenght());
        }
    }
}
