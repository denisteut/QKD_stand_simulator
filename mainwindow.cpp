#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stand.cpp"
#include <thread>
#include <chrono>
#include <iostream>

#include <fstream>
Stand stand;
std::ofstream table_result;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    double step = ui->scanStepLine->text().toDouble();
    if(step == 0){
        step = 1.0;
    }

    int plate_number = ui->PlateNumber->currentIndex()+1; // номер пластины
    int maxangle = 180;
    if(ui->maxRotateStep->text() != ""){
        maxangle = ui->maxRotateStep->text().toInt();
        if (maxangle > 360){
            maxangle = maxangle%360;
        }
    }



    using namespace std::chrono_literals;

    try {
        stand.SetAngle(ui->BaseAngle_1->text().toDouble()/180.0*M_PI, ui->BaseAngle_2->text().toDouble()/180.0*M_PI,
                       ui->BaseAngle_3->text().toDouble()/180.0*M_PI, ui->BaseAngle_4->text().toDouble()/180.0*M_PI);
    } catch (QException ex) {
        std::cout << "Wrong angles!\n" << ex.what() << std::endl;
    }

    double h=0.0,v = 0.0;


    //plot settings
    {
        ui->widget->clearGraphs();
        x.clear();
        y1.clear();
        y2.clear();

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(QColor(0, 255, 255, 100)));
        ui->widget->graph(0)->setBrush(QBrush(QColor(0, 255, 255, 70)));

        ui->widget->addGraph();
        ui->widget->graph(1)->setPen(QPen(QColor(255, 0, 255, 100)));
        ui->widget->graph(1)->setBrush(QBrush(QColor(255, 0, 255, 70)));

        ui->widget->xAxis->setRange(0 ,maxangle);
        ui->widget->xAxis->ticker()->setTickCount(maxangle/10.0);
    }
    double hmax=0, vmax=0;
    double currangle = 0.0;

    switch(plate_number){
        case 1:
            currangle = ui->BaseAngle_1->text().toDouble();
        break;
        case 2:
            currangle = ui->BaseAngle_2->text().toDouble();
        break;
        case 3:
            currangle = ui->BaseAngle_3->text().toDouble();
        break;
        case 4:
            currangle = ui->BaseAngle_4->text().toDouble();
        break;
        default:
            currangle = 0;
        break;
    }


    for(double i = 0; i < maxangle+0.1; i=i+step){

        stand.SetAngle(plate_number, (currangle + i)/180.0*M_PI);

        stand.GetSignals(std::ref(h),std::ref(v));

        if(hmax < h){
            hmax = h;
        }
        if(vmax < v){
            vmax = v;
        }
        if (vmax>hmax){ui->widget->yAxis->setRange(0 ,vmax + 10);}
                    else{ui->widget->yAxis->setRange(0 ,hmax + 10);}

        //std::this_thread::sleep_for(100ms);

        x.push_back(i);
        y1.push_back(h);
        y2.push_back(v);

        ui->widget->graph(0)->addData(x,y1);
        ui->widget->graph(1)->addData(x,y2);
        ui->widget->replot();
    }
}

void MainWindow::on_randButton_clicked()
{
    RandomizeAngles();
}
void MainWindow::RandomizeAngles(){
    double  a1 = fmod(std::rand()/10.0, 360.0),
        a2 = fmod(std::rand()/10.0, 360.0),
        a3 = fmod(std::rand()/10.0, 360.0),
        a4 = fmod(std::rand()/10.0, 360.0);
    stand.SetAngle(1,(a1)/180.0*M_PI);
    stand.SetAngle(2,(a2)/180.0*M_PI);
    stand.SetAngle(3,(a3)/180.0*M_PI);
    stand.SetAngle(4,(a4)/180.0*M_PI);
    ui->BaseAngle_1->setText(QString::number(a1));
    ui->BaseAngle_2->setText(QString::number(a2));
    ui->BaseAngle_3->setText(QString::number(a3));
    ui->BaseAngle_4->setText(QString::number(a4));

}

