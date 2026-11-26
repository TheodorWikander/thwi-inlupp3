// Source code is decompiled from a .class file using FernFlower decompiler (from Intellij IDEA).
package labbar.lab7;

public class Queue {
   private Node first = null;
   private Node last = null;
   private int lenght = 0;

   public Queue() {
   }

   public int lenght() {
      return this.lenght;
   }

   public boolean enqueue(Customer var1) {
      Node var2 = new Node(var1);
      if (this.first == null) {
         this.first = var2;
         this.last = var2;
      } else {
         this.last.next = var2;
         this.last = var2;
      }

      ++this.lenght;
      return true;
   }

   public Customer dequeue() {
      if (this.lenght == 0) {
         throw new EmptyQueueException();
      } else {
         Customer var1 = this.first.element;
         this.first = this.first.next;
         --this.lenght;
         if (this.first == null) {
            this.last = null;
         }

         return var1;
      }
   }

   public Customer first() {
    if (this.lenght == 0) {
        return null;
    }
      return this.first.element;
   }

   public static void main(String[] var0) {
      Customer var1 = new Customer(1, 3);
      Customer var2 = new Customer(2, 2);
      Customer var3 = new Customer(3, 1);
      Queue var4 = new Queue();
      var4.enqueue(var1);
      var4.enqueue(var2);
      var4.enqueue(var3);
      if (var4.dequeue().equals(var1)) {
         System.out.println("Queue lenght is " + var4.lenght);
      }

      if (var4.dequeue().equals(var2)) {
         System.out.println("Queue lenght is " + var4.lenght);
      }

      if (var4.dequeue().equals(var3)) {
         System.out.println("Queue is empty, queue lenght is " + var4.lenght);
      }

   }

   private static class Node {
      Customer element;
      Node next;

      public Node(Customer var1) {
         this.element = var1;
         this.next = null;
      }
   }
}

