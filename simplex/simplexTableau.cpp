#include <fstream>
#include <sstream>

#include "simplexTableau.h"

simplexTableau::simplexTableau() 
{
	constructMatrix(2, 1, false);
}

simplexTableau::simplexTableau(int numVar, int numConstr, bool miniTableau) 
{
	constructMatrix(numVar, numConstr, miniTableau);
}

simplexTableau::simplexTableau(const simplexTableau& copy)
{
	copyMatrix(copy);
}

simplexTableau::~simplexTableau() 
{
	destroyMatrix();
}

tableauErrorCode simplexTableau::changeValue(double value, int row, int col)
{
	if (row > numberOfConstraints + 1 || row <= 0)
	{
		return INVALID_ROW_NUMBER;
	}

	if (col > numberOfVariables + 1 || col <= 0) 
	{
		return INVALID_COLUMN_NUMBER;
	}

	valueMatrix[row - 1][col - 1] = value;
	return TABLEAU_SUCCESS;
}

tableauErrorCode simplexTableau::pivotFeasibility(checkValue feasibilityCheck)
{
	matrixLocation pivotLocation = feasibilityCheck.second;
	int rowNumber = pivotLocation.first;
	int colNumber = pivotLocation.second;
	if (rowNumber < (numberOfConstraints - 1))
	{
		double minValue = valueMatrix[rowNumber][colNumber];
		for (int rowNum = rowNumber; rowNum < numberOfConstraints; rowNum++)
		{
			double currentValue = valueMatrix[rowNum][colNumber];
			if (currentValue < minValue)
			{
				minValue = currentValue;
				rowNumber = rowNum;
			}
		}
	}
	return pivot(rowNumber, colNumber);
}

tableauErrorCode simplexTableau::pivot(int row, int col) 
{
	if (row > numberOfConstraints || row <= 0)
	{
		return INVALID_ROW_NUMBER;
	}

	if (col > numberOfVariables || col <= 0) 
	{
		return INVALID_COLUMN_NUMBER;
	}

	int actualRow = row - 1;
	int actualCol = col - 1;
	double pivotValue = valueMatrix[actualRow][actualCol];
	int rowNum = 0;
	int colNum = 0;
	double newValue = pivotValue;

	double** pivotMatrix = new double* [numberOfConstraints + 1];
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		pivotMatrix[rowNum] = new double[numberOfVariables + 1];
	}

	if (pivotValue == 0)
	{
		return PIVOT_VALUE_CANNOT_BE_ZERO;
	}

	for (rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		if (rowNum == actualRow)
		{
			for (colNum = 0; colNum <= numberOfVariables; colNum++)
			{
				if (colNum == actualCol)
				{
					continue;
				}
				double slotValue = valueMatrix[actualRow][colNum];
				double newValue = slotValue / pivotValue;
				pivotMatrix[rowNum][colNum] = newValue;
			}
			continue;
		}
		double colValue = valueMatrix[rowNum][actualCol];
		for (colNum = 0; colNum <= numberOfVariables; colNum++)
		{
			double rowValue = valueMatrix[actualRow][colNum];
			double slotValue = valueMatrix[rowNum][colNum];
			if (colNum == actualCol)
			{
				newValue = -1 * slotValue / pivotValue;
			}
			else
			{
				newValue = (slotValue * pivotValue - rowValue * colValue) / pivotValue;
			}
			pivotMatrix[rowNum][colNum] = newValue;
		}
	}
	newValue = 1 / pivotValue;
	pivotMatrix[actualRow][actualCol] = newValue;
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		for (int colNum = 0; colNum <= numberOfVariables; colNum++)
		{
			valueMatrix[rowNum][colNum] = pivotMatrix[rowNum][colNum];
		}
	}
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		delete pivotMatrix[rowNum];
		pivotMatrix[rowNum] = nullptr;
	}
	delete [] pivotMatrix;
	pivotMatrix = nullptr;
	variableName tempVar;
	if ((indepVar[col - 1]).getIndep() && !((depVar[row - 1]).getIndep())) {
		(indepVar[col - 1]).setIndep(false);
		(depVar[row - 1]).setIndep(true);
	}
	else if (!((indepVar[col - 1]).getIndep()) && (depVar[row - 1]).getIndep()) {
		(indepVar[col - 1]).setIndep(true);
		(depVar[row - 1]).setIndep(false);
	}
	tempVar.setNumber((indepVar[col - 1]).getNumber());
	(indepVar[col - 1]).setNumber((depVar[row - 1]).getNumber());
	(depVar[row - 1]).setNumber(tempVar.getNumber());
	return TABLEAU_SUCCESS;
}

tableauErrorCode simplexTableau::exportMatrix(std::string fileName)
{
	std::fstream coutStream;
	coutStream.open(fileName.c_str(), std::fstream::out);
	if (coutStream.fail())
	{
		return FILE_OPEN_FAILED;
	}
	int i, j;
	for (i = 0; i < numberOfVariables; ++i)
	{
		coutStream << indepVar[i].getString() << ",";
	}
	coutStream << "-1" << std::endl;
	for (i = 0; i <= numberOfConstraints; ++i)
	{
		for (j = 0; j <= numberOfVariables; j++)
		{
			coutStream << valueMatrix[i][j] << ",";
		}
		if (i != numberOfConstraints)
		{
			coutStream << " = -" << depVar[i].getString() << std::endl;
		}
		else
		{
			coutStream << " = f" << std::endl;
		}
	}
	coutStream.close();
	return EXPORT_SUCCESS;
}