void MainWindow::SetBaseAngle(){

    double s1 = 0.0, s2 = 0.0;
    double startplates[4];
    startplates[0] = stand.GetAngle(1);
    startplates[1] = stand.GetAngle(2);
    startplates[2] = stand.GetAngle(3);
    startplates[3] = stand.GetAngle(4);
    double accuracy = 1.0;
    if(ui->accuracyLine->text() != ""){
        accuracy = ui->accuracyLine->text().toDouble();
    }
    if(accuracy == 0){
        accuracy = 1.0;
    }

    stand.GetSignals(std::ref(s1),std::ref(s2));


    double MaxSDelta = 0, Delta1, Delta2, SDelta,
        DirPlate2 = 1, RotStep2 = 45/180.0*M_PI,
        MaxSLevel1 = 0, MaxSLevel2 = s1, MinSLevel1 = s1,
        Step4_1 = 0,    Step4_2 = 0, flag = 0, SLevel1, SLevel2;

    do{
        do{
            stand.GetSignals(std::ref(s1),std::ref(s2));
            if(s1 > MaxSLevel1){
                MaxSLevel1 = s1;
                Step4_1 = stand.GetAngle(4); //max
            }
            if(s1 < MinSLevel1){
                MinSLevel1 = s1;
                Step4_2 = stand.GetAngle(4); //min
            }
            stand.SetAngle(4,stand.GetAngle(4)+accuracy/180.0*M_PI);//Поворот 4 пластины на 1 шаг
        } while (stand.GetAngle(4) - startplates[3] < M_PI);
        SDelta = MaxSLevel1 - MinSLevel1;
        stand.SetAngle(4, startplates[3]);
        if ((MaxSLevel1 - MinSLevel1) > MaxSDelta){
            stand.SetAngle(2, stand.GetAngle(2) + DirPlate2 * RotStep2);
        }
        else {
            if(RotStep2 > 1.0/180.0*M_PI){
                RotStep2 = RotStep2 / 2;

                DirPlate2 = -DirPlate2;
                stand.SetAngle(2, stand.GetAngle(2) + DirPlate2 * RotStep2);
            }
            else{

                flag = 1;
            }

        }
        MaxSDelta = MaxSLevel1 - MinSLevel1;
        MaxSLevel1 = MaxSDelta/2;
        MinSLevel1 = MaxSLevel1;
    } while(flag != 1);
    stand.SetAngle(4, Step4_1);

    //stand.SetAngle(stand.GetAngle(1),stand.GetAngle(2)+45.0/180.0*M_PI,stand.GetAngle(3),stand.GetAngle(4)+45/180.0*M_PI);
    stand.SetAngle(2, stand.GetAngle(2)+45.0/180.0*M_PI);
    stand.SetAngle(4, stand.GetAngle(4)+45.0/180.0*M_PI);

    MaxSLevel1 = 0;
    double Step3 = 0;
    do{

        stand.GetSignals(std::ref(s1),std::ref(s2));

        stand.SetAngle(3, stand.GetAngle(3)+accuracy/180.0*M_PI);

        if(s1 > MaxSLevel1){
            MaxSLevel1 = s1;
            Step3 = stand.GetAngle(3);
        }
    } while((stand.GetAngle(3)-startplates[2]) < 90.0/180.0*M_PI); // Пока палстина 3 не повернулась на 90 градусов (не прошла 300 шагов)

    stand.SetAngle(3,Step3);
    double  a1 = stand.GetAngle(1)*180.0/M_PI,
            a2 = stand.GetAngle(2)*180.0/M_PI,
            a3 = stand.GetAngle(3)*180.0/M_PI,
            a4 = stand.GetAngle(4)*180.0/M_PI;

    ui->BaseAngle_1->setText(QString::number(a1));
    ui->BaseAngle_2->setText(QString::number(a2));
    ui->BaseAngle_3->setText(QString::number(a3));
    ui->BaseAngle_4->setText(QString::number(a4));
}
int Get_max_points(int *points){
  int res = fmax(points[0], points[1]);
  return fmax(res, points[2]);
}
int Get_min_points(int *points){
  int res = fmin(points[0], points[1]);
  return fmin(res, points[2]);
}

