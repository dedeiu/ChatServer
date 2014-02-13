//
//  ChatServer.h
//  ChatServer
//
//  Created by Andrei Ionescu on 2/11/14.
//  Copyright (c) 2014 Andrei Ionescu. All rights reserved.
//

#ifndef __ChatServer__ChatServer__
#define __ChatServer__ChatServer__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
#include "ChatSettings.h"

namespace CS
{
    class ChatServer
    {
    public:
        enum logType { DEFAULT = 0, CSDEBUG = 1};
        
        ChatServer(ChatSettings* chatSettings, logType type);
        ~ChatServer();
        void Run();
        
        void setLogLevel(logType type);
    private:
        // Initialize
        void Init();
        
        // Hide default constructor
        ChatServer();
        
        // Settings
        ChatSettings* chatSettings;

        // Logger
        int logLevel = logType::DEFAULT;
        void log(const std::string& message);
        
        // The socket descripter
        int socketfd = 0;
    
        // ChatServer general status
        int status = 0;
    
        void runServer();

        bool stopServer = false;

//        std::mutex lock;
    };
}

#endif /* defined(__ChatServer__ChatServer__) */
