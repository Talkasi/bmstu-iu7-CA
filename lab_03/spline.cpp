#include "spline.hpp"
#include "io.hpp"
#include <cassert>

#define CYAN "\033[0;36m"
#define NC "\033[0m"

double spline(table_t &table, double x, double s_border, double e_border, bool verbose)
{
    spline_t data;

    data.table = table;

    init_spline_coef_a(data);
    assert(table.size() == data.a.size());

    init_spline_coef_c_gauss(data, s_border, e_border, verbose);
    assert(table.size() == data.c.size());

    init_spline_coef_b(data, e_border);
    assert(table.size() == data.b.size());

    init_spline_coef_d(data, e_border);
    assert(table.size() == data.d.size());

    if (verbose)
        print_coeffs(data);

    return spline_approximate(data, x);
}

void init_spline_coef_a(spline_t &data)
{
    data.a.clear();

    /* a_i = y_{i - 1} 		1 <= i <= N */
    for (int i = 0; i < data.table.size(); ++i)
        data.a.push_back(data.table[i][Y]);
}

void init_spline_coef_b(spline_t &data, double e_border)
{
    data.b.clear();
    /* b_i = (y_i - y_{i - 1}) / h_i - h_i * (c_{i + 1} + 2 * c_i) / 3 		1 <= i <= N - 1*/
    /* b_N = (y_N - y_{N - 1}) / h_N - h_N * 2 * c_N / 3 */
    double h_i;
    int i = 1;
    for (; i <= data.table.size() - 1; ++i) {
        h_i = data.table[i][X] - data.table[i - 1][X];
        data.b.push_back((data.table[i][Y] - data.table[i - 1][Y]) / h_i - h_i * (data.c[i] + 2 * data.c[i - 1]) / 3);
    }

    h_i = data.table[i][X] - data.table[i - 1][X];
    data.b.push_back((data.table[i][Y] - data.table[i - 1][Y]) / h_i - h_i * 2 * data.c[i] * (e_border / 2) / 3);
}

void init_spline_coef_c_gauss(spline_t &data, double s_border, double e_border, bool verbose)
{
    data.c.clear();

    /* c_1 = 0 */
    /* h_{i - 1} * c_{i - 1} + 2 * (h_{i - 1} + h_i) * c_i + h_i * c_{i + 1} =
     * 3 * ((y_i - y_{i - 1}) / h_i - (y_{i - 1} - y_{i - 2}) / h_{i - 1}), 	2 <= i <= N - 1 */
    /* c_{N + 1} = 0 */

    std::vector<double> rside(data.table.size());
    std::vector<std::vector<double>> lside(data.table.size());
    for (int i = 0; i < lside.size(); ++i)
        lside[i].resize(data.table.size());

    init_lside_matrix(lside, data.table);
    init_rside_matrix(rside, data.table, s_border, e_border);

    if (verbose)
        print_matrix(lside, rside);

    data.c = gauss_elimination(lside, rside);
}

void init_spline_coef_d(spline_t &data, double e_border)
{
    data.d.clear();
    /* d_i = (c_{i + 1} - c_i) / (3 * h_i)   1 <= i <= N - 1 */
    /* d_N = - c_N / (3 * h_N) */

    int i = 1;
    for (; i <= data.table.size() - 1; ++i)
        data.d.push_back((data.c[i] - data.c[i - 1]) / (3 * (data.table[i][X] - data.table[i - 1][X])));

    data.d.push_back((e_border / 2 - data.c[i]) / (3 * (data.table[i][X] - data.table[i - 1][X])));
}

double spline_approximate(spline_t &data, double x)
{
    int i = 0;
    while (i < data.table.size() - 2 && x > data.table[i + 1][X])
        ++i;

    double x_i = data.table[i][X];
    double h_i = x - x_i;
    double h_i_2 = h_i * h_i;
    double h_i_3 = h_i * h_i * h_i;

    return data.a[i] + data.b[i] * h_i + data.c[i] * h_i_2 + data.d[i] * h_i_3;
}