void GetPoints(int *points, double Angles[4], int t0, int t1, int t2, int t3){
  //int points[3] = {0, 0, 0};
    double s1 = 0.0, s2 = 0.0;
    stand.SetAngle(Angles[0], Angles[1], Angles[2], Angles[3]);
    stand.GetSignals(std::ref(s1),std::ref(s2));

    points[0] = s1;
    Angles[0] = Angles[0] + 22.5/180.0*M_PI * t0;
    Angles[1] = Angles[1] + 45.0/180.0*M_PI * t1;
    Angles[2] = Angles[2] + 22.5/180.0*M_PI * t2;
    Angles[3] = Angles[3] + 45.0/180.0*M_PI * t3;
    stand.SetAngle(Angles[0], Angles[1], Angles[2], Angles[3]);
    stand.GetSignals(std::ref(s1),std::ref(s2));

    points[1] = s1;

    Angles[0] = Angles[0] + 22.5/180.0*M_PI * t0;
    Angles[1] = Angles[1] + 45.0/180.0*M_PI * t1;
    Angles[2] = Angles[2] + 22.5/180.0*M_PI * t2;
    Angles[3] = Angles[3] + 45.0/180.0*M_PI * t3;


    stand.SetAngle(Angles[0], Angles[1], Angles[2], Angles[3]);
    stand.GetSignals(std::ref(s1),std::ref(s2));

    points[2] = s1;

    return;
}

int GetDAngle(double Angles[4], int t0, int t1, int t2, int t3){

  int points[3] = {0, 0, 0};
  double k10, k21, k20,
      dAngle, preset;
  double dDiv, dTan;
  GetPoints(points, Angles, t0, t1, t2, t3);


  k10 = points[1] - points[0];
  k21 = points[2] - points[1];
  k20 = points[2] - points[0];
  int abs_k10 = abs(k10);
  int abs_k21 = abs(k21);
  dDiv = k20/(2.0*fmax(abs_k10, abs_k21));

  if(dDiv > 0.5){
    preset = 45/180*M_PI;
  }
  else if(dDiv < -0.5){
    preset = -45/180*M_PI;
  }
  else if(k21 >k10){
    preset = 90/180*M_PI;
  }else{
    preset = 0;
  }
  if(preset != 0){
    Angles[0] = Angles[0] + t0 * preset * 0.5;
    Angles[1] = Angles[1] + t1 * preset;
    Angles[2] = Angles[2] + t2 * preset * 0.5;
    Angles[3] = Angles[3] + t3 * preset;
    GetPoints(points, Angles, t0, t1, t2, t3);
    k10 = points[1] - points[0];
    k21 = points[2] - points[1];
    k20 = points[2] - points[0];

    abs_k10 = abs(k10);
    abs_k21 = abs(k21);
    dDiv = k20 / (2.0 * fmax(abs_k10, abs_k21));
  }

  if (k10 < k21){
    dAngle = double (-(dDiv + 1) * 45/180*M_PI);
  }
  else {
    dAngle = double ((dDiv + 1) * 45/180*M_PI);
  }
  Angles[0] = Angles[0] + t0 * dAngle * 0.5;
  Angles[1] = Angles[1] + t1 * dAngle;
  Angles[2] = Angles[2] + t2 * dAngle * 0.5;
  Angles[3] = Angles[3] + t3 * dAngle;

  //GetPoints(points, Angles, t0, t1, t2, t3);
  //dTan = (points[1] - points[0]) / (points[2] - points[1]);
  //dAngle = int(dAngle + preset - 300 * atan((dTan - 1) / (dTan + 1)) / M_PI);


  return dAngle;
}

