#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include "types.hpp"
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMap>

class ImageArea : public QWidget
{
  Q_OBJECT
  Board board;
  Board old_board;
  Board changed_board;
  int i = 0;
  int j = 0;
  bool first_click = true;
  bool finish = false;
  bool played = false;
  bool sound = false;
  bool hints = false;
  int counter = 0;
  int threshold = 1000;
  double thr = 1.0;
  void stabilizeBoard();
  void stepBoard();
  void finished();
  QList<QPair<QString, int>> scoreboard;
  QMediaPlayer * player;
  int timer_id = 0;
public:
  explicit ImageArea(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent*e);
  void mousePressEvent(QMouseEvent*e);
  void mouseReleaseEvent(QMouseEvent*e);
  void timerEvent(QTimerEvent *e);
  int getScore();
  void setThreshold(double thr);
  int getThreshold();
  void setHints(bool newHints);

  const QList<QPair<QString, int>> &getScoreboard() const;
  void setScoreboard(const QList<QPair<QString, int>> &newScoreboard);

signals:
  void updateScore(int s, int c);
public slots:
  void start(int size);
  void setSound(bool enable);

};

#endif // IMAGEAREA_HPP
