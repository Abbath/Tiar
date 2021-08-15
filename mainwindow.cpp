#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "imagearea.hpp"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->pushButton, &QPushButton::clicked, ui->imagearea, &ImageArea::start);
  connect(ui->imagearea, &ImageArea::updateScore, this, &MainWindow::setScore);
  connect(ui->actionSound, &QAction::triggered, ui->imagearea, &ImageArea::setSound);
  ui->actionNormal->setChecked(true);
  ui->imagearea->start();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setScore(int s, int c)
{
  ui->points->setText("Score: " + QString::number(s) + " Moves left: " + QString::number(50-c));
}


void MainWindow::on_actionEasy_triggered(bool checked)
{
  if(checked){
    ui->actionNormal->setChecked(false);
    ui->actionHard->setChecked(false);
    ui->imagearea->setThreshold(900);
  }
}

void MainWindow::on_actionNormal_triggered(bool checked)
{
  if(checked){
    ui->actionEasy->setChecked(false);
    ui->actionHard->setChecked(false);
    ui->imagearea->setThreshold(1000);
  }
}

void MainWindow::on_actionHard_triggered(bool checked)
{
  if(checked){
    ui->actionNormal->setChecked(false);
    ui->actionEasy->setChecked(false);
    ui->imagearea->setThreshold(1200);
  }
}
