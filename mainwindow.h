#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void SetBaseAngle(double &number_steps, int &number_of_measures);
    void SetBaseAngle_v2();
    void SetBaseAngle_v3();
    void SetBaseAngle_v4();
    void SetBaseAngle_v5();

    void Init(double &number_steps, int &number_of_measures);

    void on_randButton_clicked();

    void on_initButton_clicked();


    void RandomizeAngles();

    void on_Alt_Method_toggled(bool checked);

    void on_zeroingAngles_clicked();
    //to set plates angles in band from 0 to 360
    void RoundAngles();
    void on_TestButton_clicked();
    void WriteDataInFile(double &p1, double &p2, double &p3, double &p4, double &number_steps, int &number_of_measures);
    void WriteDataInFile(double &p1, double &p2, double &number_steps, int &number_of_measures);
    void on_testInit_clicked();
    void TestIteration(double &p1, double &p2, double &p3, double &p4, double &number_steps, int &number_of_measures);

    void on_OpenFile_clicked();

private:
    Ui::MainWindow *ui;
     QVector<double> x,y1,y2,y3;
};
#endif // MAINWINDOW_H