tableauErrorCode simplexTableau::importMatrix(std::string fileName, bool miniTableau)
{
	std::fstream cinStream;
	cinStream.open(fileName.c_str(), std::fstream::in);
	std::string currentLine;
	int rowNumber = 0;
	int numberOfColumns = 0;
	int columnNumber = 0;
	variableName variableInput;
	if (cinStream.fail())
	{
		return FILE_OPEN_FAILED;
	}

	while (std::getline(cinStream, currentLine))
	{
		std::stringstream ss(currentLine);
		std::string currentEntry;
		while (std::getline(ss, currentEntry, ','))
		{
			if (rowNumber == 0)
			{
				/*
				This initializes the top row of the tableau which indicates the independent variables.
				*/
				++numberOfColumns;
				if (currentEntry[0] == 't')
				{
					variableInput.setIndep(false);
				}
				else if (currentEntry[0] == 'x')
				{
					variableInput.setIndep(true);
				}
				else
				{
					++columnNumber;
					continue;
				}
				currentEntry.erase(currentEntry.begin());
				int variableNum = atoi(currentEntry.c_str());
				variableInput.setNumber(variableNum);
				if (columnNumber >= numberOfVariables)
				{
					increaseSizeVar(true, columnNumber + 1);
				}
				indepVar[columnNumber] = variableInput;
			}
			else
			{
				if (columnNumber < numberOfColumns)
				{
					/*
					This initializes the values in the main values of the tableau.
					*/
					double number = atof(currentEntry.c_str());
					valueMatrix[rowNumber - 1][columnNumber] = number;
				}
				else
				{
					/*
					This initializes the right most column of the tableau which indicates the dependent variables.
					*/
					if (currentEntry[0] == 't')
					{
						variableInput.setIndep(false);
					}
					else if (currentEntry[0] == 'x')
					{
						variableInput.setIndep(true);
					}
					else
					{
						++columnNumber;
						continue;
					}
					currentEntry.erase(0);
					int variableNum = atoi(currentEntry.c_str());
					variableInput.setNumber(variableNum);
					if (rowNumber - 1 >= numberOfConstraints)
					{
						increaseSizeVar(false, rowNumber);
					}
					indepVar[rowNumber - 1] = variableInput;
				}
			}
			++columnNumber;
		}
		++rowNumber;
		columnNumber = 0;
	}
	numberOfConstraints = rowNumber - 2;
	numberOfVariables = numberOfColumns - 1;
	cinStream.close();
	minTableau = miniTableau;
	return IMPORT_SUCCESS;
}

checkValue simplexTableau::feasibleSolutionsCheck()
{
	bool feasibleSolutions = true;
	bool noFeasibleSolutions = true;
	int rowNumber = -1;
	int colNumber = -1;
	matrixLocation pivotLocation = std::make_pair(rowNumber, colNumber);
	checkValue result = std::make_pair(feasibleSolutions, pivotLocation);
	for (int rowNum = 0; rowNum < numberOfConstraints; rowNum++)
	{
		if (valueMatrix[rowNum][numberOfVariables] < 0)
		{
			feasibleSolutions = false;
			rowNumber = rowNum;
		}
	}
	if (feasibleSolutions)
	{
		return result;
	}
	for (int colNum = 0; colNum < numberOfVariables; colNum++)
	{
		if (valueMatrix[rowNumber][colNum] < 0)
		{
			noFeasibleSolutions = false;
			colNumber = colNum;
		}
	}
	if (noFeasibleSolutions)
	{
		rowNumber = -1;
	}
	else
	{
		++rowNumber;
		++colNumber;
	}
	pivotLocation = std::make_pair(rowNumber, colNumber);
	result = std::make_pair(!(noFeasibleSolutions), pivotLocation);
	return result;
}

checkValue simplexTableau::optimalSolutionCheck()
{
	bool optimalSolution = true;
	int rowNumber = -1;
	int colNumber = -1;
	matrixLocation pivotLocation = std::make_pair(rowNumber, colNumber);
	checkValue result = std::make_pair(optimalSolution, pivotLocation);
	for (int colNum = 0; colNum < numberOfVariables; colNum++)
	{
		if (valueMatrix[numberOfConstraints][colNum] > 0)
		{
			optimalSolution = false;
			colNumber = colNum;
		}
	}
	if (!optimalSolution)
	{
		pivotLocation = std::make_pair(rowNumber, colNumber);
		result = std::make_pair(optimalSolution, pivotLocation);
	}
	return result;
}

