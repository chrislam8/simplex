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
	int actualrow;
	int actualcol;
	if (row > numconstraint + 1 || col > numvariable + 1 || row <= 0 || col <= 0) {
		return false;
	}
	if (!canchangevalue) {
		return false;
	}
	actualrow = row - 1;
	actualcol = col - 1;
	tableau[actualrow][actualcol] = value;
	return true;
}

bool simpl::prTableau() {
	int i, j;
	for (j = 0; j < numvariable; j++) {
		if (indepVar[j].indep) {
			cout << "x";
		}
		else {
			cout << "t";
		}
		cout << indepVar[j].number << "\t";
	}
	cout << endl;
	for (i = 0; i <= numconstraint; i++) {
		for (j = 0; j <= numvariable; j++) {
			printf("%.2f\t",tableau[i][j]);
			if (j == (numvariable - 1)) {
				cout << "|\t";
			}
		}
		
		if (i == (numconstraint - 1)) {
			cout << "=";
			if (depVar[i].indep) {
				cout << "x";
			}
			else {
				cout << "t";
			}
			cout << depVar[i].number;
			cout << endl;
			for (j = 0; j <= (numvariable+1); j++) {
				cout << "-----\t";
			}
			cout << endl;
			
		}
		else if (i < numconstraint) {
			cout << "=";
			if (depVar[i].indep) {
				cout << "x";
			}
			else {
				cout << "t";
			}
			cout << depVar[i].number;
			cout << endl;
		}
		else {
			cout << endl;
		}
		
	}
	cout << "Tableau printed" << endl << endl;
	return false;
}

bool simpl::pivot(int row, int col) {
	int i, j;
	float pivotval;
	float rowval;
	float colval;
	float slotval;
	float newval;
	variableName tempVar;
	if (row <= 0 || row > numconstraint || col <= 0 || col > numvariable) {
		return false;
	}
	pivotval = tableau[row - 1][col - 1];
	for (i = 0; i <= numconstraint; i++) {
		for (j = 0; j <= numvariable; j++) {
			if (i != (row - 1) && j != (col - 1)) {
				rowval = tableau[row - 1][j];
				colval = tableau[i][col - 1];
				slotval = tableau[i][j];
				newval = (slotval * pivotval - rowval * colval) / pivotval;
				tableau[i][j] = newval;
			}
		}
	}
	for (i = 0; i <= numconstraint; i++) {
		if (i != (row - 1)) {
			slotval = tableau[i][col - 1];
			newval = -1 * slotval / pivotval;
			tableau[i][col - 1] = newval;
		}
	}
	for (j = 0; j <= numvariable; j++) {
		if (j != (col - 1)) {
			slotval = tableau[row - 1][j];
			newval = slotval / pivotval;
			tableau[row - 1][j] = newval;
		}
	}
	newval = 1 / pivotval;
	tableau[row - 1][col - 1] = newval;
	if ((indepVar[col - 1]).indep && !((depVar[row - 1]).indep)) {
		cout << "changing indep variables" << endl;
		(indepVar[col - 1]).indep = false;
		(depVar[row - 1]).indep = true;
	} else if (!((indepVar[col - 1]).indep) && (depVar[row - 1]).indep) {
		cout << "changing indep variables 2" << endl;
		(indepVar[col - 1]).indep = true;
		(depVar[row - 1]).indep = false;
	}
	//tempVar.indep = (indepVar[col - 1]).indep;
	tempVar.number = (indepVar[col - 1]).number;
	//(indepVar[col - 1]).indep = (depVar[row - 1]).indep;
	(indepVar[col - 1]).number = (depVar[row - 1]).number;
	//(depVar[row - 1]).indep = tempVar.indep;
	(depVar[row - 1]).number = tempVar.number;
	canchangevalue = false;
	return true;
}

int simpl::simplex(float* xValuePtr, float* optimalValue) {
	bool simplexdone = false;
	int i, j;
	int colnum;
	int rownum;
	float minval;
	float tempval;
	if (xValuePtr == NULL || optimalValue == NULL) {
		return 3;
	}
	//This first loop is to check for feasibility (are there any possible solutions?)
	while (!simplexdone) {
		simplexdone = true;
		for (i = 0; i < numconstraint; i++) {
			if (tableau[i][numvariable] < 0) {
				simplexdone = false;
				rownum = i;
			}
		}
		if (simplexdone) {
			break; //there are feasible solutions
		}
		simplexdone = true;
		for (i = 0; i < numvariable; i++) {
			if (tableau[rownum][i] < 0) {
				simplexdone = false;
				colnum = i;
			}
		}
		if (simplexdone) {
			return 1; //no feasible solution
		}
		//determine where to pivot
		if (rownum < (numconstraint - 1)) {
			minval = tableau[rownum][colnum];
			for (i = rownum; i < numconstraint; i++) {
				if (tableau[i][colnum] < minval) {
					minval = tableau[i][colnum];
					rownum = i;
				}
			}
		}
		if (!(pivot((rownum + 1), (colnum + 1)))) {
			return 4; //pivot failed for some reason (presumed to be a logic bug)
		}
	}
	simplexdone = false;
	//This second loop is to actually solve the problem
	while (!simplexdone) {
		simplexdone = true;
		for (j = 0; j < numvariable; j++) {
			if (tableau[numconstraint][j] > 0) {
				simplexdone = false;
				colnum = j;
			}
		}
		if (simplexdone) {
			//an optimal solution has been found
			*optimalValue = -1*tableau[numconstraint][numvariable];
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
					xValuePtr[j - 1] = tableau[i][numvariable];
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
		minval = -1;
		for (i = 0; i < numconstraint; i++) {
			if (tableau[i][colnum] > 0) {
				simplexdone = false;
				tempval = tableau[i][numvariable] / tableau[i][colnum];
				if (minval < 0) {
					minval = tempval;
					rownum = i;
				}
				if (minval > tempval) {
					minval = tempval;
					rownum = i;
				}
			}
		}
		if (simplexdone) {
			return 2; //problem is unbounded
		}
		if (!(pivot((rownum + 1), (colnum + 1)))) {
			return 4; //pivot failed for some reason (presumed to be a logic bug)
		}
	}
	return 0;
}
//private methods

void simpl::constructTab(const int numvar, const int numconstr) {
	int i;

	numvariable = numvar;
	numconstraint = numconstr;
	indepVar = new variableName[numvar];
	depVar = new variableName[numconstr];
	tableau = new float* [numconstr + 1];
	for (i = 0; i <= numconstr; i++) {
		tableau[i] = new float[numvar + 1];
	}
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
	int i;
	for (i = 0; i <= numconstraint; i++) {
		delete tableau[i];
	}
	delete tableau;
	tableau = NULL;
	delete[]indepVar;
	indepVar = NULL;
	delete[]depVar;
	depVar = NULL;
}



