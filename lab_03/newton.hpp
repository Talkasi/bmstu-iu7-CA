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

double newton_by_2(table_3d_t &table, int nx, int ny, double x, double y, int z_i);
double newton_by_3(table_3d_t &table, int nx, int ny, int nz, double x, double y, double z);

#endif // NEWTON_HPP