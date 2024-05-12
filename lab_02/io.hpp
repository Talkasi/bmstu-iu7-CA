#ifndef IO_HPP
#define IO_HPP
#include "table.h"
#include <fstream>

std::ifstream open_file(std::string path);
table_t fread_table(std::ifstream &f);
void print_table(table_t &table);

#endif // IO_HPP