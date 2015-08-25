#include "stdafx.h"
#include "CharacterCard.h"


CharacterCard::CharacterCard()
{
}

CharacterCard::~CharacterCard()
{
}

int CharacterCard::getID()
{
	return mID;
}

std::string CharacterCard::getName()
{
	return mName;
}

std::string CharacterCard::getPowerDescription()
{

	if (mName == "Moordenaar")
		return "Moordenaar: Hij kiest een karakter die hij deze ronde wil vermoorden.\n"
			"Als het gekozen karakter wordt genoemd, geeft deze\n"
			" zich niet bloot en komt hij deze ronde ook niet aan de beurt.\n"
			" Voorbeeld : als de moordenaar deze ronde de \n "
			"koopman aanwijst, dan slaat deze in stilte zijn beurt over.";
	
	 if (mName == "Dief")
		 return "Dief: Hij kiest een karakter, die hij deze ronde wil bestelen.\n"
		 "Hij mag de moordenaar of diens slachtoffer niet\n"
		 "aanwijzen.Als een karakter door de dief is gekozen,\n"
		 "dan geeft deze zich pas bloot als hij aan de beurt is.Het"
		 "bestolen karakter geeft zich niet in de beurt van de dief bloot!\n"
		 "Hij geeft al het goud dat hij bezit pas aan het\n"
		 "begin van zijn eigen beurt aan de dief.\n"
		 "Het goud dat het bestolen karakter tijdens zijn beurt ontvangt mag houden.";

	if (mName == "Magiër")
		return "Magiër: Hij mag of\n"
		"1. al zijn handkaarten(ook als dit 0 is) voor alle handkaarten van een andere speler omruilen\n" 
		"of\n"
		"2. naar keuze een aantal handkaarten afleggen en een gelijk aantal gebouwenkaarten trekken";

	if (mName == "Prediker")
		return "Prediker: Zijn gebouwenkaarten mogen door de \n"
		"condotierre niet verwijderd worden. De prediker onvangt 1 goudstuk\n"
		"voor elk blauw gebouw dat hij voor zich heeft liggen.";

	if (mName == "Koopman")
		return "Koopman: Hij ontvangt 1 goudstuk. Bovendien ontvangt hij 1 goudstuk voor elk groen gebouw dat hij voor zich heeft liggen.";
	
	if (mName == "Bouwmeester")
		return "Bouwemeester: Hij trekt 2 gebouwenkaarten. \n De bouwmeester mag in zijn beurt tot 3 gebouwenkaarten uitleggen.\n"
		"De bouwmeester kan derhalve aan het einde van het spel als enige meer dan 8 gebouwen bezitten.";

	if (mName == "Condottiere")
		return "Condottiere: Hij mag een gebouw bij een andere speler weghalen.\nGebouwen die maar 1 goudstuk waard zijn mag hij"
		"kosteloos verwijderen.\nVoor andere gebouwen moet hij 1 goudstuk minder betalen dan ze hun eigenaar gekost"
		"hebben.Het goud wordt aan de bank betaald.\nHet verwijderde gebouw gaat op de aflegstapel.\nDe condotierre"
		"mag ook gebouwen van vermoorde karakters verwijderen.\nHij mag echter geen gebouwen verwijderen uit"
		"steden die al uit 8 of meer gebouwen bestaan.\nDe condotierre ontvangt 1 goudstuk voor elk rood gebouw dat"
		"hij voor zich heeft liggen.";

	return "no cards with that name...";
}

