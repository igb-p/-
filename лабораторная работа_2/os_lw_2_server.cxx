#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <signal.h>
#include <vector>

using namespace std;

sigset_t blockedMask, origMask;
volatile sig_atomic_t wasSigHup = 0;


void sigHupHandler(int r)
{  
	wasSigHup = 1;
}

void sigRegistrator()
{
	struct sigaction sa;
	sigaction(SIGHUP, NULL, &sa); 
	sa.sa_handler = sigHupHandler;
	sa.sa_flags |= SA_RESTART;
	sigaction(SIGHUP, &sa, NULL);
}

void sigBlocking()
{ 
	sigemptyset(&blockedMask);
	sigaddset(&blockedMask, SIGHUP);
	sigprocmask(SIG_BLOCK, &blockedMask, &origMask);
	sigemptyset(&origMask);
}

int main()
{
      int supersock = socket(AF_INET, SOCK_STREAM, 0);
      int client, acception, len, maxFd;
     // vector <int> clients;
      char buf[1024];
      
      if(supersock < 0)
      {
              perror("Error calling socket");
              return 0;
      }

      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(18666);
      addr.sin_addr.s_addr = htonl(INADDR_ANY);
      if( bind(supersock, (struct sockaddr *)&addr, sizeof(addr)) < 0 )
      {
              perror("Error calling bind");
              return 0;
      }

      if( listen(supersock, 5) )
      {
              perror("Error calling listen");
              return 0;
      }
      
     sigRegistrator();
     sigBlocking();
     
      for(;;)
      {
		  fd_set server_fds;
		  FD_ZERO(&server_fds);
		  FD_SET(supersock, &server_fds);
		  
		  if(maxFd<supersock) maxFd=supersock+1;
		  
		  if(client!=0)
		  {
			  FD_SET(client, &server_fds);
			  if(maxFd<client) maxFd=client+1;
		  }
		  
		  if(pselect(maxFd, &server_fds, NULL, NULL, NULL, &origMask)==-1)
		  {
			  if(errno==EINTR)
			  {
				  if(wasSigHup==1)
				  {
					  cout << "signal" << endl;
					  close(acception);
					  close(client);
					  close(supersock);
				  }
			  }
			  else 
			  {
				  cout <<"error"; 
				  close(acception);
				  close(client);
				  close(supersock);
				  exit(2);
			  }
		  } 
		  if(FD_ISSET(supersock, &server_fds))
		  {
			  struct sockaddr_in client_ad;
			  socklen_t cl_ad_len = sizeof(client_ad);
			  acception = accept(supersock, (struct sockaddr*) &client_ad, &cl_ad_len);
			  if(acception==-1)
			  {
				  perror("accept");
				  exit(EXIT_FAILURE);
			  }
			  cout <<"new connection"<<endl;
			  if(client==0)
			  {
				  client=acception;
			  }
			  else {close(acception);}
	     }
		  if (FD_ISSET(client, &server_fds))
		  {
			  len=read(client, buf, 1024);
			  if(len<=0)
			  {
				  close(client);
				  FD_CLR(client, &server_fds);
			  }
			  cout <<len<<endl;
		  }
	  }
	  close(acception);
	  close(client);
	  close(supersock);
	  return 0;
}
