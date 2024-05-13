#include "io.hpp"
#include <cassert>
#include <iostream>

#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

std::ifstream open_file(std::string path)
{
    std::ifstream file(path, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "ERROR: Can't open input file.\n";
        exit(1);
    }

    return file;
}

table_3d_t fread_table_3d(std::ifstream &f)
{
    table_3d_t res_table;
    int nx;
    int ny;
    int nz;

    f >> nx;
    f >> ny;
    f >> nz;

    res_table.x.resize(nx);
    res_table.y.resize(ny);
    res_table.z.resize(nz);

    for (int i = 0; i < nx; ++i)
        f >> res_table.x[i];

    for (int i = 0; i < ny; ++i)
        f >> res_table.y[i];

    for (int i = 0; i < nz; ++i)
        f >> res_table.z[i];

    res_table.data.resize(nz);
    for (;;) {
        for (size_t i_z = 0; i_z < nz; ++i_z) {
            res_table.data[i_z].resize(ny);

            for (size_t i_y = 0; i_y < ny; ++i_y) {
                res_table.data[i_z][i_y].resize(nx);

                for (size_t i_x = 0; i_x < nz; ++i_x) {
                    if (!(f >> res_table.data[i_z][i_y][i_x]))
                        return res_table;
                }
            }
        }
    }
}

void print_start(size_t n)
{
    printf("┌");
    for (size_t i = 0; i < n; ++i)
        printf("─────────────┬");
    printf("─────────────┐\n");
}

void print_middle(size_t n)
{
    printf("├");
    for (size_t i = 0; i < n; ++i)
        printf("─────────────┼");
    printf("─────────────┤\n");
}

void print_end(size_t n)
{
    printf("└");
    for (size_t i = 0; i < n; ++i)
        printf("─────────────┴");
    printf("─────────────┘\n");
}

void print_title(std::vector<double> x)
{
    print_start(x.size());

    printf("│" GREEN "     Y\\X     " NC);
    for (size_t i = 0; i < x.size(); ++i)
        printf("│" GREEN " %10lf  " NC, x[i]);
    printf("│ \n" NC);

    print_middle(x.size());
}

void print_table_3d(table_3d_t &table)
{
    for (size_t i_z = 0; i_z < table.z.size(); ++i_z) {
        printf("Z = " GREEN "%lf\n" NC, table.z[i_z]);
        print_title(table.x);

        for (size_t i_y = 0; i_y < table.y.size(); ++i_y) {
            printf("│" GREEN " %11lf " NC "│", table.y[i_y]);
            for (size_t i_x = 0; i_x < table.x.size(); ++i_x)
                printf(" %11.6lf │", table.data[i_z][i_y][i_x]);
            printf("\n");
        }
        print_end(table.x.size());
    }
}

table_t fread_table(std::ifstream &f)
{
    table_t res_table;
    table_row_t tmp = {};

    size_t n_elems = 0;
    f >> n_elems;

    for (;;) {
        for (size_t i = 0; i < n_elems; ++i)
            if (!(f >> tmp[i]))
                return res_table;

        res_table.push_back(tmp);
    }
}

void print_table(table_t &table)
{
    printf("┌───────────────────┬───────────────────┐\n");
    printf("│         X         │         Y         │\n");
    printf("├───────────────────┼───────────────────┤\n");
    for (size_t i = 0; i < table.size(); ++i)
        printf("│ %17.6g │ %17.6g │\n", table[i][X], table[i][Y]);
    printf("└───────────────────┴───────────────────┘\n");
}

table_t get_nearest_y(table_3d_t &table, int n, double y)
{
    table_t tmp_table;
    table_row_t tmp_row = {};
    for (int i = 0; i < table.y.size(); ++i) {
        tmp_row[X] = table.y[i];
        tmp_row[Y] = i;
        tmp_table.push_back(tmp_row);
    }

    return get_nearest(tmp_table, n, y);
}

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

table_t get_nearest_z(table_3d_t &table, int n, double z)
{
    table_t tmp_table;
    table_row_t tmp_row = {};
    for (int i = 0; i < table.z.size(); ++i) {
        tmp_row[0] = table.z[i];
        tmp_row[1] = i;
        tmp_table.push_back(tmp_row);
    }

    return get_nearest(tmp_table, n, z);
}

table_t get_zx_from_y(table_3d_t &table, int y_i, int z_i)
{
    assert(table.z.size() == table.x.size());
    table_t slice;
    table_row_t tmp_row = {};

    for (int i = 0; i < table.x.size(); ++i) {
        tmp_row[0] = table.x[i];
        tmp_row[1] = table.data[z_i][y_i][i];
        slice.push_back(tmp_row);
    }

    return slice;
}