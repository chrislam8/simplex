#include <iostream>
#include <string>
#include "simpl.h"

using namespace std;

int main () {
	simpl *test = new simpl;
	bool isErr = false;
	if (!(test->changeValue(10.2,1,1))) {
		cout << "Error: changeValue returned false when it should return true" << endl;
		isErr = true;
	}
	if (test->changeValue(10.2,10,1)) {
		cout << "Error: changeValue returned true when it should return false" << endl;
		isErr = true;
	}
	if (test->changeValue(10.2,0,1)) {
		cout << "Error: changeValue returned true when it should return false" << endl;
		isErr = true;
	}
	if (!isErr) {
		cout << "All tests successfully passed" << endl;
	}
	delete test;
	cout << "All good\n";
	return 0;
}
