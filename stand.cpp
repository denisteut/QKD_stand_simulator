#include "stand.h"
#include <iostream>

const auto i = 0.0 + 1i;
Stand::Stand() {
    this->angle1 = 0;
    this->angle2 = 0;
    this->angle3 = 0;

    this->angle4 = 0;
    this->ScanM = false;

}


int Stand::GetSignals(double &h, double &v){
    if(ScanM){
        GetSignals1(std::ref(h), std::ref(v));
    }else{
        GetSignals2(std::ref(h), std::ref(v));
    }

    return 0;
}
int Stand::GetSignals(double &h, double &v, int &number_of_measures){
    if(ScanM){
        GetSignals1(std::ref(h), std::ref(v));
    }else{
        GetSignals2(std::ref(h), std::ref(v));
    }
    number_of_measures++;
    return 0;
}
int Stand::GetSignals1(double &h, double &v){
    Eigen::MatrixXcd Jones(2,1);
    Jones(0,0) = 0.0 + 1i;
    Jones(1,0) = 0.0 + 0i;;
    Jones = Jones * 100;
    Eigen::MatrixXcd hwp(2,2);
    Eigen::MatrixXcd qwp(2,2);
    Eigen::MatrixXcd lp1(2,2);
    lp1(0,0) = 1;
    lp1(0,1) = 0;
    lp1(1,0) = 0;
    lp1(1,1) = 0;
    Eigen::MatrixXcd lp2(2,2);
    lp2(0,0) = 0;
    lp2(0,1) = 0;
    lp2(1,0) = 0;
    lp2(1,1) = 1;
    HwpPass(angle1,ref(hwp), ref(Jones));
    QwpPass(angle2,ref(qwp), ref(Jones));
    HwpPass(angle3,ref(hwp), ref(Jones));
    QwpPass(angle4,ref(qwp), ref(Jones));

    //Eigen::MatrixXcd Jones1 = lp1 * Jones;
    //Eigen::MatrixXcd Jones2= lp2 * Jones;
    //h = Jones1.norm();
    //v = Jones2.norm();
    h = pow(abs(Jones(0,0)),2);
    v = pow(abs(Jones(1,0)),2);
    return 1;
}

int Stand::GetSignals2(double &h, double &v){

    Eigen::MatrixXcd one_zero(2,1);
    one_zero(0,0) = 1.0;
    one_zero(1,0) = 0.0;
    Eigen::MatrixXcd P0 = Mmatrix(angle1, 1/2.0) * one_zero;
    Eigen::MatrixXcd P1 = Mmatrix(angle2, 1/4.0) * P0;
    Eigen::MatrixXcd P2 = Mmatrix(angle3, 1/2.0) * P1;
    Eigen::MatrixXcd P3 = Mmatrix(angle4, 1/4.0) * P2;

    h = pow(abs(P3(0,0)),2)*100;
    v = pow(abs(P3(1,0)),2)*100;

    return 1;
}

Eigen::MatrixXcd Rmatrix(double angle){
    Eigen::MatrixXcd r_marix(2,2);
    r_marix(0,0) = cos(angle);
    r_marix(0,1) = -sin(angle);
    r_marix(1,0) = sin(angle);
    r_marix(1,1) = cos(angle);
    return r_marix;
}

//plate = pi if 1/2 pi/2 if 1/4
Eigen::MatrixXcd Pmatrix(double plate){
    Eigen::MatrixXcd p_matrix(2,2);
    p_matrix(0,0) = exp(-i*(plate/2));
    p_matrix(0,1) = 0;
    p_matrix(1,0) = 0;
    p_matrix(1,1) = exp(i*(plate/2));

    return p_matrix;
}

Eigen::MatrixXcd Stand::Mmatrix(double angle, double plate){
    if(plate == 0.25){
        plate = M_PI/2.0;
    }
    else if(plate == 0.5){
        plate = M_PI;
    }
    Eigen::MatrixXcd m_marix = Rmatrix(-angle)*Pmatrix(plate)*Rmatrix(angle);

    return m_marix;
}



void Stand::QwpPass(double angle, Eigen::MatrixXcd &qwp, Eigen::MatrixXcd &Jones){

    qwp(0,0) = pow(cos(angle),2)+i*pow(sin(angle),2);
    qwp(1,0) = (1.0-i)*sin(angle)*cos(angle);
    qwp(0,1) = (1.0-i)*sin(angle)*cos(angle);
    qwp(1,1) = pow(sin(angle),2)+i*pow(cos(angle),2);
    qwp = pow(M_E,((-i*M_PI)/4.0)) * qwp;

    Jones = qwp * Jones;
    return;
}

void Stand::HwpPass(double angle, Eigen::MatrixXcd &hwp, Eigen::MatrixXcd &Jones){
    //hwp(0,0) = cos(2*angle);
    //hwp(1,0) = sin(2*angle);
    //hwp(0,1) = sin(2*angle);
    //hwp(1,1) = -cos(2*angle);

    //hwp = pow(M_E,((-i*M_PI)/2.0)) * hwp;

    hwp(0,0) = pow(cos(angle),2)-pow(sin(angle),2);
    hwp(1,0) = 2*cos(angle)*sin(angle);
    hwp(0,1) = 2*cos(angle)*sin(angle);
    hwp(1,1) = pow(sin(angle),2)-pow(cos(angle),2);

    hwp = pow(M_E,((-i*M_PI)/2.0)) * hwp;


    Jones = hwp * Jones;

    return;
}



void Stand::SetAngle(int plate_number, double angle){
    switch (plate_number){
        case 1:
            this->angle1 = angle;
        break;
        case 2:
            this->angle2 = angle;
        break;
        case 3:
            this->angle3 = angle;
        break;
        case 4:
            this->angle4= angle;
        break;
    }
}

void Stand::SetAngle(double angle1, double angle2, double angle3, double angle4){
    this->angle1= angle1;
    this->angle2= angle2;
    this->angle3= angle3;
    this->angle4= angle4;
}
double Stand::GetAngle(int plate_number){
    switch (plate_number) {
    case 1:
        return this->angle1;
        break;
    case 2:
        return this->angle2;
        break;
    case 3:
        return this->angle3;
        break;

    case 4:
        return this->angle4;
        break;
    default:
        return 0;
        break;
    }
}

void Stand::Init1(){

}
void Stand::Init2(){

}