std::vector<double> gauss_elimination(std::vector<std::vector<double>> &lside, std::vector<double> rside)
{
    int n = lside.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double ratio = lside[j][i] / lside[i][i];
            for (int k = i; k < n; ++k)
                lside[j][k] -= ratio * lside[i][k];

            rside[j] -= ratio * rside[i];
        }
    }

    std::vector<double> res(n);
    for (int i = n - 1; i >= 0; --i) {
        res[i] = rside[i];
        for (int j = i + 1; j < n; ++j) {
            res[i] -= lside[i][j] * res[j];
        }
        res[i] /= lside[i][i];
    }

    return res;
}

void init_lside_matrix(std::vector<std::vector<double>> &lside, const table_t &table)
{
    double h_i_prev;
    double h_i;
    lside[0][0] = 1;

    /* c_1 = 0 */
    /* h_{i - 1} * c_{i - 1} + 2 * (h_{i - 1} + h_i) * c_i + h_i * c_{i + 1} =
     * 3 * ((y_i - y_{i - 1}) / h_i - (y_{i - 1} - y_{i - 2}) / h_{i - 1}), 	2 <= i <= N - 1 */
    /* c_{N + 1} = 0 */

    int i = 2;
    for (; i < table.size(); ++i) {
        h_i_prev = table[i - 1][X] - table[i - 2][X];
        h_i = table[i][X] - table[i - 1][X];

        lside[i - 1][i - 2] = h_i_prev;
        lside[i - 1][i - 1] = 2 * (h_i_prev + h_i);
        lside[i - 1][i] = h_i;
    }

    lside[i - 1][i - 1] = 1;
}

void init_rside_matrix(std::vector<double> &rside, const table_t &table, double s_border, double e_border)
{
    double h_i_prev;
    double h_i;

    rside[0] = s_border / 2;
    rside[table.size() - 1] = e_border / 2;

    for (int i = 2; i < table.size(); ++i) {
        h_i_prev = table[i - 1][X] - table[i - 2][X];
        h_i = table[i][X] - table[i - 1][X];
        rside[i - 1] = 3 * ((table[i][Y] - table[i - 1][Y]) / h_i - (table[i - 1][Y] - table[i - 2][Y]) / h_i_prev);
    }
}

void print_matrix(std::vector<std::vector<double>> &lside, std::vector<double> &rside)
{
    for (int i = 0; i < lside.size(); ++i) {
        for (int j = 0; j < lside[i].size(); ++j) {
            if (abs(i - j) > 1)
                printf("%12.6lf  ", lside[i][j]);
            else
                printf(CYAN "%12.6lf  " NC, lside[i][j]);
        }
        printf("│ %12.6lf\n", rside[i]);
    }
    printf("\n");
}

void print_coeffs(spline_t &data)
{
    printf("┌───────────────────┬───────────────────┬───────────────────┬───────────────────┐\n");
    printf("│         A         │         B         │         C         │         D         │\n");
    printf("├───────────────────┼───────────────────┼───────────────────┼───────────────────┤\n");
    for (size_t i = 0; i < data.table.size() - 1; ++i)
        printf("│ %17.6g │ %17.6g │ %17.6g │ %17.6g │\n", data.a[i], data.b[i], data.c[i], data.d[i]);
    printf("└───────────────────┴───────────────────┴───────────────────┴───────────────────┘\n");
}

double spline_by_2(table_3d_t &table, double x, double y, int z_i)
{
    table_t zx;
    table_row_t tmp_row = {};

    table_t tmp;
    for (int y_i = 0; y_i < table.y.size(); ++y_i) {
        tmp = get_zx_from_y(table, y_i, z_i);
        tmp_row[0] = table.y[y_i];
        tmp_row[1] = spline(tmp, x, 0, 0, false);
        zx.push_back(tmp_row);
    }

    return spline(zx, y, 0, 0, false);
}

double spline_by_3(table_3d_t &table, double x, double y, double z)
{
    table_t fz;
    table_row_t tmp_row = {};

    for (int z_i = 0; z_i < table.z.size(); ++z_i) {
        tmp_row[0] = table.z[z_i];
        tmp_row[1] = spline_by_2(table, x, y, z_i);
        fz.push_back(tmp_row);
    }

    return spline(fz, z, 0, 0, false);
}
