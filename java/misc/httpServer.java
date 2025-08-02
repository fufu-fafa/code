import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.net.InetSocketAddress;

public class httpServer {
    public static void main(String[] args) throws IOException {
        int port = 8080;
        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);
        server.createContext("/", new StaticFileHandler());
        server.setExecutor(null);
        server.start();
        System.out.println("Server started on http://localhost:" + port);
    }
}

class StaticFileHandler implements HttpHandler {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
        System.out.println(exchange.getRequestMethod() + " " + exchange.getRequestURI() + " from " + exchange.getRemoteAddress());
        String uriPath = exchange.getRequestURI().getPath();
        if (uriPath.equals("/")) {
            uriPath = "/index.html"; 
        }

        if (!uriPath.contains(".")) {
            uriPath += ".html";
        }

        String filePath = "." + uriPath; // serve files from current directory
        Path path = Paths.get(filePath);

        if (Files.exists(path) && !Files.isDirectory(path)) {
            byte[] fileBytes = Files.readAllBytes(path);

            String contentType = "text/html";
            if (filePath.endsWith(".css")) contentType = "text/css";
            else if (filePath.endsWith(".js")) contentType = "application/javascript";
            else if (filePath.endsWith(".png")) contentType = "image/png";
            else if (filePath.endsWith(".svg")) contentType = "image/svg+xml";
            else if (filePath.endsWith(".jpg") || filePath.endsWith(".jpeg")) contentType = "image/jpeg";

            exchange.getResponseHeaders().set("Content-Type", contentType);
            exchange.sendResponseHeaders(200, fileBytes.length);
            OutputStream os = exchange.getResponseBody();
            os.write(fileBytes);
            os.close();
        } else {
            // Return 404 Not Found
            String response = "404 (Not Found)\n";
            exchange.sendResponseHeaders(404, response.length());
            OutputStream os = exchange.getResponseBody();
            os.write(response.getBytes());
            os.close();
        }
    }
}

