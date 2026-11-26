package labbar.lab7;

public class Register {
   private boolean open = false;
   private Queue queue = new Queue();

   public Register() {
   }

   public boolean open() {
      this.open = true;
      return this.open;
   }

   public boolean close() {
      this.open = false;
      return this.open;
   }

   public boolean isOpen() {
      return this.open;
   }

   public void step() {
    if(this.open && this.hasCustomers()) {
        this.queue.first().serve();
    }
   }

   public boolean hasCustomers() {
      return this.queue.lenght() > 0;
   }

   public boolean currentCustomerIsDone() {
    if (!this.hasCustomers()) {
        return false;
    }
    Customer var1 = this.queue.first();
    return var1.isDone();
   }

   public boolean addToQueue(Customer var1) {
      return this.queue.enqueue(var1);
   }

   public Customer removeCurrentCustomer() {
      return this.queue.dequeue();
   }

   public int getQueueLenght() {
      return this.queue.lenght();
   }

   public String toString() {
      if (!this.open) {
         return "X [ ]";
      }
      if (!this.hasCustomers()) {
         return "[ ]";
      }
      String s = this.queue.first().toString();
      for (int i = 1; i < this.queue.lenght(); i++) {
         s += "@";
      }
      return s;
   }

   public static void main(String[] var0) {
      Register var1 = new Register();
      Customer var2 = new Customer(1, 2);
      Customer var3 = new Customer(2, 1);
      Customer var4 = new Customer(3, 1);
      var1.addToQueue(var2);
      var1.addToQueue(var3);
      var1.addToQueue(var4);
      if (var1.open()) {
         if (var1.getQueueLenght() == 3) {
            System.out.println("Queue lenght is " + var1.getQueueLenght());
         }

         var1.step();
         var1.step();
         System.out.println("Queue lenght is " + var1.getQueueLenght());
         var1.step();
         var1.step();
         System.out.println("Queue lenght is " + var1.getQueueLenght());
         if (!var1.hasCustomers()) {
            System.out.println("Register has no customers");
         }

         if (var1.isOpen()) {
            System.out.println("Register is open");
         }

         var1.close();
         if (!var1.isOpen()) {
            System.out.println("Register is closed");
         }
      }

   }
}
