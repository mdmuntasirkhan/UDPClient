#include <iostream>
#include <string.h>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

void Client(); /// Prototyping 

Uint8 dataBuffer[512]; /// This is just a byte array

int main(int argc, char*argv[]){
	Client();
	return 1;
}

void Client(){
	NET_DatagramSocket *sd;
	NET_Address *srvadd;
	NET_Datagram *p=nullptr;

	bool isRunning;
	
	/// Initialize SDL_net subsystem
	if (NET_Init() == false) {
		std::cerr << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
 
	/// Open a socket and bind the port to the OS
	if(!(sd = NET_CreateDatagramSocket(NULL, 0x1A2B,0))) {
		std::cerr << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	
 
	/// Resolve server name 
	srvadd = NET_ResolveHostname("127.0.0.1");
	if (srvadd == NULL) {
		std::cerr << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	///Make space for the packet
	p = new NET_Datagram{};
	p->buf = dataBuffer;

	
	///Main loop
	isRunning = true;
	while (isRunning){
		std::cout << "Write something: ";
		fgets((char*)dataBuffer,sizeof(dataBuffer)-1,stdin);
		std::cout << std::endl;
		


		
		p->addr = srvadd;	/// Set the destination host
		p->port = 0x1A2B;	/// and destination port 
		p->buflen = strlen((char*)dataBuffer)+1;
		p->buf = dataBuffer;

		NET_SendDatagram(sd, srvadd, p->port, p->buf, p->buflen); 
 
		/// Quit if packet contains "quit" 
		if (strcmp((char *)p->buf, "quit") == 0){
			isRunning = false;
		}
	}
 

	NET_DestroyDatagram(p);
	NET_Quit();
 
	return;
}


