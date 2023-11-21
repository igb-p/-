#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <signal.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	string message;
	int s = socket(AF_INET,SOCK_STREAM,0);
	if(s < 0)
	{
		perror( "Error calling socket" );
        return 0;
    }

     struct sockaddr_in peer;
     peer.sin_family = AF_INET;
     peer.sin_port = htons(18666);
     peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
     int result = connect(s,(struct sockaddr*)&peer,sizeof(peer));
     if( result< 0)
     {
		  perror( "Error calling connect" );
		  return 0;
	 }
      for(;;)
      {
		cin>>message;
		cout<<message<<endl;  
		
		if(message == "exit")
		{
			close(s);
			break;
		}
		send(s, message.c_str(), message.length(), 0);
	   }
	   
	   return 0;
}
