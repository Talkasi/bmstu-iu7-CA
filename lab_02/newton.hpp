#ifndef NEWTON_HPP
#define NEWTON_HPP
#include "table.h"
#include <vector>

using diff_table_row_t = std::vector<double>;
using diff_table_t = std::vector<diff_table_row_t>;

diff_table_row_t first_divided_difference(table_t &table);
diff_table_t newton_divided_difference(table_t &table);

double newton(table_t &table, int n, double x, bool verbose);
double newton_second_derivative(table_t &table, double x);

#endif // NEWTON_HPP