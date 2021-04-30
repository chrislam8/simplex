#include "simplexTableau.h"
#include "simplexEnums.h"

using namespace std;

class simpl {
public:
	simpl(); //basic constructor
	simpl(int numvar, int numconstr, bool miniTableau); //custom constructor
	simpl(const simpl& copy); //copy constructor
	~simpl(); //destructor
	simpl& operator=(const simpl& copy);
	bool changeValue(double value, int row, int col);
	simplexErrorCode simplex(double* xValuePtr, double* optimalValue); 
	/* 
	xValuePtr is meant as a pointer to return the optimal x values for the problem
	optimalValue is meant as a pointer to where the optimal value will be placed 
	*/
	bool exportTableau(std::string fileName);
	bool importTableau(std::string fileName, bool minTableau);
	int getNumberOfVariables();
	//methods for testing
	bool negativeTranspose();
private:
	simplexTableau* tableau;
	int numconstraint; //also numrows - 1
	int numvariable; //also numcols - 1
	bool canChangeValue;
	//private methods
	void constructTab(const int numvar,const int numconstr,const bool miniTableau);
	void destroyTab();
	void copyTab(const simpl& copy);
	bool pivot(int row, int col);
	
};

