#ifndef TABLE_H
#define TABLE_H
#include <array>
#include <vector>

enum table_row_elems
{
    X,
    Y,
    N_ELEMS
};

using table_row_t = std::array<double, N_ELEMS>;
using table_t = std::vector<table_row_t>;

struct table_3d_t
{
    std::vector<std::vector<std::vector<double>>> data;

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
};

#endif // TABLE_H