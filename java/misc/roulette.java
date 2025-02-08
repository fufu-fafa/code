import java.util.Random;
import java.util.Scanner;

public class roulette {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("usage: java testing < -n for numbers | -c for colors >");
            return;
        }

        Scanner scanner = new Scanner(System.in);
        Random random = new Random();

        try {
            int input;
            if (args[0].equals("-n")) {
                System.out.print("enter a number between 0 and 36, or '00': ");
                String input_str = scanner.nextLine().trim();

                if (input_str.equals("00")) {
                    input = 37;
                } else if (input_str.equals("37")) {
                    System.out.println("no");
                    return;
                } else {
                    try {
                        input = Integer.parseInt(input_str);
                    } catch (NumberFormatException err) {
                        System.out.println("no");
                        return;
                    }
                }

                if (input < 0 || input > 37) {
                    System.out.println("no no");
                    return;
                }

                int generated = random.nextInt(38);
                String generated_str;

                if (generated == 37) {
                    generated_str = "00";
                } else {
                    generated_str = String.valueOf(generated);
                }

                System.out.println(generated_str);
                System.out.println(input == generated ? "(ULTRA DING DING DING)²" : "unlucky");

            } else if (args[0].equals("-c")) {
                System.out.print("enter a color r for red, b for black, or g for green: ");
                String other_input = scanner.nextLine().toLowerCase().trim();

                if (!other_input.equals("r") && !other_input.equals("b") && !other_input.equals("g")) {
                    System.out.println("invalid color choice");
                    return;
                }

                int roll = random.nextInt(38);

                String generated_col;
                if (roll == 37 || roll == 0) {
                    generated_col = "g";
                } else if (isred(roll)) {
                    generated_col = "r";
                } else {
                    generated_col = "b";
                }

                if (other_input.equals("g") && generated_col.equals("g")) {
                    System.out.println("ULTRA DING DING DING");
                } else {
                    System.out.println(other_input.equals(generated_col) ? "DING DING DING" : "unlucky");
                }

                System.out.println(generated_col + (roll == 37 ? "00" : roll));

            } else {
                System.out.println("no");
            } 
        } finally {
            scanner.close();
        }
    }
    private static boolean isred(int num) {
        int[] rednum = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
        for (int red : rednum) {
            if (num == red) return true;
        }
        return false;
    }    
}
