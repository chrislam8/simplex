#pragma once

#include <string>

class variableName {
public:
	variableName();
	variableName(bool indep, int number);
	~variableName();

	//setters
	void setIndep(bool newIndep);
	void setNumber(int newNumber);

	//getters
	bool getIndep();
	int getNumber();
	std::string getString();

private:
	bool indep;
	int number;
	std::string stringRepresentation;

	void setStringRepresentation();
};