void MainWindow::SetBaseAngle_v2(){
  int points[3] = {0, 0, 0}; //измеренные уровни сигналов в процессе выполнения процедуры
  double startplates[4];
  startplates[0] = stand.GetAngle(1);

  startplates[1] = stand.GetAngle(2);
  startplates[2] = stand.GetAngle(3);
  startplates[3] = stand.GetAngle(4);
  double Angles[4] = {startplates[0], startplates[1], startplates[2], startplates[3]}; //искомые углы волновых пластин
  GetPoints(points, Angles, 0, 0, 1, 1);



  double dAngle; // углы сдвига фазы

  if (Get_max_points(points) - Get_min_points(points) < 0.25 * (Get_max_points(points) + Get_min_points(points))){
    Angles[1] += 45/180.0*M_PI; // +45
  }
  dAngle = GetDAngle(Angles, 0, 0, 1, 1);
  Angles[1] += 90/180.0*M_PI; // 90
  Angles[2] += dAngle * 0.5;
  Angles[3] += dAngle;
  dAngle = GetDAngle(Angles, 0, 0, 1, 1);
  Angles[2] = int(Angles[2] + dAngle * 0.25 - 22.5/180.0*M_PI); //22.5
  Angles[3] = int(Angles[3] + dAngle * 0.5 - 45/180.0*M_PI); //45
  dAngle = GetDAngle(Angles, 0, 1, 0, 0);
  Angles[1] = Angles[1] + dAngle + 45/180.0*M_PI;
  Angles[3] += 45/180.0*M_PI;

  dAngle = GetDAngle(Angles, 0, 0, 1, 0);
  Angles[2] = Angles[2] + dAngle * 0.5;

  //SetPlatesAngles(Angles[0], Angles[1], Angles[2], Angles[3]);

    stand.SetAngle(Angles[0], Angles[1], Angles[2], Angles[3]);



  double  a1 = stand.GetAngle(1)*180.0/M_PI,
          a2 = stand.GetAngle(2)*180.0/M_PI,
          a3 = stand.GetAngle(3)*180.0/M_PI,
          a4 = stand.GetAngle(4)*180.0/M_PI;

  ui->BaseAngle_1->setText(QString::number(a1));
  ui->BaseAngle_2->setText(QString::number(a2));
  ui->BaseAngle_3->setText(QString::number(a3));
  ui->BaseAngle_4->setText(QString::number(a4));



}

