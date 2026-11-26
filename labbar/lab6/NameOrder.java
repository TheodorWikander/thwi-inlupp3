import java.util.Scanner;

public class NameOrder {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter first name: ");
        String name1 = sc.next();  
        Scanner sc2 = new Scanner(System.in);
        System.out.print("Enter second name: ");
        String name2 = sc2.next();
        if (name1.compareToIgnoreCase(name2) < 0) {
            System.out.println(name1 + ", " + name2);
        } else if (name1.compareToIgnoreCase(name2) > 0){
            System.out.println(name2 + ", " + name1);
        } else {
            System.out.println("Names are equal! " + name1 + ", " + name2);
        }
    }
}
