#pragma once

#include "matrixErrorCode.h"

class simplexMatrix {
public:
	simplexMatrix();
	simplexMatrix(int numVar, int numConstr);
	~simplexMatrix();

	matrixErrorCode changeValue(float value, int row, int col);
	matrixErrorCode pivot(int row, int col);

	feasibilityCheckValue feasibleSolutionsCheck();

private:
	float** valueMatrix;
	void constructMatrix(int numVar, int numConstr);

	int numberOfVariables;
	int numberOfConstraints;
};