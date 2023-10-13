package com.network;

import java.io.*;
import java.net.*;
import java.util.*;
import com.google.gson.Gson;

public class MainClient {
    private static final String RESET = "\u001B[0m";
    private static final String RED = "\u001B[31m";
    private static final String GREEN = "\u001B[32m";
    private static final String YELLOW = "\u001B[33m";
    private static final String BLUE = "\u001B[34m";
    private static final String DISCONNECT_MESSAGE = "!DISCONNECTED!";
    private static final String FIRST_CONNECTION = "!FIRST_CONNECTION!";
    private static final int PORT = 4000;

    private static final String SERVER = "localhost"; // Use the actual server address

    private final Socket clientSocket;
    private final PrintWriter out;
    private final BufferedReader in;

    public MainClient() throws IOException {
        this.clientSocket = new Socket(SERVER, PORT);
        this.out = new PrintWriter(clientSocket.getOutputStream(), true);
        this.in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
    }

    public void coloredPrint(String text, String color) {
        System.out.println(color + text + RESET);
    }

    public void sendAndReceiveMessages() {
        boolean connected = true;
        coloredPrint("Enter your name or alias.", BLUE);
        try (Scanner scanner_x = new Scanner(System.in)) {
            String name = scanner_x.nextLine();
            Map<String, String> jsonMap = new HashMap<>();
            jsonMap.put("name", name);
            jsonMap.put("msg", FIRST_CONNECTION);
            String msgToSend = new Gson().toJson(jsonMap);
            out.println(msgToSend);
        }
        while (connected) {
            coloredPrint("Enter a number to check whether it's prime:", BLUE);
            try (Scanner scanner = new Scanner(System.in)) {
                String inputMessage = scanner.nextLine();
                sendMessage(inputMessage);
                receiveMessage();
                coloredPrint("Press Enter to continue or 'exit' to close...", YELLOW);
                String response = scanner.nextLine();
                if ("exit".equals(response)) {
                    connected = false;
                }
            }
        }
        closeClientConnection();
    }

    public void sendMessage(String inputMessage) {
        Map<String, String> jsonMap = new HashMap<>();
        jsonMap.put("msg", inputMessage);
        String msgToSend = new Gson().toJson(jsonMap);
        out.println(msgToSend);
    }

    public void receiveMessage() {
        try {
            String serverMessage = in.readLine();
            coloredPrint("Server: " + serverMessage, GREEN);
        } catch (IOException e) {
            coloredPrint("Unable to receive message from the server: " + e.getMessage(), RED);
            closeClientConnection();
        }
    }

    public void closeClientConnection() {
        coloredPrint("Connection Closed!", RED);
        sendMessage(DISCONNECT_MESSAGE);
        try {
            in.close();
            out.close();
            clientSocket.close();
        } catch (IOException e) {
            coloredPrint("Error while closing the connection: " + e.getMessage(), RED);
        }
        System.exit(0);
    }

    public static void main(String[] args) {
        try {
            MainClient client = new MainClient();
            client.sendAndReceiveMessages();
        } catch (IOException e) {
            System.out.println("Unable to connect to the server: " + e.getMessage());
        }
    }
}
