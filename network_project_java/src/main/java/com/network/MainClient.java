package com.network;

import com.google.gson.Gson;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.HashMap;

public class MainClient {
    private static final String RESET = "\u001B[0m";
    private static final String RED = "\u001B[31m";
    private static final String GREEN = "\u001B[32m";
    private static final String BLUE = "\u001B[34m";
    private static final String YELLOW = "\u001B[33m";
    private static final String DISCONNECT_MESSAGE = "!DISCONNECTED!";
    private static final String FIRST_CONNECTION = "!FIRST_CONNECTION!";
    private static final String GAME_START = "start_the_game_please";
    private static final int PORT = 4000;
    private static final String SERVER = "localhost"; // Use the actual server address
    private final HashMap<String, String> hashMap = new HashMap<>();
    private final Socket clientSocket;
    private final PrintWriter out;
    private final BufferedReader in;
    private String NAME;

    public MainClient() throws IOException {
        hashMap.put("p", "PRIME");
        hashMap.put("P", "PRIME");
        hashMap.put("c", "COMPOSITE");
        hashMap.put("C", "COMPOSITE");
        this.clientSocket = new Socket(SERVER, PORT);
        this.out = new PrintWriter(clientSocket.getOutputStream(), true);
        this.in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
    }

    public static void main(String[] args) {
        try {
            MainClient client = new MainClient();
            client.sendAndReceiveMessages();
        } catch (IOException e) {
            System.out.println("Unable to connect to the server: " + e.getMessage());
        }
    }

    private static String input(String message) {
        System.out.print(message);
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        try {
            return reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }

    public void coloredPrint(String text, String color) {
        System.out.println(color + text + RESET);
    }

    public void sendAndReceiveMessages() throws IOException {
        boolean connected = true, showStartMessage = true;
        this.NAME = input(BLUE + "Enter your name or alias.");
        String msgToSend = new Gson().toJson(new Message(FIRST_CONNECTION, NAME));
        out.println(msgToSend);
        while (connected) {
            String msg = "";
            coloredPrint("_______[GAME_SERVER]________\n", GREEN);
            msg = input("Start a new game [y/n]: ");
            if (msg.equalsIgnoreCase("n")) {
                msg = DISCONNECT_MESSAGE;
                sendMessage(msg);
                connected = false;
                continue;
            } else if (msg.equalsIgnoreCase("y")) {
                sendMessage(GAME_START);
                String serverMsg = receiveMessage();
                System.out.println(serverMsg);
                showStartMessage = false;
            } else {
                coloredPrint("Enter a VALID option [y/Y] or [n/N]", YELLOW);
                continue;
            }
            while (!showStartMessage) {
                msg = input("Prime or Composite [p/c] : ");
                if (!(msg.equalsIgnoreCase("p") || msg.equalsIgnoreCase("c"))) {
                    coloredPrint("Invalid Option", YELLOW);
                    continue;
                } else {
                    sendMessage(hashMap.get(msg));
                    receiveMessage();
                    showStartMessage = true;
                }
                if (input(BLUE + "Press EXIT to exit the game or ENTER to continue." + RESET)
                        .equalsIgnoreCase("exit")) {
                    msg = DISCONNECT_MESSAGE;
                    sendMessage(msg);
                    connected = false;
                }
            }

        }
        coloredPrint("Connection Closed!", RED);
        closeClientConnection();
        System.exit(0);
    }

    public void sendMessage(String inputMessage) {
        String msgToSend = new Gson().toJson(new Message(inputMessage, NAME));
        out.println(msgToSend);
    }

    public String receiveMessage() {
        try {
            String serverMessage = in.readLine();
            coloredPrint("Server : " + serverMessage, GREEN);
            return serverMessage;
        } catch (IOException e) {
            coloredPrint("[UNABLE TO RECEIVE MESSAGE FROM THE SERVER]: " + e.getMessage(), RED);
            return null;
        }
    }

    public void closeClientConnection() {
        try {
            in.close();
            out.close();
            clientSocket.close();
        } catch (IOException e) {
            coloredPrint("Error while closing the connection: " + e.getMessage(), RED);
        }
    }

    private static class Message {
        private final String msg;
        private final String name;

        public Message(String msg, String name) {
            this.msg = msg;
            this.name = name;
        }
    }
}
