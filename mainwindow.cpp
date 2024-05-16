#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stand.cpp"
#include <thread>
#include <chrono>

Stand stand;
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
    if(stand.ScanM){
        ui->displayMLabel->setText("1");
    }else{
        ui->displayMLabel->setText("0");
    }
    int plate_number = ui->PlateNumber->currentIndex()+1; // номер пластины
    int maxangle = 90;
    if(ui->maxRotateStep->text() != ""){
        maxangle = ui->maxRotateStep->text().toInt();
        if (maxangle > 360){
            maxangle = maxangle%360;
        }
    }



    using namespace std::chrono_literals;

    try {
        stand.SetAngle(ui->BaseAngle_1->text().toDouble(), ui->BaseAngle_2->text().toDouble(), ui->BaseAngle_3->text().toDouble(), ui->BaseAngle_4->text().toDouble());
    } catch (QException ex) {
        std::cout << "Wrong angles!\n" << ex.what() << std::endl;
    }

    double h,v = 0.0;


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
            currangle = stand.GetAngle(1);
        break;
        case 2:
            currangle = stand.GetAngle(2);
        break;
        case 3:
            currangle = stand.GetAngle(3);
        break;
        case 4:
            currangle = stand.GetAngle(4);
        break;
        default:
            currangle = 0;
        break;
    }


    for(int i = 0; i < maxangle; i=i+1){

        stand.SetAngle(plate_number, (currangle + i)/180.0*M_PI);

        if(ui->Alt_Method->isChecked()){
            stand.GetSignals(std::ref(h),std::ref(v));
        }
        else{
            stand.GetSignals2(std::ref(h),std::ref(v));
        }

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
    double  a1 = std::rand()%360,
            a2 = std::rand()%360,
            a3 = std::rand()%360,
            a4 = std::rand()%360;
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
      stand.SetAngle(4,stand.GetAngle(4)+2.0/180.0*M_PI);//Поворот 4 пластины на 1 шаг


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

    switch (ui->init_ComboBox->currentIndex()) {
    case 0:
        SetBaseAngle();
        //Init1();
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
    stand.ScanM = !stand.ScanM;
}