void MainWindow::SetBaseAngle_v3(){
    double s1 = 0.0, s2 = 0.0;
    double startplates[4];
    startplates[0] = stand.GetAngle(1);
    startplates[1] = stand.GetAngle(2);
    startplates[2] = stand.GetAngle(3);
    startplates[3] = stand.GetAngle(4);

    stand.GetSignals(std::ref(s1),std::ref(s2));


    double MaxSDelta = 0, Delta1, Delta2, SDelta,
        DirPlate2 = 1, RotStep2 = 90/180.0*M_PI,
        MaxSLevel1 = 0, MinSLevel1 = s1,
        Step4_1 = 0,    Step4_2 = 0, flag = 0;

    do{
        do{
            stand.GetSignals(std::ref(s1),std::ref(s2));
            if(s1 > MaxSLevel1){
                MaxSLevel1 = s1;
                Step4_1 = stand.GetAngle(4); //max
            }
            if(s1 < MinSLevel1){
                MinSLevel1 = s1;
                Step4_2 = stand.GetAngle(4); //min
            }
            stand.SetAngle(4,stand.GetAngle(4)+1.0/180.0*M_PI);//Поворот 4 пластины на 1 шаг


        } while (stand.GetAngle(4) - startplates[3] < M_PI);


        stand.SetAngle(4, Step4_1);
        stand.GetSignals(std::ref(s1),std::ref(s2));

        Delta1 = abs(s1-s2);

        stand.SetAngle(4, Step4_2);
        stand.GetSignals(std::ref(s1),std::ref(s2));

        Delta2 = abs(s1-s2);


        SDelta = Delta2 + Delta1;
        stand.SetAngle(4, startplates[3]);

        if ((SDelta) > MaxSDelta){

            stand.SetAngle(2, stand.GetAngle(2) + DirPlate2 * RotStep2);
            MaxSDelta = SDelta;
        }
        else {
            if(RotStep2 > 1/180.0*M_PI){
                RotStep2 = RotStep2 / 2;

                DirPlate2 = -DirPlate2;
                stand.SetAngle(2, stand.GetAngle(2) + DirPlate2 * RotStep2);
            }
            else{

                flag = 1;
            }

        }

        //MaxSLevel1 = MaxSDelta/2;
        //MinSLevel1 = MaxSLevel1;

    } while(flag != 1);



    stand.SetAngle(4, Step4_1);

    //stand.SetAngle(stand.GetAngle(1),stand.GetAngle(2)+45.0/180.0*M_PI,stand.GetAngle(3),stand.GetAngle(4)+45/180.0*M_PI);
    stand.SetAngle(2, stand.GetAngle(2)+45.0/180.0*M_PI);
    stand.SetAngle(4, stand.GetAngle(4)+45.0/180.0*M_PI);

    MaxSLevel1 = 0;
    double Step3 = 0;

    do{

        stand.GetSignals(std::ref(s1),std::ref(s2));

        stand.SetAngle(3, stand.GetAngle(3)+1.0/180.0*M_PI);

        if(s1 > MaxSLevel1){
            MaxSLevel1 = s1;
            Step3 = stand.GetAngle(3);
        }
    } while((stand.GetAngle(3)-startplates[2]) < 90.0/180.0*M_PI); // Пока палстина 3 не повернулась на 90 градусов (не прошла 300 шагов)

    stand.SetAngle(3,Step3);
    double  a1 = stand.GetAngle(1)*180.0/M_PI,
        a2 = stand.GetAngle(2)*180.0/M_PI,
        a3 = stand.GetAngle(3)*180.0/M_PI,
        a4 = stand.GetAngle(4)*180.0/M_PI;

    ui->BaseAngle_1->setText(QString::number(a1));
    ui->BaseAngle_2->setText(QString::number(a2));
    ui->BaseAngle_3->setText(QString::number(a3));
    ui->BaseAngle_4->setText(QString::number(a4));

}

//min и max массива из 3 элементов по ссылке на него

void MainWindow::on_initButton_clicked()
{

    Init();
}

void MainWindow::Init(){
    switch (ui->init_ComboBox->currentIndex()) {
    case 0:
        SetBaseAngle();
        break;
    case 1:
        SetBaseAngle_v2();
        break;
    case 2:
        SetBaseAngle_v3();
        break;
    case 3:
        SetBaseAngle_v4();
        break;
    case 4:
        SetBaseAngle_v5();
        break;
    default:
        break;
    }
}
void MainWindow::RoundAngles(){
    if (abs(ui->BaseAngle_1->text().toDouble()) > 360){

        ui->BaseAngle_1->setText(QString::number(fmod(ui->BaseAngle_1->text().toDouble(),360.0)));
        stand.SetAngle(1,ui->BaseAngle_1->text().toDouble()/180.0*M_PI);
    }
    if (abs(ui->BaseAngle_2->text().toDouble()) > 360){

        ui->BaseAngle_2->setText(QString::number(fmod(ui->BaseAngle_2->text().toDouble(),360.0)));
        stand.SetAngle(2,ui->BaseAngle_2->text().toDouble()/180.0*M_PI);
    }
    if (abs(ui->BaseAngle_3->text().toDouble()) > 360){

        ui->BaseAngle_3->setText(QString::number(fmod(ui->BaseAngle_3->text().toDouble(),360.0)));
        stand.SetAngle(3,ui->BaseAngle_3->text().toDouble()/180.0*M_PI);
    }
    if (abs(ui->BaseAngle_4->text().toDouble()) > 360){

        ui->BaseAngle_4->setText(QString::number(fmod(ui->BaseAngle_4->text().toDouble(),360.0)));
        stand.SetAngle(4,ui->BaseAngle_4->text().toDouble()/180.0*M_PI);
    }


}


