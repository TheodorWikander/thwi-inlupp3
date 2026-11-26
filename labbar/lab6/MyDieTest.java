import java.util.Scanner;

public class MyDieTest {
    public static void main(String [] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Number of sides: ");
        int sides = sc.nextInt();
        Die d = new Die(sides);
        int tot = 0;
        for (int i = 0; i < 10; ++i) {
            int s = d.roll();
            tot = s + tot;
        }
        System.out.println("Du fick: " + tot);
    }
}
