//
//  ChatServer.cpp
//  ChatServer
//
//  Created by Andrei Ionescu on 2/11/14.
//  Copyright (c) 2014 Andrei Ionescu. All rights reserved.
//

#include "ChatServer.h"
#include "ChatSettings.h"
#include <iostream>
#include <thread>

namespace CS
{
    ChatServer::ChatServer(ChatSettings* chatSettings, logType type)
    {
        this->setLogLevel(type);
        
        this->chatSettings = chatSettings;
        this->Init();
    }
    
    ChatServer::~ChatServer()
    {
        this->log("Stop listening thread.");
        this->stopServer = true;
        
        this->log("Delete host_info object.");
        delete &this->chatSettings->host_info;
        
        this->log("Free address info list.");
        freeaddrinfo(this->chatSettings->host_info_list);
        
        this->log("Call ChatSettings destructor.");
        this->chatSettings->~ChatSettings();
    }
    
    void ChatServer::setLogLevel(logType type)
    {
        this->logLevel = type;
    }
    
    void ChatServer::Run()
    {
        if(this->stopServer == true)
        {
            this->Init();
            this->stopServer = false;
        }
        
        std::thread t(&ChatServer::runServer, this);
        t.join();
    }
    
    void ChatServer::Init()
    {
        this->log("Setting up the server...");
        
        // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
        // to by hints must contain either 0 or a null pointer, as appropriate." When a struct
        // is created in c++, it will be given a block of memory. This memory is not nessesary
        // empty. Therefor we use the memset function to make sure all fields are NULL.
        memset(&(this->chatSettings->host_info), 0, sizeof this->chatSettings->host_info);
        
        // IP version not specified. Can be both.
        this->chatSettings->host_info.ai_family = AF_UNSPEC;
        
        // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
        this->chatSettings->host_info.ai_socktype = SOCK_STREAM;
        
        // IP Wildcard
        this->chatSettings->host_info.ai_flags = AI_PASSIVE;
        
        // Now fill up the linked list of host_info structs with google's address information.
        this->status = getaddrinfo("localhost", "5556", &(this->chatSettings->host_info), &(this->chatSettings->host_info_list));
        
        if(this->status != 0)
        {
            // getaddrinfo returns 0 on succes, or some other value when an error occured.
            // (translated into human readable text by the gai_gai_strerror function).
            this->log("getaddressinfo error: " + std::string(gai_strerror(this->status)));
        }
        
        this->log("Creating a socket...");

        this->socketfd = socket(this->chatSettings->host_info_list->ai_family, this->chatSettings->host_info_list->ai_socktype,
                                this->chatSettings->host_info_list->ai_protocol);
        
        if(this->socketfd == -1)
        {
            this->log("Socket error.");
        }
        
        this->log("Binding socket...");
        
        // we use to make the setsockopt() function to make sure the port is not in use
        // by a previous execution of our code. (see man page for more information)
        int yes = 1;
        this->status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        this->status = bind(this->socketfd, this->chatSettings->host_info_list->ai_addr, this->chatSettings->host_info_list->ai_addrlen);
        if (this->status == -1)
        {
            this->log("Bind error.");
        }

    }
    
    void ChatServer::log(const std::string& message)
    {
        switch(this->logLevel)
        {
            case logType::CSDEBUG:
                std::cout << message << std::endl;
                break;
            default:
            case logType::DEFAULT:
                // silent
                break;
        }
    }
    
    void ChatServer::runServer()
    {
        while(this->stopServer == false)
        {
            this->log("Listening for connections...");
            this->status = listen(this->socketfd, 5);
            
            if(this->status == -1)
            {
                this->log("Listen error.");
            }
            
            struct sockaddr_storage their_addr;
            socklen_t addr_size = sizeof(their_addr);
            int new_sd = accept(this->socketfd, (struct sockaddr *)&their_addr, &addr_size);
            
            if(new_sd == -1)
            {
                this->log("listen error");
            }
            else
            {
                this->log("Connection accepted. Using new socketfd : " + std::to_string(new_sd));
            }
            
            this->log("Waiting to recieve data...");

            ssize_t bytes_recieved;
            char incomming_data_buffer[1000];
            bytes_recieved = recv(new_sd, incomming_data_buffer,1000, 0);
            
            // If no data arrives, the program will just wait here until some data arrives.
            switch(bytes_recieved)
            {
                case -1:
                    this->log("Recieve error!.");
                    break;
                case 0:
                    this->log("Host shut down.");
                    break;
                default:
                    incomming_data_buffer[bytes_recieved] = '\0';
                    this->log(" bytes recieved :" + std::string(incomming_data_buffer));
                    this->log("sending back a message...");

                    char *msg = incomming_data_buffer;
                    int len;
                    
                    ssize_t bytes_sent;
                    len = strlen(msg);
                    bytes_sent = send(new_sd, msg, len, 0);
                    break;
            }
        }
    }
}