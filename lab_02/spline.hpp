#ifndef SPLINE_HPP
#define SPLINE_HPP
#include "table.h"
#include <vector>

struct spline_t
{
    table_t table;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> d;
};

double spline(table_t &table, double x, double s_border, double e_border, bool verbose);

double get_ksi_next(spline_t &data, int i, double ksi);
double get_eta_next(spline_t &data, int i, double ksi, double eta);

void init_spline_coef_a(spline_t &data);
void init_spline_coef_b(spline_t &data, double e_border);
void init_spline_coef_c(spline_t &data, double s_border, double e_border, bool verbose);
void init_spline_coef_d(spline_t &data, double e_border);

double spline_approximate(spline_t &data, double x);

void init_spline_coef_c_gauss(spline_t &data, double s_border, double e_border, bool verbose);

void init_lside_matrix(std::vector<std::vector<double>> &lside, const table_t &table);
void init_rside_matrix(std::vector<double> &rside, const table_t &table, double s_border, double e_border);

std::vector<double> gauss_elimination(std::vector<std::vector<double>> &lside, std::vector<double> rside);

void print_coeffs(spline_t &data);
void print_matrix(std::vector<std::vector<double>> &lside, std::vector<double> &rside);

#endif // SPLINE_HPP
