public class bin {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java BinConverter <encrypt|decrypt> \"text\"");
            return;
        }
        
        String command = args[0];
        String input = args[1];
        
        if (command.equalsIgnoreCase("encrypt")) {
            System.out.println(encryptToBinary(input));
        } else if (command.equalsIgnoreCase("decrypt")) {
            System.out.println(decryptFromBinary(input));
        } else {
            System.out.println("Invalid command. Use 'encrypt' or 'decrypt'.");
        }
    }
    
    public static String encryptToBinary(String text) {
        StringBuilder binary = new StringBuilder();
        for (char c : text.toCharArray()) {
            binary.append(String.format("%8s", Integer.toBinaryString(c)).replace(' ', '0')).append(" ");
        }
        return binary.toString().trim();
    }
    
    public static String decryptFromBinary(String binaryText) {
        StringBuilder text = new StringBuilder();
        String[] binaryValues = binaryText.split(" ");
        for (String binary : binaryValues) {
            text.append((char) Integer.parseInt(binary, 2));
        }
        return text.toString();
    }
}
