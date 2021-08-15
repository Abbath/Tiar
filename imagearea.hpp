#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include "types.hpp"
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMediaPlayer>

class ImageArea : public QWidget
{
  Q_OBJECT
  Board board;
  int i = 0;
  int j = 0;
  bool first_click = true;
  bool finish = false;
  bool played = false;
  bool sound = false;
  int counter = 0;
  int threshold = 1000;
  void stabilizeBoard();
  QMediaPlayer * player;
public:
  explicit ImageArea(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent*e);
  void mousePressEvent(QMouseEvent*e);
  void mouseReleaseEvent(QMouseEvent*e);
  int getScore();
  void setThreshold(int value);
signals:
  void updateScore(int s, int c);
public slots:
  void start();
  void setSound(bool enable);

};

#endif // IMAGEAREA_HPP
