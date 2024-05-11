#ifndef SPLINE_HPP
#define SPLINE_HPP
#include <vector>
#include "table.h"

struct spline_t {
	table_t table;
	std::vector<double> a;
	std::vector<double> b;
	std::vector<double> c;
	std::vector<double> d;
};

double spline(table_t &talbe, double x);

void init_spline_coef_a(spline_t &data);
void init_spline_coef_b(spline_t &data);
void init_spline_coef_c(spline_t &data);
void init_spline_coef_d(spline_t &data);

double spline_approximate(spline_t &data, double x);

void print_coeffs(spline_t &data);

void init_spline_coef_c_gauss(spline_t &data);
void init_lside_matrix(std::vector<std::vector<double>> &lside, const table_t &table);
void init_rside_matrix(std::vector<double> &rside, const table_t &table);
void print_matrix(std::vector<std::vector<double>> &lside, std::vector<double> &rside);
std::vector<double> gauss_elimination(std::vector<std::vector<double>> &lside, std::vector<double> rside);

#endif // SPLINE_HPP
