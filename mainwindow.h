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
    void SetBaseAngle();
    void SetBaseAngle_v2();
    void SetBaseAngle_v3();
    void SetBaseAngle_v4();
    void SetBaseAngle_v5();

    void on_randButton_clicked();

    void on_initButton_clicked();

    void on_Alt_Method_clicked();

    void on_Alt_Method_toggled(bool checked);

private:
    Ui::MainWindow *ui;
     QVector<double> x,y1,y2,y3;
};
#endif // MAINWINDOW_H
