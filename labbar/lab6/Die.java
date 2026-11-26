import java.util.Scanner;

public class Die {
  private int numberOfSides;
  private int value;

  public Die() {
      this.numberOfSides = 6;
      this.value = roll();
  }

  public Die(int numberOfSides) {
      if (numberOfSides < 1) {
        throw new IllegalArgumentException( numberOfSides + " is an illegal number of sides for die!");
      } else {
        this.numberOfSides = numberOfSides;
        this.value = roll();
      }
  }

  public int roll() {
    this.value = (int) (Math.random() * numberOfSides) + 1;
    return this.get();
  }

  public int get() {
      return value;
  }

  public String toString() {
    return "" + value + "";
  }

  public boolean equals(Die otherDie) {
    if (otherDie == null) {
        return false;
    }
    return this.numberOfSides == otherDie.numberOfSides && this.value == otherDie.value;
  }

  public static void main(String [] args) {
      Scanner sc = new Scanner(System.in);
      System.out.print("Number of sides: ");
      int sides = sc.nextInt();
      Die d = new Die(sides);
      Die nd = new Die();
      if (d.equals(nd)) {
        System.out.println("Equal!");
      } else {
        System.out.println("Not equal!");
      }
  }
}

//      System.out.println("Alea iacta est: " + d.roll());

