public class bin {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java bin.java <encode|decode> \"text\"");
            return;
        }
        
        String command = args[0];
        String input = args[1];
        
        if (command.equalsIgnoreCase("encode")) {
            System.out.println(encodeToBinary(input));
        } else if (command.equalsIgnoreCase("decode")) {
            System.out.println(decodeFromBinary(input));
        } else {
            System.out.println("Invalid command. Use 'encode' or 'decode'.");
        }
    }
    
    public static String encodeToBinary(String text) {
        StringBuilder binary = new StringBuilder();
        for (char c : text.toCharArray()) {
            binary.append(String.format("%8s", Integer.toBinaryString(c)).replace(' ', '0')).append(" ");
        }
        return binary.toString().trim();
    }
    
    public static String decodeFromBinary(String binaryText) {
        StringBuilder text = new StringBuilder();
        String[] binaryValues = binaryText.split(" ");
        for (String binary : binaryValues) {
            text.append((char) Integer.parseInt(binary, 2));
        }
        return text.toString();
    }
}
