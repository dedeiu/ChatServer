
#include <iostream>
#include "ChatServer.h"
#include "ChatSettings.h"

using namespace CS;

int main()
{
    // settings
    ChatSettings* cs = new ChatSettings();
    cs->hostname = "localhost";
    cs->port     = "5556";
    
    ChatServer* css = new ChatServer(cs, css->logType::CSDEBUG);
    css->Run();
    return 0;
}
