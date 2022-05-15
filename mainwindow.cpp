#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "imagearea.hpp"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->pushButton, &QPushButton::clicked, [&]() { ui->imagearea->start(size); });
  connect(ui->imagearea, &ImageArea::updateScore, this, &MainWindow::setScore);
  connect(ui->actionSound, &QAction::triggered, ui->imagearea, &ImageArea::setSound);
  ui->actionNormal->setChecked(true);
  ui->action10x10->setChecked(true);
  ui->imagearea->start(10);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setScore(int s, int c)
{
  ui->points->setText("Score: " + QString::number(s) + "/" + QString::number(ui->imagearea->getThreshold()) + " Moves left: " + QString::number(50-c));
}


void MainWindow::on_actionEasy_triggered(bool checked)
{
  if(checked){
    ui->actionNormal->setChecked(false);
    ui->actionHard->setChecked(false);
    ui->imagearea->setThreshold(0.9);
  }
}

void MainWindow::on_actionNormal_triggered(bool checked)
{
  if(checked){
    ui->actionEasy->setChecked(false);
    ui->actionHard->setChecked(false);
    ui->imagearea->setThreshold(1);
  }
}

void MainWindow::on_actionHard_triggered(bool checked)
{
  if(checked){
    ui->actionNormal->setChecked(false);
    ui->actionEasy->setChecked(false);
    ui->imagearea->setThreshold(1.2);
  }
}

void MainWindow::on_action10x10_triggered(bool checked)
{
  if(checked)	{
    ui->action20x20->setChecked(false);
    ui->action30x30->setChecked(false);
    size = 10;
    ui->imagearea->start(size);
  }
}

void MainWindow::on_action20x20_triggered(bool checked)
{
    if(checked)	{
      ui->action10x10->setChecked(false);
      ui->action30x30->setChecked(false);
      size = 20;
      ui->imagearea->start(size);
    }
}

void MainWindow::on_action30x30_triggered(bool checked)
{
    if(checked)	{
      ui->action10x10->setChecked(false);
      ui->action20x20->setChecked(false);
      size = 30;
      ui->imagearea->start(size);
    }
}

void MainWindow::on_actionHints_triggered(bool checked)
{
  ui->imagearea->setHints(checked);
  update();
}

