import java.util.ArrayList;
import java.util.Scanner;

public class taskList {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        ArrayList<String> tasks = new ArrayList<>();
        int choice;

        do {
            System.out.println("\nTo-Do List Menu:");
            System.out.println("1. Add Task");
            System.out.println("2. Remove Task");
            System.out.println("3. View Tasks");
            System.out.println("4. Exit");
            System.out.print("Enter your choice: ");
            choice = scanner.nextInt();
            scanner.nextLine(); // Consume newline

            switch (choice) {
                case 1: // Add Task
                    System.out.print("Enter task: ");
                    String task = scanner.nextLine();
                    tasks.add(task);
                    System.out.println("Task added!");
                    break;

                case 2: // Remove Task
                    System.out.println("Enter task number to remove:");
                    int index = scanner.nextInt();
                    if (index >= 1 && index <= tasks.size()) {
                        tasks.remove(index - 1);
                        System.out.println("Task removed!");
                    } else {
                        System.out.println("Invalid task number!");
                    }
                    break;

                case 3: // View Tasks
                    System.out.println("\nYour Tasks:");
                    if (tasks.isEmpty()) {
                        System.out.println("No tasks yet.");
                    } else {
                        for (int i = 0; i < tasks.size(); i++) {
                            System.out.println((i + 1) + ". " + tasks.get(i));
                        }
                    }
                    break;

                case 4: // Exit
                    System.out.println("");
                    break;

                default:
                    System.out.println("Invalid choice. Try again.");
            }
        } while (choice != 4);

        scanner.close();
    }
}

