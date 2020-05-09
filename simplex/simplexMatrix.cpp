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

feasibilityCheckValue simplexMatrix::feasibleSolutionsCheck()
{
	bool feasibleSolutions = true;
	int rowNumber = -1;
	int colNumber = -1;
	matrixLocation pivotLocation = std::make_pair(rowNumber, colNumber);
	feasibilityCheckValue result = std::make_pair(feasibleSolutions, pivotLocation);
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
			feasibleSolutions = false;
			colNumber = colNum;
		}
	}
	if (feasibleSolutions)
	{
		rowNumber = -1;
	}
	else
	{
		++rowNumber;
		++colNumber;
	}
	pivotLocation = std::make_pair(rowNumber, colNumber);
	result = std::make_pair(feasibleSolutions, pivotLocation);
	return result;
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