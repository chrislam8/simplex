#pragma once

#include "simplexTableauEnums.h"

class simplexTableau {
public:
	simplexTableau();
	simplexTableau(int numVar, int numConstr);
	~simplexTableau();

	tableauErrorCode changeValue(float value, int row, int col);
	tableauErrorCode pivotFeasibility(checkValue feasibilityCheck);
	tableauErrorCode pivot(int row, int col);
	tableauErrorCode printMatrix();

	checkValue feasibleSolutionsCheck();
	checkValue optimalSolutionCheck();
	checkValue unboundedSolutionCheck(int colNumber);

	float getOptimalValue();
	float getVariableValue(int row);

	simplexTableau& operator=(const simplexTableau& copy);

private:
	float** valueMatrix;
	void constructMatrix(int numVar, int numConstr);

	int numberOfVariables;
	int numberOfConstraints;
};