#pragma once

#include <utility>

enum tableauErrorCode {
	TABLEAU_SUCCESS,
	INVALID_COLUMN_NUMBER,
	INVALID_ROW_NUMBER,
	PIVOT_VALUE_CANNOT_BE_ZERO,
	FUNCTION_NOT_IMPLEMENTED
};

#define matrixLocation std::pair<int, int>
#define checkValue std::pair<bool, matrixLocation>