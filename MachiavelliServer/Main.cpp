// MachiavelliServer.cpp : Defines the entry point for the console application.
// Andrew Servania
//Studentennummer: 2036129
//Datum: 09-01-2015
//Vak: C++2

#include "stdafx.h"

#include <memory>
#include <ctime>
#include "Server.h"


std::unique_ptr<Server> machiavelliServer;

int _tmain(int argc, _TCHAR* argv[])
{
	/*Necessary for random card-stack shuffling*/
	std::srand(unsigned(std::time(0)));
	try
	{	
		machiavelliServer = std::make_unique<Server>();
		machiavelliServer->runServer();
	}
	catch (...)
	{
		std::cerr << "Whoa! If you're reading this message then the server most likely "
					"tried to divide by zero and a blackhole-like vortex was emerging from within" 
					" the server and the server did the ultimate sacrifice and executed "
					"a self-destruct in order to save mankind! You're very lucky!\n"
					" Now please try restart the server again. :)";
		std::getchar();
	}
	return 0;
}

