#include "simplexMatrix.h"

simplexMatrix::simplexMatrix() {
	constructMatrix(2, 1);
}

simplexMatrix::simplexMatrix(int numVar, int numConstr) {
	constructMatrix(numVar, numConstr);
}

simplexMatrix::~simplexMatrix() {
	for (int rowNum = 0; rowNum <= numberOfConstraints; rowNum++) {
		delete valueMatrix[rowNum];
		valueMatrix[rowNum] = nullptr;
	}

}

void simplexMatrix::constructMatrix(int numVar, int numConstr) {
	valueMatrix = new float* [numConstr + 1];
	for (int rowNum = 0; rowNum <= numConstr; rowNum++) {
		valueMatrix[rowNum] = new float[numVar + 1];
	}
	
	numberOfVariables = numVar;
	numberOfConstraints = numConstr;
}