#include "io.hpp"
#include "newton.hpp"
#include "spline.hpp"
#include "table.h"
#include <iostream>
#include <sciplot/sciplot.hpp>

using namespace sciplot;
using namespace std;

double get_min_x(table_t &table);
double get_max_x(table_t &table);

Vec x_vec(table_t &table);
Vec y_vec(table_t &table);
Vec newton_vec(table_t &table, int n, Vec x, bool verbose);
Vec spline_vec(table_t &table, Vec x, double s_border, double e_border, bool verbose);

int main()
{
    ifstream f = open_file("data/lab_02_data.txt");
    table_t table = fread_table(f);

    cout << "Working table:" << endl;
    print_table(table);

    double x;
    cout << "Enter X: ";
    cin >> x;

    double min_x = get_min_x(table);
    double max_x = get_max_x(table);

    double s_border = newton_second_derivative(table, min_x);
    double e_border = newton_second_derivative(table, max_x);

    cout << "Newton: " << endl;
    double y_newton = newton(table, 3, x, true);

    cout << "Spline natural: " << endl;
    double y_spline_natural = spline(table, x, 0, 0, true);

    cout << "Spline with one border: " << endl;
    double y_spline_newton1 = spline(table, x, s_border, 0, true);

    cout << "Spline with two borders: " << endl;
    double y_spline_newton2 = spline(table, x, s_border, e_border, true);

    cout << "X:                         " << x << endl;
    cout << "Y Newton:                  " << y_newton << endl;
    cout << "Y Spline natural:          " << y_spline_natural << endl;
    cout << "Y Spline with one border:  " << y_spline_newton1 << endl;
    cout << "Y Spline with two borders: " << y_spline_newton2 << endl;

    Vec _x = linspace((const int)min_x, (const int)max_x, 200);

    Plot2D plot;

    plot.fontName("Palatino");
    plot.fontSize(12);

    plot.xlabel("x");
    plot.ylabel("y");

    plot.legend().atOutsideRight().fontSize(10).displayVertical().displayExpandWidthBy(2);

    plot.drawCurve(x_vec(table), y_vec(table)).label("Given data").lineWidth(2);
    plot.drawCurve(_x, newton_vec(table, 3, _x, false)).label("Newton").lineWidth(2);
    plot.drawCurve(_x, spline_vec(table, _x, 0, 0, false)).label("Spline natural").lineWidth(2);
    plot.drawCurve(_x, spline_vec(table, _x, s_border, 0, false)).label("Spline with one border").lineWidth(2);
    plot.drawCurve(_x, spline_vec(table, _x, s_border, e_border, false)).label("Spline with two borders").lineWidth(2);

    Figure fig = {{plot}};
    Canvas canvas = {{fig}};
    canvas.size(1000, 700);

    canvas.show();

    return 0;
}

double get_min_x(table_t &table)
{
    double min = table[0][X];
    for (int i = 0; i < table.size(); ++i)
        if (min > table[i][X])
            min = table[i][X];

    return min;
}

double get_max_x(table_t &table)
{
    double max = table[0][X];
    for (int i = 0; i < table.size(); ++i)
        if (max < table[i][X])
            max = table[i][X];

    return max;
}

Vec newton_vec(table_t &table, int n, Vec x, bool verbose)
{
    Vec res(x.size());
    for (int i = 0; i < x.size(); ++i)
        res[i] = newton(table, n, x[i], verbose);

    return res;
}

Vec spline_vec(table_t &table, Vec x, double s_border, double e_border, bool verbose)
{
    Vec res(x.size());
    for (int i = 0; i < x.size(); ++i)
        res[i] = spline(table, x[i], s_border, e_border, verbose);

    return res;
}

Vec x_vec(table_t &table)
{
    Vec res(table.size());
    for (int i = 0; i < table.size(); ++i)
        res[i] = table[i][X];

    return res;
}

Vec y_vec(table_t &table)
{
    Vec res(table.size());
    for (int i = 0; i < table.size(); ++i)
        res[i] = table[i][Y];

    return res;
}
