#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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

public slots:
  void setScore(int s, int c);

private slots:
  void on_actionEasy_triggered(bool checked);

  void on_actionNormal_triggered(bool checked);

  void on_actionHard_triggered(bool checked);

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_HPP
