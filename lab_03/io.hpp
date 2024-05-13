#ifndef IO_HPP
#define IO_HPP
#include "table.h"
#include <fstream>

std::ifstream open_file(std::string path);
table_3d_t fread_table_3d(std::ifstream &f);
void print_table_3d(table_3d_t &table);

table_t fread_table(std::ifstream &f);
void print_table(table_t &table);

table_t get_nearest(table_t &table, int n, double x);
table_t get_nearest_y(table_3d_t &table, int n, double y);
table_t get_nearest_z(table_3d_t &table, int n, double z);
table_t get_zx_from_y(table_3d_t &table, int y_i, int z_i);

#endif // IO_HPP