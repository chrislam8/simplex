#include "variableName.h"

variableName::variableName()
{
	this->indep = false;
	this->number = -1;
	setStringRepresentation();
}

variableName::variableName(bool indep, int number)
{
	this->indep = indep;
	if (number >= 0)
	{
		this->number = number;
	}
	else
	{
		this->number = -1;
	}
	setStringRepresentation();
}

variableName::~variableName()
{
}

void variableName::setIndep(bool newIndep)
{
	indep = newIndep;
	setStringRepresentation();
}

void variableName::setNumber(int newNumber)
{
	if (newNumber >= 0)
	{
		number = newNumber;
	}
	else
	{
		number = -1;
	}
	setStringRepresentation();
}

bool variableName::getIndep()
{
	return indep;
}

int variableName::getNumber()
{
	return number;
}

std::string variableName::getString()
{
	return stringRepresentation;
}

variableName& variableName::operator=(const variableName& copy)
{
	indep = copy.indep;
	number = copy.number;
	setStringRepresentation();
	return *this;
}

void variableName::setStringRepresentation()
{
	if (indep)
	{
		stringRepresentation = "x";
	}
	else
	{
		stringRepresentation = "t";
	}
	stringRepresentation = stringRepresentation + std::to_string(number);
}