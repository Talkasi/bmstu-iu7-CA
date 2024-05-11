#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include <array>

enum table_row_elems {
	X,
	Y,
	FD,
	SD,
	N_ELEMS
};

using table_row_t = std::array<double, N_ELEMS>;
using table_t = std::vector<table_row_t>;

#endif // TABLE_H