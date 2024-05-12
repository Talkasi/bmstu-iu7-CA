#include "newton.hpp"
#include "io.hpp"

table_t get_nearest(table_t &table, int n, double x)
{
    table_t res = {};
    int middle_i = table.size();

    for (int i = 0; i < table.size(); ++i) {
        if (table[i][X] > x) {
            middle_i = i;
            break;
        }
    }

    int li = middle_i - n / 2;
    int ri = li + n - 1;

    if (li < 0) {
        li = 0;
        ri = n - 1;
    }
    else if (ri >= table.size()) {
        ri = table.size() - 1;
        li = ri - n + 1;
    }

    for (; li <= ri; ++li)
        res.push_back(table[li]);

    return res;
}

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
