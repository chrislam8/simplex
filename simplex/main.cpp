#include <iostream>
#include <string>

#include "test.h"

void runTests();
void helpText();
void about();

int main() 
{
	bool continuing = true;
	std::string input;
	std::string inputFile, inputCheck, inputExtension;
	simpl* instance = new simpl;
	while (continuing)
	{
		bool realCommand = false;
		std::cout << "What is your command?" << std::endl;
		std::cout << "The command 'help' prints out the available commands." << std::endl;
		std::getline(std::cin, input);
		if (input.length() >= 7)
		{
			inputFile = input;
			inputFile.erase(inputFile.begin(), inputFile.begin() + 7);
			inputCheck = input;
			inputCheck.erase(inputCheck.begin() + 6, inputCheck.end());
			inputExtension = inputFile;
			inputExtension.erase(inputExtension.begin(), inputExtension.end() - 4);
		}
		else
		{
			inputFile = "";
			inputCheck = "";
			inputExtension = "";
		}
		if (input == "exit")
		{
			continuing = false;
			realCommand = true;
		}
		else
		{
			std::cout << std::endl;
		}
		if (input == "test")
		{
			runTests();
			realCommand = true;
		}
		if (input == "help")
		{
			helpText();
			realCommand = true;
		}
		if (input == "about")
		{
			about();
			realCommand = true;
		}
		if (inputCheck == "import" && inputExtension == ".csv")
		{
			if (instance->importTableau(inputFile))
			{
				std::cout << inputFile << " successfully imported." << std::endl;
			}
			else
			{
				std::cout << inputFile << " failed to import properly." << std::endl;
			}
			realCommand = true;
		}
		if (inputCheck == "export" && inputExtension == ".csv")
		{
			if (instance->exportTableau(inputFile))
			{
				std::cout << "The tableau was successfully exported to " << inputFile << std::endl;
			}
			else
			{
				std::cout << "The tableau failed to be exported to " << inputFile << std::endl;
			}
			realCommand = true;
		}
		if (input == "perform simplex")
		{
			int numvar = instance->getNumberOfVariables();
			double* xValues = new double[numvar];
			double optimalValue = 0;
			simplexErrorCode simplexCode = instance->simplex(xValues, &optimalValue);
			if (simplexCode == SIMPLEX_SUCCESS)
			{
				std::cout << "Simplex successful." << std::endl;
				std::cout << "The optimal value for this problem is " << -1 * optimalValue << std::endl;
				std::cout << "The optimal x values for this problem are " << std::endl;
				int index = 0;
				for (index = 0; index < numvar; ++index)
				{
					std::cout << "x[" << index + 1 << "] = " << xValues[index] << "," << std::endl;
				}
			}
			else
			{
				std::cout << "Simplex failed." << std::endl;
			}
			delete[] xValues;
			realCommand = true;
		}
		if (!realCommand)
		{
			std::cout << input << " is not a real command. Try again.";
		}
		std::cout << std::endl << std::endl;
	}
	delete instance;
	return 0;
}

void runTests()
{
	allTests testSeq;
	testResultCodes testResult = testSeq.runAllTests();
	if (testResult.second != TEST_SUCCESS)
	{
		std::cout << "Test number #" << testResult.first << " failed with code " << testResult.second;
	}
	else
	{
		std::cout << "All tests succeeded";
	}
}

void helpText()
{
	std::cout << "List of commands:" << std::endl << std::endl;
	std::cout << "about - displays some information about this program" << std::endl;
	std::cout << "exit - exits the program" << std::endl;
	std::cout << "help - prints the list of available commands" << std::endl;
	std::cout << "test - runs all program tests" << std::endl;
	std::cout << "import file.csv - imports file.csv to the program" << std::endl;
	std::cout << "export file.csv - exports tableau to file.csv" << std::endl;
}

void about()
{
	std::cout << "Program Name: Linear Programming Calculator" << std::endl;
	std::cout << "Author: Christopher Lam" << std::endl;
	std::cout << "Version: 1.0.0.0" << std::endl;
	std::cout << "Last updated: December 21, 2020" << std::endl;
}