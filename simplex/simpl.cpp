#include <iostream>
#include "simpl.h"


//public methods

simpl::simpl() {
	constructTab(2, 1);
}

simpl::simpl(int numvar, int numconstr) {
	constructTab(numvar, numconstr);
}

simpl::~simpl() {
	destroyTab();
}

bool simpl::changeValue(float value, int row, int col) {
	tableauErrorCode errorCode = tableau->changeValue(value, row, col);
	return (errorCode == TABLEAU_SUCCESS);
}

bool simpl::prTableau() {
	tableauErrorCode errorCode = tableau->printMatrix();
	return (errorCode == TABLEAU_SUCCESS);
}

bool simpl::pivot(int row, int col) {
	variableName tempVar;
	tableauErrorCode errorCode = tableau->pivot(row, col);
	if ((indepVar[col - 1]).indep && !((depVar[row - 1]).indep)) {
		(indepVar[col - 1]).indep = false;
		(depVar[row - 1]).indep = true;
	} else if (!((indepVar[col - 1]).indep) && (depVar[row - 1]).indep) {
		(indepVar[col - 1]).indep = true;
		(depVar[row - 1]).indep = false;
	}
	tempVar.number = (indepVar[col - 1]).number;
	(indepVar[col - 1]).number = (depVar[row - 1]).number;
	(depVar[row - 1]).number = tempVar.number;
	return (errorCode == TABLEAU_SUCCESS);
}

int simpl::simplex(float* xValuePtr, float* optimalValue) {
	bool simplexdone = false;
	int i, j;
	int colnum;
	int rownum;
	if (xValuePtr == NULL || optimalValue == NULL) {
		return 3;
	}
	//This first loop is to check for feasibility (are there any possible solutions?)
	while (!simplexdone) {
		checkValue feasibilityCheck = tableau->feasibleSolutionsCheck();
		if (feasibilityCheck.first)
		{
			if (feasibilityCheck.second == std::make_pair(-1, -1)) //there are feasible solutions
			{
				break;
			}
		}
		else
		{
			return 1; //no feasible solution
		}
		tableauErrorCode errorCode = tableau->pivotFeasibility(feasibilityCheck);
		if (errorCode != TABLEAU_SUCCESS)
		{
			return 4;
		}
	}
	simplexdone = false;
	//This second loop is to actually solve the problem
	while (!simplexdone) {
		simplexdone = true;
		checkValue optimalCheck = tableau->optimalSolutionCheck();
		simplexdone = optimalCheck.first;
		colnum = (optimalCheck.second).second;
		if (simplexdone) {
			//an optimal solution has been found
			*optimalValue = tableau->getOptimalValue();
			for (i = 0; i < numvariable; i++) {
				xValuePtr[i] = -1;
			}
			for (i = 0; i < numvariable; i++) {
				if ((indepVar[i]).indep) {
					j = (indepVar[i]).number;
					xValuePtr[j - 1] = 0;
				}
			}
			for (i = 0; i < numconstraint; i++) {
				if ((depVar[i]).indep) {
					j = (depVar[i]).number;
					xValuePtr[j - 1] = tableau->getVariableValue(i);
				}
			}
			for (i = 0; i < numvariable; i++) {
				if (xValuePtr[i] < -0.5) {
					return 5; //some logic error caused a variable to be negative
				}
			}
			return 0; //success
		}
		simplexdone = true;
		checkValue unboundedCheck = tableau->unboundedSolutionCheck(colnum);
		rownum = (unboundedCheck.second).first;
		simplexdone = unboundedCheck.first;
		if (simplexdone) {
			return 2; //problem is unbounded
		}
		if (!(pivot((rownum+1), (colnum+1)))) {
			return 4; //pivot failed for some reason (presumed to be a logic bug)
		}
	}
	return 0;
}
//private methods

void simpl::constructTab(const int numvar, const int numconstr) {
	int i;
	if (numvar <= 0 || numconstr <= 0) {
		tableau = NULL;
		indepVar = NULL;
		depVar = NULL;
		numvariable = 0;
		numconstraint = 0;
		canchangevalue = false;
		return;
	}

	numvariable = numvar;
	numconstraint = numconstr;
	indepVar = new variableName[numvar];
	depVar = new variableName[numconstr];
	tableau = new simplexTableau(numvar, numconstr);
	for (i = 0; i < numvar; i++) {
		(indepVar[i]).indep = true;
		(indepVar[i]).number = (i + 1);
	}
	for (i = 0; i < numconstr; i++) {
		(depVar[i]).indep = false;
		(depVar[i]).number = (i + 1);
	}
	canchangevalue = true;
}

void simpl::destroyTab() {
	if (numvariable == 0 && numconstraint == 0) {
		return;
	}
	delete tableau;
	tableau = NULL;
	delete[]indepVar;
	indepVar = NULL;
	delete[]depVar;
	depVar = NULL;
}



