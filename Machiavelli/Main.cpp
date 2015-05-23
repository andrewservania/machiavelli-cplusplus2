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
	machiavelliClient = std::make_unique<MachiavelliClient>();
	return 0;
}

