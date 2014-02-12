//
//  ChatSettings.h
//  ChatServer
//
//  Created by Andrei Ionescu on 2/11/14.
//  Copyright (c) 2014 Andrei Ionescu. All rights reserved.
//

#ifndef __ChatServer__ChatSettings__
#define __ChatServer__ChatSettings__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

namespace CS
{
    struct ChatSettings
    {
    public:
        //ChatSettings();
        
        std::string port = "5556";
        std::string hostname = "localhost";
        // The struct that getaddrinfo() fills up with data.
        addrinfo host_info;
        // Pointer to the to the linked list of host_info's.
        addrinfo *host_info_list;
    };
}
#endif /* defined(__ChatServer__ChatSettings__) */
