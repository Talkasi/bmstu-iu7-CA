#include "io.hpp"
#include <iostream>

std::ifstream open_file(std::string path)
{
    std::ifstream file(path, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "ERROR: Can't open input file.\n";
        exit(1);
    }

    return file;
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
    printf("┌──────────┬──────────┬──────────┬──────────┐\n");
    printf("│     X    │    Y     │    Y'    │    Y''   │\n");
    printf("├──────────┼──────────┼──────────┼──────────┤\n");
    for (size_t i = 0; i < table.size(); ++i)
        printf("│ %6.6lf │ %6.6lf │ %6.6lf │ %6.6lf │\n", table[i][X], table[i][Y], table[i][FD], table[i][SD]);
    printf("└──────────┴──────────┴──────────┴──────────┘\n");
}
