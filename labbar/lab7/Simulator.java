package labbar.lab7;

public class Simulator{

    public static void main(String[] args) throws InterruptedException{
        int steps = 100;

        int numRegisters = 5;   
        int intensity = 30;     
        int maxGroceries = 10; 
        int threshold = 3;      

        Simulation s = new Simulation(numRegisters, intensity, maxGroceries, threshold); 
        for(int i = 0; i < steps; i++){
            System.out.print("\033[H\033[2J\033[3J");
            System.out.flush();
            s.step();
            System.out.println(s);
            Thread.sleep(500);
        }
        System.out.println("");
        System.out.println("Simulation finished!");
    }
}