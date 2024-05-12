#ifndef IO_HPP
#define IO_HPP
#include "table.h"
#include <fstream>

std::ifstream open_file(std::string path);
table_3d_t fread_table_3d(std::ifstream &f);
void print_table_3d(table_3d_t &table);

table_t fread_table(std::ifstream &f);
void print_table(table_t &table);

#endif // IO_HPP