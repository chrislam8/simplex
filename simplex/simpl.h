#include <iostream>
#include <string>

#include "simplexTableau.h"
#include "simplexEnums.h"

using namespace std;

class simpl {
public:
	simpl(); //basic constructor
	simpl(int numvar, int numconstr); //custom constructor
	~simpl(); //destructor
	bool changeValue(float value, int row, int col);
	simplexErrorCode simplex(float* xValuePtr, float* optimalValue); 
	/* 
	xValuePtr is meant as a pointer to return the optimal x values for the problem
	optimalValue is meant as a pointer to where the optimal value will be placed 
	*/
private:
	simplexTableau* tableau;
	int numconstraint; //also numrows - 1
	int numvariable; //also numcols - 1
	bool canChangeValue;
	//private methods
	void constructTab(const int numvar,const int numconstr);
	void destroyTab();
	bool prTableau();	
	bool pivot(int row, int col);
	
};