void MainWindow::SetBaseAngle_v4(){
    double s1 = 0.0, s2 = 0.0;
    double startplates[4];
    startplates[0] = stand.GetAngle(1);
    startplates[1] = stand.GetAngle(2);
    startplates[2] = stand.GetAngle(3);
    startplates[3] = stand.GetAngle(4);

    stand.GetSignals(std::ref(s1),std::ref(s2));


    double  DeltaPrev = 0.0, DeltaNew = 0.0,
            MaxSLevel1 = 0.0, MaxSLevel2 = 0.0,
            Step4_1 = 0.0, Step4_2 = 0.0,
            Delta1 = 0.0, Delta2 = 0.0, flag = 0;
    do{
        do{
            stand.GetSignals(std::ref(s1),std::ref(s2));
            if(s1 > MaxSLevel1){
                MaxSLevel1 = s1;
                Step4_1 = stand.GetAngle(4); //max1
            }
            if(s1 < MaxSLevel2){
                MaxSLevel2 = s1;
                Step4_2 = stand.GetAngle(4); //max2
            }
            stand.SetAngle(4,stand.GetAngle(4) + 1.0/180.0*M_PI);//Поворот 4 пластины на 1 шаг


        } while (stand.GetAngle(4) - startplates[3] < M_PI);


        stand.SetAngle(4, Step4_1);
        stand.GetSignals(std::ref(s1),std::ref(s2));

        Delta1 = abs(s1-s2);

        stand.SetAngle(4, Step4_2);
        stand.GetSignals(std::ref(s1),std::ref(s2));

        Delta2 = abs(s1-s2);

        if(Delta2 > Delta1){
            DeltaNew = Delta2;
        }else{
            DeltaNew = Delta1;
        }

        if(DeltaNew > DeltaPrev){
            DeltaPrev = DeltaNew;
            stand.SetAngle(2, stand.GetAngle(2) + 1.0/180.0*M_PI);
        }else{
            stand.SetAngle(2, stand.GetAngle(2) - 1.0/180.0*M_PI);
            flag = 1;}

    } while(flag != 1);





    //stand.SetAngle(stand.GetAngle(1),stand.GetAngle(2)+45.0/180.0*M_PI,stand.GetAngle(3),stand.GetAngle(4)+45/180.0*M_PI);
    stand.SetAngle(2, stand.GetAngle(2)+22.5/180.0*M_PI);
    stand.SetAngle(4, stand.GetAngle(4)+22.5/180.0*M_PI);

    stand.GetSignals(std::ref(s1),std::ref(s2));
    stand.SetAngle(3, stand.GetAngle(3)+1.0/180.0*M_PI);



    do{

        stand.GetSignals(std::ref(s1),std::ref(s2));
        MaxSLevel1 = s1;

        stand.SetAngle(3, stand.GetAngle(3)+1.0/180.0*M_PI);
        stand.GetSignals(std::ref(s1),std::ref(s2));

        if(s1 > MaxSLevel1){
            MaxSLevel1 = s1;
        }
        else{
            flag = 0;
        }
    } while(flag != 0); // Пока палстина 3 не повернулась на 90 градусов (не прошла 300 шагов)


    double  a1 = stand.GetAngle(1)*180.0/M_PI,
        a2 = stand.GetAngle(2)*180.0/M_PI,
        a3 = stand.GetAngle(3)*180.0/M_PI,
        a4 = stand.GetAngle(4)*180.0/M_PI;

    ui->BaseAngle_1->setText(QString::number(a1));
    ui->BaseAngle_2->setText(QString::number(a2));
    ui->BaseAngle_3->setText(QString::number(a3));
    ui->BaseAngle_4->setText(QString::number(a4));

}


