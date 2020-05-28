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
	if (canChangeValue)
	{
		tableauErrorCode errorCode = tableau->changeValue(value, row, col);
		return (errorCode == TABLEAU_SUCCESS);
	}
	else
	{
		return false;
	}
}

bool simpl::prTableau() {
	tableauErrorCode errorCode = tableau->printMatrix();
	return (errorCode == TABLEAU_SUCCESS);
}

bool simpl::pivot(int row, int col) {
	tableauErrorCode errorCode = tableau->pivot(row, col);
	return (errorCode == TABLEAU_SUCCESS);
}

simplexErrorCode simpl::simplex(float* xValuePtr, float* optimalValue) {
	bool simplexdone = false;
	int i;
	int colnum;
	int rownum;
	if (xValuePtr == NULL || optimalValue == NULL) {
		return NULL_POINTER_ENCOUNTERED;
	}
	canChangeValue = false;
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
			return NO_FEASIBLE_SOLUTION; //no feasible solution
		}
		tableauErrorCode errorCode = tableau->pivotFeasibility(feasibilityCheck);
		if (errorCode != TABLEAU_SUCCESS)
		{
			return PIVOT_ERROR;
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
			variableNumValue variableNum;
			for (i = 0; i < numvariable; i++) {
				variableNum = tableau->getIndepVariableNum(i);
				if (variableNum.second == TABLEAU_SUCCESS)
				{
					xValuePtr[variableNum.first - 1] = 0;
				}
			}
			for (i = 0; i < numconstraint; i++) {
				variableNum = tableau->getDepVariableNum(i);
				if (variableNum.second == TABLEAU_SUCCESS)
				{
					xValuePtr[variableNum.first - 1] = tableau->getVariableValue(i);
				}
			}
			for (i = 0; i < numvariable; i++) {
				if (xValuePtr[i] < -0.5) {
					return ALGORITHM_ERROR; //some logic error caused a variable to be negative
				}
			}
			return SIMPLEX_SUCCESS; //success
		}
		simplexdone = true;
		checkValue unboundedCheck = tableau->unboundedSolutionCheck(colnum);
		rownum = (unboundedCheck.second).first;
		simplexdone = unboundedCheck.first;
		if (simplexdone) {
			return UNBOUNDED_PROBLEM; //problem is unbounded
		}
		if (!(pivot((rownum+1), (colnum+1)))) {
			return PIVOT_ERROR; //pivot failed for some reason (presumed to be a logic bug)
		}
	}
	return ALGORITHM_ERROR;
}
//private methods

void simpl::constructTab(const int numvar, const int numconstr) {

	if (numvar <= 0 || numconstr <= 0) {
		tableau = NULL;
		numvariable = 0;
		numconstraint = 0;
		canChangeValue = false;
		return;
	}

	numvariable = numvar;
	numconstraint = numconstr;
	tableau = new simplexTableau(numvar, numconstr);
	canChangeValue = true;
}

void simpl::destroyTab() {
	if (numvariable == 0 && numconstraint == 0) {
		return;
	}
	delete tableau;
	tableau = NULL;
}



