#ifndef STAND_H
#define STAND_H
#include "Eigen/Dense"
#include <math.h>
#include <cmath>
#include <complex>

using namespace std::complex_literals;

class Stand
{
public:
    Stand();
    int GetSignals(double &h, double &v);
    int GetSignals1(double &h, double &v);
    int GetSignals2(double &h, double &v);
    //creates complexmatrix of plates
    void QwpPass(double angle, Eigen::MatrixXcd &qwp, Eigen::MatrixXcd &Jones);
    void HwpPass(double angle, Eigen::MatrixXcd &hwp, Eigen::MatrixXcd &Jones);
    void SetAngle(int plate_number, double angle);
    void SetAngle(double angle1, double angle2, double angle3, double angle4);
    double GetAngle(int plate_number);
    void Init1();
    void Init2();
    bool ScanM;

private:
    Eigen::MatrixXcd Mmatrix(double angle, double plate);
    double angle1, angle2, angle3, angle4, power_level;

    Eigen::MatrixXcd hwp;
    Eigen::MatrixXcd qwp;
    std::complex<double> complexnum;
    const std::complex<double> i = 0.0 + 1.0i;

};

#endif // STAND_H
