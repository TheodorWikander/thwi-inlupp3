package labbar.lab7;
import java.util.Scanner;

public class Customer {
    private int bornTime;
    private int groceries;

    public Customer(int bornTime, int groceries) {
        this.bornTime = bornTime;
        this.groceries = groceries;
    }

    public void serve() {
        if (this.groceries < 0) throw new IllegalArgumentException(this.groceries + " is an illegal number of groceries!");
        this.groceries--;
    }

    public boolean isDone() {
        return this.groceries == 0;
    }

    public int getBornTime() {
        return this.bornTime;
    }

    public String toString() {
        return "[" + this.groceries + "]";
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of groceries: ");
        int groceries = sc.nextInt();
        int born = 1;
        Customer c = new Customer(born, groceries);
        while (!c.isDone()) {
            System.out.println(c.groceries);
            c.serve();
        }
        System.out.println("Done! Customer has " + c.groceries + " groceries left");
    }
}
