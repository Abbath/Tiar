#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
  void writeScoreboard();
  void readScoreboard();
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void closeEvent(QCloseEvent* e);

public slots:
  void setScore(int s, int c);

private slots:
  void on_actionEasy_triggered(bool checked);
  void on_actionNormal_triggered(bool checked);
  void on_actionHard_triggered(bool checked);
  void on_action10x10_triggered(bool checked);
  void on_action20x20_triggered(bool checked);
  void on_action30x30_triggered(bool checked);
  void on_actionHints_triggered(bool checked);
  void on_actionScoreboard_triggered();

  void on_actionAbout_triggered();

private:
  Ui::MainWindow *ui;
  int size = 10;
};
#endif // MAINWINDOW_HPP
