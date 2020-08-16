#include "variableName.h"

variableName::variableName()
{
	this->indep = false;
	this->number = -1;
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
	if (indep)
	{
		stringRepresentation = "x";
	}
	else
	{
		stringRepresentation = "t";
	}
	stringRepresentation = stringRepresentation + std::to_string(this->number);
}

variableName::~variableName()
{
}

void variableName::setIndep(bool newIndep)
{
	indep = newIndep;
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
}

bool variableName::getIndep()
{
	return indep;
}

int variableName::getNumber()
{
	return number;
}