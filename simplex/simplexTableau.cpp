#include <fstream>

#include "simplexTableau.h"

simplexTableau::simplexTableau() 
{
	constructMatrix(2, 1);
}

simplexTableau::simplexTableau(int numVar, int numConstr) 
{
	constructMatrix(numVar, numConstr);
}

simplexTableau::~simplexTableau() 
{
	destroyMatrix();
}

tableauErrorCode simplexTableau::changeValue(float value, int row, int col)
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
		float minValue = valueMatrix[rowNumber][colNumber];
		for (int rowNum = rowNumber; rowNum < numberOfConstraints; rowNum++)
		{
			float currentValue = valueMatrix[rowNum][colNumber];
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
	float pivotValue = valueMatrix[actualRow][actualCol];
	int rowNum = 0;
	int colNum = 0;
	float newValue = pivotValue;

	float** pivotMatrix = new float* [numberOfConstraints + 1];
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		pivotMatrix[rowNum] = new float[numberOfVariables + 1];
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
				float slotValue = valueMatrix[actualRow][colNum];
				float newValue = slotValue / pivotValue;
				pivotMatrix[rowNum][colNum] = newValue;
			}
			continue;
		}
		float colValue = valueMatrix[rowNum][actualCol];
		for (colNum = 0; colNum <= numberOfVariables; colNum++)
		{
			float rowValue = valueMatrix[actualRow][colNum];
			float slotValue = valueMatrix[rowNum][colNum];
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
	delete pivotMatrix;
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

tableauErrorCode simplexTableau::exportMatrix()
{
	std::fstream coutStream;
	coutStream.open("output.csv", std::fstream::out);
	coutStream << "Hi";
	coutStream.close();
	return FUNCTION_NOT_IMPLEMENTED;
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
	float minValue = -1;
	for (int rowNum = 0; rowNum < numberOfConstraints; rowNum++)
	{
		if (valueMatrix[rowNum][colNumber] > 0)
		{
			unboundedSolution = false;
			float tempValue = valueMatrix[rowNum][numberOfVariables] / valueMatrix[rowNum][colNumber];
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

float simplexTableau::getOptimalValue()
{
	return -1 * valueMatrix[numberOfConstraints][numberOfVariables];
}

float simplexTableau::getVariableValue(int row)
{
	return valueMatrix[row][numberOfVariables];
}

variableNumValue simplexTableau::getIndepVariableNum(int colNum)
{
	return getVariableNum(true, colNum);
}

variableNumValue simplexTableau::getDepVariableNum(int rowNum)
{
	return getVariableNum(false, rowNum);
}

void simplexTableau::constructMatrix(int numVar, int numConstr) 
{
	valueMatrix = new float* [numConstr + 1];
	for (int rowNum = 0; rowNum <= numConstr; rowNum++) 
	{
		valueMatrix[rowNum] = new float[numVar + 1];
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