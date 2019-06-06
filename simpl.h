#include <iostream>
#include <string>

using namespace std;

class simpl {
public:
	simpl(); //basic constructor
	simpl(int numvar, int numconstr); //custom constructor
	~simpl(); //destructor
	bool changeValue(float value, int row, int col);
	//This method is intended for users to be able to see the tableau (mainly for debugging purposes)
	void returnTableau(float** tableauLoc);
	//These methods are not yet coded
	bool pivot(int row, int col);
	int simplex(float* xValuePtr, float* optimalValue); 
	/* xValuePtr is meant as a pointer to return the optimal x values for the problem
	optimalValue is meant as a pointer to where the optimal value will be placed 
	return values:
		0 - successful
		1 - no feasible solution
		2 - unbounded problem (no optimal solution)
		other - error in algorithm
	*/
private:
	float** tableau;
	string* indepVar;
	string* depVar;
	int numconstraint; //also numrows - 1
	int numvariable; //also numcols - 1
	bool canchangevalue;
	//private methods
	void constructTab(const int numvar,const int numconstr);
	void destroyTab();
};

//public methods

simpl::simpl() {
	constructTab(2,1);	
}

simpl::simpl(int numvar, int numconstr) {
	if (numvar > 9) {
		cout << "WARNING: Too many variables, changing to 9" << endl;
		numvar = 9;
	}
	if (numconstr > 9) {
		cout << "WARNING: Too many constraints, changing to 9" << endl;
		numconstr = 9;
	}
	constructTab(numvar,numconstr);
}

simpl::~simpl() {
	destroyTab();
}

bool simpl::changeValue (float value, int row, int col) {
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

void simpl::returnTableau(float** tableauLoc) {
	int i,j;
	for (i = 0; i <= numconstraint; i++) {
		for (j = 0; j <= numvariable; j++) {
			tableauLoc[i][j] = tableau[i][j];
		}
	}
}

bool simpl::pivot(int row, int col) {
	int i,j;
	float pivotval;
	float rowval;
	float colval;
	float slotval;
	float newval;
	if (row <= 0 || row > numconstraint || col <= 0 || col > numvariable) {
		return false;
	}
	pivotval = tableau[row-1][col-1];
	for (i = 0; i <= numconstraint; i++) {
		for (j = 0; j <= numvariable; j++) {
			if (i != (row-1) && j != (col-1)) {
				rowval = tableau[row-1][j];
				colval = tableau[i][col-1];
				slotval = tableau[i][j];
				newval = (slotval*pivotval - rowval*colval)/pivotval;
				tableau[i][j] = newval;
			}
		}
	}
	for (i = 0; i <= numconstraint; i++) {
		if (i != (row-1)) {
			slotval = tableau[i][col-1];
			newval = -1*slotval/pivotval;
			tableau[i][col-1] = newval;
		}
	}
	for (j = 0; j <= numvariable; j++) {
		if (j != (col-1)) {
			slotval = tableau[row-1][j];
			newval = slotval/pivotval;
			tableau[row-1][j] = newval;
		}
	}
	newval = 1/pivotval;
	tableau[row-1][col-1] = newval;
	return true;
}

int simpl::simplex(float* xValuePtr, float* optimalValue) {
	bool simplexdone = false;
	int i,j;
	int colnum;
	int rownum;
	float minval;
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
			break;
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
		if (rownum < (numconstraint-1)) {
			minval = tableau[rownum][colnum];
			for (i = rownum; i < numconstraint; i++) {
				if (tableau[i][colnum] < minval) {
					minval = tableau[i][colnum];
					rownum = i;
				}
			}
		}
		if (!(pivot((rownum+1),(colnum+1)))) {
			return 4;
		}
	}
	simplexdone = false;
	//This second loop is to actually solve the problem
	while (!simplexdone) {
		simplexdone = true;
	}
	return 0;
}
//private methods

void simpl::constructTab(const int numvar,const int numconstr) {
	int i;
	char numArr[9] = {'1','2','3','4','5','6','7','8','9'};
	string startIndep = "x";
	string startDep = "t";
	string tempStr;
	numvariable = numvar;
	numconstraint = numconstr;
	indepVar = new string[numvar];
	depVar = new string[numconstr];
	tableau = new float*[numconstr+1];
	for (i = 0; i <= numconstr; i++) {
		tableau[i] = new float[numvar+1];
	}
	for (i = 0; i < numvar; i++) {
		tempStr = startIndep;
		tempStr += numArr[i];
		indepVar[i] = tempStr;
	}
	for (i = 0; i < numconstr; i++) {
		tempStr = startDep;
		tempStr += numArr[i];
		depVar[i] = tempStr;
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
	delete []indepVar;
	indepVar = NULL;
	delete []depVar;
	depVar = NULL;
}



