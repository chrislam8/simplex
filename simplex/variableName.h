#pragma once

#include <string>

class variableName {
public:
	variableName();
	variableName(bool indep, int number);
	variableName(const variableName& copy);
	~variableName();

	//setters
	void setIndep(bool newIndep);
	void setNumber(int newNumber);

	//getters
	bool getIndep();
	int getNumber();
	std::string getString();

	variableName& operator=(const variableName& copy);

private:
	bool indep;
	int number;
	std::string stringRepresentation;

	void setStringRepresentation();
	void copyVariable(const variableName& copy);
};