#include "pch.h"

#include <cstdlib>
#include <iostream>
#include <thread>

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// large enough for sdp in one shot
#define DEFAULT_BUFLEN 55120

#pragma comment(lib, "Ws2_32.lib")

#include "DirectTcpServer.h"

DirectTcpServer::DirectTcpServer(int port) { _port = port; }

DirectTcpServer::~DirectTcpServer() {}

int DirectTcpServer::Start() {
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    char recvbuf[DEFAULT_BUFLEN] = {0};
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, std::to_string(_port).c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket =
        socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    _clientSocket = accept(ListenSocket, NULL, NULL);
    if (_clientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {
        iResult = recv(_clientSocket, (char *)recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Hank, call the callback to set the sdp
            auto message = std::string((char *)recvbuf);

            _messageHandler->OnMessage(message);

            memset(recvbuf, 0, DEFAULT_BUFLEN);

            /*
            iSendResult = send(ClientSocket, (char *) recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR)
            {
              printf("send failed with error: %d\n", WSAGetLastError());
              closesocket(ClientSocket);
              WSACleanup();
              return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
            */
        } else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(_clientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(_clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(_clientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(_clientSocket);
    
    
    //Hank:  do not do this, it might cause exception of libwebrtc, do this at the very last step of the whole
    //  application -- when peer connection is completely shutdown
    //WSACleanup();
}

void DirectTcpServer::Send(std::string message) {
    int size = message.size();
    const char *msg = message.c_str();
    int sendResult = send(_clientSocket, message.c_str(), message.size(), 0);
    if (sendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(_clientSocket);
        WSACleanup();
        return;
    }

    printf("Bytes sent: %d\n", sendResult);
    // Hank: hacking, Android side use api readline
    send(_clientSocket, "\n", 1, 0);
}

void DirectTcpServer::ListeningLoop() {}
