#include "simplexMatrix.h"

simplexMatrix::simplexMatrix() 
{
	constructMatrix(2, 1);
}

simplexMatrix::simplexMatrix(int numVar, int numConstr) 
{
	constructMatrix(numVar, numConstr);
}

simplexMatrix::~simplexMatrix() 
{
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++) 
	{
		delete valueMatrix[rowNum];
		valueMatrix[rowNum] = nullptr;
	}
	delete valueMatrix;
	valueMatrix = nullptr;
}

matrixErrorCode simplexMatrix::changeValue(float value, int row, int col)
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
	return SUCCESS;
}

matrixErrorCode simplexMatrix::pivotFeasibility(checkValue feasibilityCheck)
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

matrixErrorCode simplexMatrix::pivot(int row, int col) 
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
				valueMatrix[rowNum][colNum] = newValue;
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
			valueMatrix[rowNum][colNum] = newValue;
		}
	}
	newValue = 1 / pivotValue;
	valueMatrix[actualRow][actualCol] = newValue;
	return SUCCESS;
}

matrixErrorCode simplexMatrix::printMatrix()
{
	return FUNCTION_NOT_IMPLEMENTED;
}

checkValue simplexMatrix::feasibleSolutionsCheck()
{
	bool feasibleSolutions = true;
	bool noFeasibleSolutions = true;
	int rowNumber = -1;
	int colNumber = -1;
	matrixLocation pivotLocation = std::make_pair(rowNumber, colNumber);
	checkValue result = std::make_pair(feasibleSolutions, pivotLocation);
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++)
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
	for (int colNum = 0; colNum <= numberOfVariables; colNum++)
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

checkValue simplexMatrix::optimalSolutionCheck()
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

checkValue simplexMatrix::unboundedSolutionCheck(int colNumber)
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
			if (rowNum == 0)
			{
				minValue = tempValue;
				rowNumber = rowNum;
			}
			else if (minValue > tempValue)
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

float simplexMatrix::getOptimalValue()
{
	return -1 * valueMatrix[numberOfConstraints][numberOfVariables];
}

float simplexMatrix::getVariableValue(int row)
{
	return valueMatrix[row][numberOfVariables];
}

void simplexMatrix::constructMatrix(int numVar, int numConstr) 
{
	valueMatrix = new float* [numConstr + 1];
	for (int rowNum = 0; rowNum <= numConstr; rowNum++) 
	{
		valueMatrix[rowNum] = new float[numVar + 1];
	}
	
	numberOfVariables = numVar;
	numberOfConstraints = numConstr;
}