void MainWindow::SetBaseAngle_v5(){return;}



void MainWindow::on_Alt_Method_clicked()
{

}


void MainWindow::on_Alt_Method_toggled(bool checked)
{
    if(checked){
        stand.ScanM = true;
    }
    else{
        stand.ScanM = false;
    }
}


void MainWindow::on_zeroingAngles_clicked()
{

    stand.SetAngle(1,0);
    stand.SetAngle(2,0);
    stand.SetAngle(3,0);
    stand.SetAngle(4,0);
    ui->BaseAngle_1->setText(QString::number(0));
    ui->BaseAngle_2->setText(QString::number(0));
    ui->BaseAngle_3->setText(QString::number(0));
    ui->BaseAngle_4->setText(QString::number(0));
}


void MainWindow::on_TestButton_clicked()
{
    RoundAngles();
}

void MainWindow::TestIteration(double &p1, double &p2, double &p3, double &p4){
    Init();
    double  base_angle1 = ui->BaseAngle_1->text().toDouble()/180.0*M_PI,
            base_angle2 = ui->BaseAngle_2->text().toDouble()/180.0*M_PI,
            base_angle3 = ui->BaseAngle_3->text().toDouble()/180.0*M_PI,
            base_angle4 = ui->BaseAngle_4->text().toDouble()/180.0*M_PI,
            s1 = 0.0, s2 = 0.0,
            delta1 = 0.0, delta2 = 0.0;
    stand.GetSignals(std::ref(s1), std::ref(s2));
    delta1 = abs(s1-s2);

    stand.SetAngle(1,base_angle1+22.5/180.0*M_PI);
    stand.GetSignals(std::ref(s1), std::ref(s2));
    p1 = abs(s1-s2);

    stand.SetAngle(1,base_angle1+45.0/180.0*M_PI);
    delta2 = abs(s1-s2);
    p2 = abs(delta1-delta2);

    stand.SetAngle(1,base_angle1);

    stand.SetAngle(2, base_angle2+45.0/180.0*M_PI);
    stand.GetSignals(std::ref(s1), std::ref(s2));
    p3 = abs(s1-s2);
    stand.SetAngle(2, base_angle2+90.0/180.0*M_PI);
    delta2 = abs(s1-s2);
    p4 = abs(delta1-delta2);




}

void MainWindow::WriteDataInFile( double &p1, double &p2, double &p3, double &p4){

    table_result << p1 << "," << p2 << "," << p3 << "," << p4 << "\n";


}
void MainWindow::on_testInit_clicked()
{


    table_result.open("testres.csv");
    table_result<<"p1,p2,p3,p4\n";

    int num_iter = 100;
    if(ui->numberOfIter->text() != ""){
        num_iter = ui->numberOfIter->text().toInt();
    }

    double p1 = 0.0, p2 = 0.0, p3 = 0.0, p4 = 0.0;
    for(int j = 0; j < num_iter; j++){
        RandomizeAngles();
        TestIteration(std::ref(p1), std::ref(p2), std::ref(p3), std::ref(p4));
        WriteDataInFile(std::ref(p1), std::ref(p2), std::ref(p3), std::ref(p4));
    }
    table_result <<"=AVERAGE(A2:A"<<num_iter+1<<"),"<<"=AVERAGE(B2:B"<<num_iter+1<<"),"<<"=AVERAGE(C2:C"<<num_iter+1<<"),"<<"=AVERAGE(D2:D"<<num_iter+1<<")\n";
    table_result << num_iter << " iterations is finished";
    table_result.close();
}


void MainWindow::on_OpenFile_clicked()
{
    std::string command = "libreoffice ./testres.csv";
    system(command.c_str());
}

