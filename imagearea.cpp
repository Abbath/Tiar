#include "imagearea.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QThread>
#include <QRect>

void ImageArea::setThreshold(int value)
{
  threshold = value;
}

void ImageArea::stabilizeBoard() {
  auto old_board = board;
  do {
    old_board = board;
    board.remove_trios();
    board.fill_up();
  }while(!(old_board == board));
}

ImageArea::ImageArea(QWidget *parent) : QWidget(parent) {
  player = new QMediaPlayer;
}

void ImageArea::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  for(int j = 0; j < 10; ++j){
    for(int i = 0; i < 10; ++i){
      QColor b;
      switch (board.at(i, j)) {
        case 1:
          b = Qt::red;
          break;
        case 2:
          b = Qt::green;
          break;
        case 3:
          b = Qt::blue;
          break;
        case 4:
          b = QColor(255, 165, 0);
          break;
        case 5:
          b = Qt::darkMagenta;
          break;
        default:
          b = Qt::black;
          break;
      }
      painter.setPen(b);
      painter.setBrush(QBrush(b, Qt::SolidPattern));
      switch (board.at(i, j)) {
        case 1: {
          painter.translate(j * this->width() / 10 + 32,  i * this->height() / 10 + 32);
          painter.rotate(45);
          painter.translate(-(j * this->width() / 10 + 32),  -(i * this->height() / 10 + 32));
          painter.drawEllipse(QRect(j * this->width() / 10 + 10, i * this->height() / 10, this->width() / 10 - 22 , this->height() / 10 - 2));
          painter.translate(j * this->width() / 10 + 32,  i * this->height() / 10 + 32);
          painter.rotate(-45);
          painter.translate(-(j * this->width() / 10 + 32),  -(i * this->height() / 10 + 32));
          break;
        }
        case 2: {
          painter.fillRect(QRect(j * this->width() / 10 + 4, i * this->height() / 10 + 4, this->width() / 10 - 8 , this->height() / 10 - 8), b);
          break;
        }
        case 4: {
          painter.drawEllipse(QRect(j * this->width() / 10 + 10, i * this->height() / 10 + 2, this->width() / 10 - 22 , this->height() / 10 - 4));
          break;
        }
        case 5: {
          QPainterPath path;
          QPolygon polygon;
          polygon << QPoint(j * this->width() / 10 + this->width() / 30, i * this->height() / 10 + 2)
                  << QPoint(j * this->width() / 10 + 2 * (this->width() / 30), i * this->height() / 10 + 2)
                  << QPoint((j+1) * this->width() / 10 - 4, i * this->height() / 10 + this->height() / 20)
                  << QPoint(j * this->width() / 10 + 2 * (this->width() / 30), (i+1) * this->height() / 10 - 4)
                  << QPoint(j * this->width() / 10 + this->width() / 30, (i+1) * this->height() / 10 - 4)
                  << QPoint(j * this->width() / 10 + 2, i * this->height() / 10 + this->height() / 20);
          path.addPolygon(polygon);
          painter.fillPath(path, b);
          break;
        }
        default: {
          painter.drawEllipse(QRect(j * this->width() / 10 + 4, i * this->height() / 10 + 4, this->width() / 10 - 8 , this->height() / 10 - 8));
          break;
        }
      }
    }
  }
  if(finish){
    painter.setFont(QFont(painter.font().family(), 96));
    painter.setPen(Qt::black);
    if(getScore() >= threshold){
      painter.drawText(this->rect(), Qt::AlignCenter, "YOU WON!");
      player->setMedia(QUrl::fromLocalFile("g.m4a"));
    }else{
      painter.drawText(this->rect(), Qt::AlignCenter, "YOU LOST!");
      player->setMedia(QUrl::fromLocalFile("a.m4a"));
    }
    player->setVolume(50);
    player->play();
  }
  e->accept();
}

void ImageArea::mousePressEvent(QMouseEvent *e){
  if(finish){
    update();
    e->accept();
    return;
  }
  auto position = e->pos();
  int y = position.x() / (this->width() / 10);
  int x = position.y() / (this->height() / 10);
  if(first_click){
    this->i = x;
    this->j = y;
    first_click = false;
  }else{
    first_click = true;
    if(!(abs(x - this->i) == 1 ^ abs(y - this->j) == 1)){
      return;
    }
    auto ob = board;
    auto tmp = board.at(x,y);
    board.at(x, y) = board.at(this->i, this->j);
    board.at(this->i, this->j) = tmp;
    auto nb = board;
    stabilizeBoard();
    if(nb == board){
      board = ob;
    }else{
      counter += 1;
      player->setMedia(QUrl::fromLocalFile("p.mp3"));
      player->setVolume(50);
      player->play();
    }
    if(counter == 50){
      finish = true;
    }
    update();
    emit updateScore(getScore(), counter);
  }
  e->accept();
}

void ImageArea::mouseReleaseEvent(QMouseEvent *e){
  if(finish){
    update();
    e->accept();
    return;
  }
  auto position = e->pos();
  int y = position.x() / (this->width() / 10);
  int x = position.y() / (this->height() / 10);
  if (x == this->i && y == this->j){
    return;
  }
  if(!first_click){
    first_click = true;
    if(!(abs(x - this->i) == 1 ^ abs(y - this->j) == 1)){
      return;
    }
    auto ob = board;
    auto tmp = board.at(x,y);
    board.at(x, y) = board.at(this->i, this->j);
    board.at(this->i, this->j) = tmp;
    auto nb = board;
    stabilizeBoard();
    if(nb == board){
      board = ob;
    }else{
      counter += 1;
      player->setMedia(QUrl::fromLocalFile("p.mp3"));
      player->setVolume(50);
      player->play();
    }
    if(counter == 50){
      finish = true;
    }
    update();
    emit updateScore(getScore(), counter);
  }
  e->accept();
}

int ImageArea::getScore() {
  return board.trios_removed;
}

void ImageArea::start() {
  finish = false;
  counter = 0;
  board.fill();
  stabilizeBoard();
  board.trios_removed = 0;
  update();
  emit updateScore(0, 0);
}
