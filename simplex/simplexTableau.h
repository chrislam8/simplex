#pragma once

#include "simplexTableauEnums.h"
#include "variableName.h"

class simplexTableau {
public:
	simplexTableau();
	simplexTableau(int numVar, int numConstr);
	simplexTableau(const simplexTableau& copy);
	~simplexTableau();

	tableauErrorCode changeValue(double value, int row, int col);
	tableauErrorCode pivotFeasibility(checkValue feasibilityCheck);
	tableauErrorCode pivot(int row, int col);
	tableauErrorCode exportMatrix(std::string fileName);
	tableauErrorCode importMatrix(std::string fileName);

	checkValue feasibleSolutionsCheck();
	checkValue optimalSolutionCheck();
	checkValue unboundedSolutionCheck(int colNumber);

	double getOptimalValue();
	double getVariableValue(int row);

	variableNumValue getIndepVariableNum(int colNum);
	variableNumValue getDepVariableNum(int rowNum);

	simplexTableau& operator=(const simplexTableau& copy);

private:
	double** valueMatrix;
	variableName* indepVar;
	variableName* depVar;
	void constructMatrix(int numVar, int numConstr);
	void destroyMatrix();
	void copyMatrix(const simplexTableau& copy);
	void increaseSizeVar(bool indep, int newSize);
	void increaseSizeMatrix(int newRows, int newColumns);
	variableNumValue getVariableNum(bool indep, int number);

	int numberOfVariables;
	int numberOfConstraints;
};