checkValue simplexTableau::unboundedSolutionCheck(int colNumber)
{
	bool unboundedSolution = true;
	int rowNumber = -1;
	matrixLocation pivotLocation = std::make_pair(rowNumber, colNumber);
	checkValue result = std::make_pair(unboundedSolution, pivotLocation);
	double minValue = -1;
	for (int rowNum = 0; rowNum < numberOfConstraints; rowNum++)
	{
		if (valueMatrix[rowNum][colNumber] > 0)
		{
			unboundedSolution = false;
			double tempValue = valueMatrix[rowNum][numberOfVariables] / valueMatrix[rowNum][colNumber];
			if (minValue < 0)
			{
				minValue = tempValue;
				rowNumber = rowNum;
			}
			if (minValue > tempValue)
			{
				minValue = tempValue;
				rowNumber = rowNum;
			}
		}
	}
	if (!unboundedSolution)
	{
		pivotLocation = std::make_pair(rowNumber, colNumber);
		result = std::make_pair(unboundedSolution, pivotLocation);
	}
	return result;
}

double simplexTableau::getOptimalValue()
{
	return -1 * valueMatrix[numberOfConstraints][numberOfVariables];
}

double simplexTableau::getVariableValue(int row)
{
	return valueMatrix[row][numberOfVariables];
}

simplexTableau& simplexTableau::operator=(const simplexTableau& copy)
{
	if (&copy == this)
	{
		return *this;
	}
	copyMatrix(copy);
	return *this;
}

variableNumValue simplexTableau::getIndepVariableNum(int colNum)
{
	return getVariableNum(true, colNum);
}

variableNumValue simplexTableau::getDepVariableNum(int rowNum)
{
	return getVariableNum(false, rowNum);
}

void simplexTableau::constructMatrix(int numVar, int numConstr, bool miniTableau) 
{
	valueMatrix = new double* [numConstr + 1];
	for (int rowNum = 0; rowNum <= numConstr; rowNum++) 
	{
		valueMatrix[rowNum] = new double[numVar + 1];
	}
	
	numberOfVariables = numVar;
	numberOfConstraints = numConstr;

	indepVar = new variableName[numVar];
	depVar = new variableName[numConstr];
	int i;
	for (i = 0; i < numVar; i++) {
		(indepVar[i]).setIndep(true);
		(indepVar[i]).setNumber(i + 1);
	}
	for (i = 0; i < numConstr; i++) {
		(depVar[i]).setIndep(false);
		(depVar[i]).setNumber(i + 1);
	}
	minTableau = miniTableau;
}

void simplexTableau::destroyMatrix()
{
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		delete valueMatrix[rowNum];
		valueMatrix[rowNum] = nullptr;
	}
	delete valueMatrix;
	valueMatrix = nullptr;
	delete[]indepVar;
	indepVar = NULL;
	delete[]depVar;
	depVar = NULL;
}

void simplexTableau::copyMatrix(const simplexTableau& copy)
{
	numberOfConstraints = copy.numberOfConstraints;
	numberOfVariables = copy.numberOfVariables;
	valueMatrix = new double* [numberOfConstraints + 1];
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		valueMatrix[rowNum] = new double[numberOfVariables + 1];
	}
	int i, j;
	for (i = 0; i <= numberOfConstraints; i++)
	{
		for (j = 0; j <= numberOfVariables; j++)
		{
			valueMatrix[i][j] = copy.valueMatrix[i][j];
		}
	}
}

void simplexTableau::increaseSizeVar(bool indep, int newSize)
{
	variableName* tempVar = NULL;
	variableName* createVar = new variableName[newSize];
	int oldSize = 0;
	int minSize = newSize;
	int i = 0;
	if (indep)
	{
		tempVar = indepVar;
		oldSize = numberOfVariables;
	}
	else
	{
		tempVar = depVar;
		oldSize = numberOfConstraints;
	}
	if (oldSize < newSize)
	{
		minSize = oldSize;
	}
	for (i = 0; i < minSize; ++i)
	{
		createVar[i] = tempVar[i];
	}
	delete[] tempVar;
	tempVar = NULL;
	if (indep)
	{
		indepVar = createVar;
		numberOfVariables = newSize;
	}
	else
	{
		depVar = createVar;
		numberOfConstraints = newSize;
	}
}

variableNumValue simplexTableau::getVariableNum(bool indep, int number)
{
	int variableNum;
	variableNumValue result = std::make_pair(-1, VARIABLE_NOT_INDEPENDENT);
	if (number < 0 || (number >= numberOfConstraints && number >= numberOfVariables))
	{
		tableauErrorCode errorCode;
		if (indep)
		{
			errorCode = INVALID_COLUMN_NUMBER;
		}
		else
		{
			errorCode = INVALID_ROW_NUMBER;
		}
		result = std::make_pair(-1, errorCode);
	}
	else if (indep && indepVar[number].getIndep())
	{
		variableNum = indepVar[number].getNumber();
		result = std::make_pair(variableNum, TABLEAU_SUCCESS);
	}
	else if (!indep && depVar[number].getIndep())
	{
		variableNum = depVar[number].getNumber();
		result = std::make_pair(variableNum, TABLEAU_SUCCESS);
	}
	return result;
}
