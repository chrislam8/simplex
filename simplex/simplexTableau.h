#pragma once

#include "simplexTableauEnums.h"
#include "variableName.h"

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

	variableNumValue getIndepVariableNum(int colNum);
	variableNumValue getDepVariableNum(int rowNum);

private:
	float** valueMatrix;
	variableName* indepVar;
	variableName* depVar;
	void constructMatrix(int numVar, int numConstr);
	void destroyMatrix();
	variableNumValue getVariableNum(bool indep, int number);

	int numberOfVariables;
	int numberOfConstraints;
};