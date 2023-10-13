package com.network;

import com.google.gson.Gson;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainServer {
    private static final int PORT = 4000;
    private static final String DISCONNECT_MESSAGE = "!DISCONNECTED!";
    private static final String FIRST_CONNECTION = "!FIRST_CONNECTION!";
    private static final String SERVER = "localhost";
    private static final String GAME_START = "start_the_game_please";

    // ANSI escape codes for text colors
    private static final String RESET = "\u001B[0m";
    private static final String RED = "\u001B[31m";
    private static final String GREEN = "\u001B[32m";
    private static final String YELLOW = "\u001B[33m";
    private static final String BLUE = "\u001B[34m";

    private final Map<String, HashMap<Object, Object>> userList = new HashMap<String, HashMap<Object, Object>>();

    public static void main(String[] args) {
        MainServer server = new MainServer();
        server.start();
    }

    public void start() {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            while (true) {
                try {
                    Socket clientConnection = serverSocket.accept();
                    Thread thread = new Thread(
                            () -> handleClient(clientConnection, new InetSocketAddress(SERVER, PORT)));
                    thread.start();
                    coloredPrint("[NEW CONNECTION] " + clientConnection.getInetAddress() + " connected.\n", GREEN);
                    coloredPrint("[ACTIVE CONNECTIONS] " + (Thread.activeCount() - 2) + "\n", YELLOW);
                } catch (IOException e) {
                    coloredPrint("[UNABLE TO CONNECT TO THE CLIENTS]: " + e.getMessage() + "...\n", RED);
                    System.exit(0);
                }
            }
        } catch (IOException e) {
            coloredPrint("[UNABLE TO CREATE SOCKET]: " + e.getMessage() + "...\n", RED);
            System.exit(0);
        }
    }

    private void coloredPrint(String text, String color) {
        System.out.println(color + text + RESET);
    }

    private void sendMessage(String msg, Socket clientConnection, InetSocketAddress clientAddress, String clientName) {
        try {
            System.out.println(msg);
            PrintWriter out = new PrintWriter(clientConnection.getOutputStream(), true);
            out.println(msg);
        } catch (IOException e) {
            coloredPrint("[UNABLE TO SEND MESSAGE TO " + userList.get(clientAddress + clientName).get("name") + "]: "
                    + e.getMessage() + "...\n", RED);
            userList.remove(clientAddress + clientName);
            System.exit(0);
        }
    }

    private String is_prime(long number) {
        if (number < 2)
            return "COMPOSITE";
        for (int i = 2; i < Math.sqrt(number) + 1; i++) {
            if (number % i == 0)
                return "COMPOSITE";
        }
        return "PRIME";
    }

    private List<String> decodeMessage(String strReceived, Socket clientConnection, InetSocketAddress clientAddress) {
        Map<String, String> clientObject = new Gson().fromJson(strReceived, HashMap.class);
        String clientName = clientObject.get("name");

        List<String> list = new ArrayList<>();
        list.add(clientName);
        if (clientObject.get("msg").equals(FIRST_CONNECTION)) {
            userList.put(clientAddress + clientName, new HashMap<Object, Object>());
            userList.get(clientAddress + clientName).put("name", clientObject.get("name"));
            list.add("Joined the server.");
            return list;
        } else {
            if (clientObject.get("msg").equals(DISCONNECT_MESSAGE)) {
                coloredPrint(userList.get(clientAddress + clientName).get("name") + " is offline now.", RED);
                coloredPrint("[ACTIVE CONNECTIONS] " + (Thread.activeCount() - 3) + "\n", YELLOW);
                list.add(clientObject.get("msg"));
            } else {
                String clientMsg = clientObject.get("msg");
                if (clientMsg.equals(GAME_START)) {
                    long numberForClient = (long) Math.ceil(Math.random() * 100);
                    userList.get(clientAddress + clientName).put("number", is_prime(numberForClient));
                    String guessMessage = "Guess for this Number: " + numberForClient;
                    sendMessage(guessMessage, clientConnection, clientAddress, clientName);
                } else if (clientMsg.equals(userList.get(clientAddress + clientName).get("number"))) {
                    sendMessage(GREEN + "Correct!!!" + RESET, clientConnection, clientAddress, clientName);
                } else {
                    sendMessage(RED + "Wrong answer" + RESET, clientConnection, clientAddress, clientName);
                }
                list.add(clientMsg);
            }
            return list;
        }
    }

    private void handleClient(Socket clientConnection, InetSocketAddress clientAddress) {
        coloredPrint("[NEW CONNECTION] " + clientAddress + " connected.\n", GREEN);
        boolean connected = true;
        String clientName = "";
        while (connected) {
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(clientConnection.getInputStream()));
                String strReceived = in.readLine();
                if (strReceived == null || strReceived.isEmpty()) {
                    continue;
                }
                List<String> list = decodeMessage(strReceived, clientConnection, clientAddress);
                clientName = list.get(0);
                if (list.get(1).equals(DISCONNECT_MESSAGE)) {
                    connected = false;
                } else {
                    coloredPrint(userList.get(clientAddress + list.get(0)).get("name") + " : " + list.get(1), BLUE);
                }
            } catch (IOException e) {
                coloredPrint("[UNABLE TO RECEIVE MESSAGE FROM " + clientName + e.getMessage() + "...\n", RED);
                userList.remove(clientAddress + clientName);
                System.exit(0);
            }
        }
        userList.remove(clientAddress + clientName);
        try {
            clientConnection.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
