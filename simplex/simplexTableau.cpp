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
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++) 
	{
		delete valueMatrix[rowNum];
		valueMatrix[rowNum] = nullptr;
	}
	delete valueMatrix;
	valueMatrix = nullptr;
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
	return TABLEAU_SUCCESS;
}

tableauErrorCode simplexTableau::printMatrix()
{
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

simplexTableau& simplexTableau::operator=(const simplexTableau& copy)
{
	if (&copy == this)
	{
		return *this;
	}
	numberOfConstraints = copy.numberOfConstraints;
	numberOfVariables = copy.numberOfVariables;
	valueMatrix = new float* [numberOfConstraints + 1];
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
	{
		valueMatrix[rowNum] = new float[numberOfVariables + 1];
	}
	int i, j;
	for (i = 0; i <= numberOfConstraints; i++)
	{
		for (j = 0; j <= numberOfVariables; j++)
		{
			valueMatrix[i][j] = copy.valueMatrix[i][j];
		}
	}
	return *this;
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
}