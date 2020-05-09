#pragma once

class simplexMatrix {
public:
	simplexMatrix();
	simplexMatrix(int numVar, int numConstr);
	~simplexMatrix();

private:
	float** valueMatrix;
	void constructMatrix(int numVar, int numConstr);

	int numberOfVariables;
	int numberOfConstraints;
};