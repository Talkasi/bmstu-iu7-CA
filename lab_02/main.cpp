#include <iostream>
#include "table.h"
#include "io.hpp"
#include "newton.hpp"
#include "spline.hpp"

using namespace std;

int main()
{
    ifstream f = open_file("data/lab_02_data.txt");
    table_t table = fread_table(f);

    cout << "Working table:" << endl;
    print_table(table);

    double x;
    cout << "Enter X: ";
    cin >> x;

    double y_newton = newton(table, 3, x);
    double y_spline_natural = spline(table, x);
    // double y_spline_newton1 = spline(table, x);
    // double y_spline_newton2 = spline(table, x);

    cout << "X:                         " << x << endl;
    cout << "Y Newton:                  " << y_newton << endl;
    cout << "Y Spline natural:          " << y_spline_natural << endl;
    // cout << "Y Spline with one border:  " << y_spline_newton1 << endl;
    // cout << "Y Spline with two borders: " << y_spline_newton2 << endl;

    return 0;
}

