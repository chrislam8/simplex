#include <iostream>
#include <string>

using namespace std;

class simpl {
public:
	simpl(); //basic constructor
	simpl(int numvar, int numconstr); //custom constructor
	~simpl(); //destructor
	bool changeValue(float value, int row, int col);
	void returnTableau(float** tableauLoc);
	bool pivot(int row, int col);
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
	return false;
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



