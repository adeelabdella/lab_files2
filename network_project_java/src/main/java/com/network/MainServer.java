package com.network;

import com.google.gson.Gson;
import java.io.*;
import java.net.*;
import java.util.*;

public class MainServer {
    private static final int PORT = 4000;
    private static final String DISCONNECT_MESSAGE = "!DISCONNECTED!";
    private static final String FIRST_CONNECTION = "!FIRST_CONNECTION!";
    private static final String SERVER = "localhost";
    private static final int MAX_SIZE = Integer.MAX_VALUE;

    // ANSI escape codes for text colors
    private static final String RESET = "\u001B[0m";
    private static final String RED = "\u001B[31m";
    private static final String GREEN = "\u001B[32m";
    private static final String YELLOW = "\u001B[33m";
    private static final String BLUE = "\u001B[34m";

    private final HashMap<InetSocketAddress, HashMap<String, String>> userList = new HashMap<>();

    public static void main(String[] args) {
        MainServer server = new MainServer();
        server.run();
    }

    public List<Number> is_prime(Long number) {
        if (number < 2)
            return new ArrayList<>();
        ArrayList<Number> list = new ArrayList<>();
        for (int i = 2; i < Math.sqrt(number) + 1; i++) {
            if (number % i == 0) {
                list.add(i);
            }
        }
        return list;
    }

    public void run() {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            while (true) {
                try {
                    Socket clientConnection = serverSocket.accept();
                    Thread thread = new Thread(
                            () -> handleClient(clientConnection, new InetSocketAddress(SERVER, PORT)));
                    thread.start();
                    coloredPrint("[ACTIVE CONNECTIONS] " + (Thread.activeCount() - 2 + 1) + "\n", YELLOW);
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

    private void sendMessage(String msg, Socket clientConnection, InetSocketAddress clientAddress) {
        try {
            PrintWriter out = new PrintWriter(clientConnection.getOutputStream(), true);
            out.println(msg);
        } catch (IOException e) {
            coloredPrint("[UNABLE TO SEND MESSAGE TO " + userList.get(clientAddress).get("name") + "]: "
                    + e.getMessage() + "...\n", RED);
            userList.remove(clientAddress);
            System.exit(0);
        }
    }

    private String decodeMessage(String strReceived, Socket clientConnection, InetSocketAddress clientAddress) {
        HashMap<String, String> clientObject;
        clientObject = new Gson().fromJson(strReceived, HashMap.class);
        
        if (clientObject.get("msg").equals(FIRST_CONNECTION)) {
            userList.put(clientAddress, new HashMap<>());
            userList.get(clientAddress).put("name", clientObject.get("name"));
            return "Joined the server.";
        } else {
            if (clientObject.get("msg").equals(DISCONNECT_MESSAGE)) {
                return clientObject.get("msg");
            }
            try {
                long number = Long.parseLong(clientObject.get("msg"));
                if (number > MAX_SIZE - 1) {
                    sendMessage("Enter a number smaller than " + MAX_SIZE, clientConnection, clientAddress);
                } else if (number < 0) {
                    sendMessage(number + "is not a Natural Number.", clientConnection, clientAddress);
                } else {
                    List<Number> list;
                    list = is_prime(number);
                    if (list.isEmpty()) {
                        sendMessage("You have entered a PRIME number", clientConnection, clientAddress);
                    } else {
                        StringBuilder factor = getFactor(list);
                        sendMessage("You have entered a COMPOSITE number. Factors = [" + factor + "]", clientConnection,
                                clientAddress);
                    }
                }
                return clientObject.get("msg");
            } catch (NumberFormatException e) {
                sendMessage("You have entered an INVALID number", clientConnection, clientAddress);
                return clientObject.get("msg");
            }
        }
    }

    private static StringBuilder getFactor(List<Number> list) {
        StringBuilder factor = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            if (i > 3)
                break;
            factor.append(list.get(i));
            if (i < list.size() - 1) {
                factor.append(", ");
            }
        }
        if (list.size() > 4) {
            factor.append("...");
        }
        return factor;
    }

    private void handleClient(Socket clientConnection, InetSocketAddress clientAddress) {
        coloredPrint("[NEW CONNECTION] " + clientAddress + " connected.\n", GREEN);
        boolean connected = true;
        while (connected) {
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(clientConnection.getInputStream()));
                String strReceived = in.readLine();
                if (strReceived == null || strReceived.isEmpty()) {
                    continue;
                }
                String msg = decodeMessage(strReceived, clientConnection, clientAddress);
                if (msg.equals(DISCONNECT_MESSAGE)) {
                    connected = false;
                    coloredPrint(userList.get(clientAddress).get("name") + " is offline now.", RED);
                    coloredPrint("[ACTIVE CONNECTIONS] " + (Thread.activeCount() - 3 + 1) + "\n", YELLOW);
                } else {
                    coloredPrint(userList.get(clientAddress).get("name") + " : " + msg, BLUE);
                }
            } catch (IOException e) {
                coloredPrint("[UNABLE TO RECEIVE MESSAGE FROM " + userList.get(clientAddress).get("name") + "]: "
                        + e.getMessage() + "...\n", RED);
                userList.remove(clientAddress);
                System.exit(0);
            }
        }
        userList.remove(clientAddress);
        try {
            clientConnection.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
