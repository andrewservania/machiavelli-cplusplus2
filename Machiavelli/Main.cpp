// Machiavelli.cpp : Defines the entry point for the console application.
// Andrew Servania
//Studentennummer: 2036129
//Avans Hogeschool Den Bosch
//Datum: 09-01-2015
//Vak: C++2

#include "stdafx.h"
#include <memory>
#include "MachiavelliClient.h"

std::unique_ptr<MachiavelliClient> machiavelliClient;

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		machiavelliClient = std::make_unique<MachiavelliClient>();
	}
	catch (...)
	{
		std::cerr << "Whoa! If you're reading this message then something technical happened that was not supposed to happen. Please restart the machiavelli client.\nThank you!";
	}
	return 0;
}

