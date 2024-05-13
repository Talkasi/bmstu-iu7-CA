#include "newton.hpp"
#include "io.hpp"
#include <cassert>

diff_table_row_t first_divided_difference(table_t &table)
{
    diff_table_row_t res;
    for (int i = 0; i < table.size(); ++i)
        res.push_back(table[i][Y]);

    return res;
}

diff_table_t newton_divided_difference(table_t &table)
{
    diff_table_t diff_table = {first_divided_difference(table)};

    for (int i = 1; i < table.size(); ++i) {
        diff_table_row_t tmp;
        for (int j = 0; j < table.size() - i; ++j)
            tmp.push_back((diff_table[i - 1][j + 1] - diff_table[i - 1][j]) / (table[i + j][X] - table[j][X]));

        diff_table.push_back(tmp);
    }

    return diff_table;
}

double newton(table_t &table, int n, double x, bool verbose)
{
    table_t new_table = get_nearest(table, n + 1, x);

    if (verbose) {
        printf("Part of the table to work with:\n");
        print_table(new_table);
    }

    diff_table_t diff_table = newton_divided_difference(new_table);

    double res = diff_table[0][0];
    double m = 1;

    for (int i = 1; i <= n; ++i) {
        m *= (x - new_table[i - 1][X]);
        res += m * diff_table[i][0];
    }

    return res;
}

double newton_second_derivative(table_t &table, double x)
{
    table_t new_table = get_nearest(table, 4, x);
    diff_table_t diff_table = newton_divided_difference(new_table);
    return 2 * diff_table[2][0] +
           diff_table[3][0] * (6 * x - 2 * (new_table[0][X] + new_table[1][X] + new_table[2][X]));
}

double newton_by_2(table_3d_t &table, int nx, int ny, double x, double y, int z_i)
{
    table_t near_y_table = get_nearest_y(table, ny + 1, y);

    table_t zy;
    table_row_t tmp_row = {};
    for (int y_i = 0; y_i < near_y_table.size(); ++y_i) {
        table_t tmp = get_zx_from_y(table, y_i, z_i);
        tmp_row[0] = near_y_table[y_i][0];
        tmp_row[1] = newton(tmp, nx, x, false);
        zy.push_back(tmp_row);
    }

    return newton(zy, ny, y, false);
}

double newton_by_3(table_3d_t &table, int nx, int ny, int nz, double x, double y, double z)
{
    table_t near_z_table = get_nearest_z(table, ny + 1, y);

    table_t fz;
    table_row_t tmp_row = {};
    for (int z_i = 0; z_i < near_z_table.size(); ++z_i) {
        tmp_row[0] = near_z_table[z_i][0];
        tmp_row[1] = newton_by_2(table, nx, ny, x, y, z_i);
        fz.push_back(tmp_row);
    }

    return newton(fz, nz, z, false);
}
