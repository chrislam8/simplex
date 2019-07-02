#include <iostream>
#include <string>

using namespace std;

struct variableName {
	bool indep;
	int number;
};

class simpl {
public:
	simpl(); //basic constructor
	simpl(int numvar, int numconstr); //custom constructor
	~simpl(); //destructor
	bool changeValue(float value, int row, int col);
	//This method is intended for users to be able to see the tableau (mainly for debugging purposes)
	bool prTableau();	
	bool pivot(int row, int col);
	//This method is not yet fully tested
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
	variableName* indepVar;
	variableName* depVar;
	int numconstraint; //also numrows - 1
	int numvariable; //also numcols - 1
	bool canchangevalue;
	//private methods
	void constructTab(const int numvar,const int numconstr);
	void destroyTab();
};

