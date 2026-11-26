import java.util.Scanner;

public class PairOfDice {
    private Die firstDie;
    private Die secondDie;

    public PairOfDice(int numberOfSides) {
        this.firstDie = new Die(numberOfSides);
        this.secondDie = new Die(numberOfSides);
    }

    public int rollPair() {
        int first = this.firstDie.roll();
        int second = this.secondDie.roll();
        return first + second;
    }

    public int getFirst() {
        return this.firstDie.get();
    }

    public int getSecond() {
        return this.secondDie.get();
    }

    public String toString() {
        return "first die is (" + this.firstDie + ") and second die is (" + this.secondDie + ")";
    }

    public static void main(String [] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Number of sides: ");
        int sides = sc.nextInt();
        PairOfDice p = new PairOfDice(sides);
        System.out.println("Before rolling: " + p);
        int sum = p.rollPair();
        System.out.println("Sum: " + sum);
        System.out.println("First die is: (" + p.getFirst() + ")" + " second die is: (" + p.getSecond() + ")");
    }
}

