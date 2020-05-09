#pragma once

#include <utility>

enum matrixErrorCode {
	SUCCESS,
	INVALID_COLUMN_NUMBER,
	INVALID_ROW_NUMBER,
	PIVOT_VALUE_CANNOT_BE_ZERO
};

#define matrixLocation std::pair<int, int>
#define feasibilityCheckValue std::pair<bool, matrixLocation>