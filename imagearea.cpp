#include "imagearea.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QThread>
#include <QRect>
#include <QInputDialog>

void ImageArea::setThreshold(double _thr)
{
    thr = _thr;
}

int ImageArea::getThreshold()
{
    return threshold;
}

void ImageArea::setHints(bool newHints)
{
  hints = newHints;
}

void ImageArea::stabilizeBoard() {
  auto old_board = board;
  do {
    old_board = board;
    board.remove_trios();
    board.fill_up();
  }while(!(old_board == board));
  board.match_patterns();
}

void ImageArea::finished()
{
    QString user = QInputDialog::getText(this, "Provide info", "Username");
    if(!user.isEmpty()){
        scoreboard.append(qMakePair(user, getScore()));
    }
}

const QList<QPair<QString, int>> &ImageArea::getScoreboard() const
{
    return scoreboard;
}

void ImageArea::setScoreboard(const QList<QPair<QString, int>> &newScoreboard)
{
    scoreboard = newScoreboard;
}

ImageArea::ImageArea(QWidget *parent) : QWidget(parent) , board{10, 10} {
  player = new QMediaPlayer;
}

void ImageArea::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  painter.fillRect(this->rect(), Qt::gray);
  int w = this->width() / board.width();
  int h = this->height() / board.height();
  for(int j = 0; j < board.width(); ++j){
    for(int i = 0; i < board.height(); ++i){
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
      if(hints && board.is_matched(i, j)){
        QColor bi = QColor(b);
        bi.setRed(255-bi.red());
        bi.setGreen(255-bi.green());
        bi.setBlue(255-bi.blue());
        painter.fillRect(QRect(j * w, i * h, w , h), bi);
      }
      painter.setPen(b);
      painter.setBrush(QBrush(b, Qt::SolidPattern));

      switch (board.at(i, j)) {
        case 1: {
          painter.translate(j * w + w / 2.0,  i * h + h / 2.0);
          painter.rotate(45);
          painter.translate(-(j * w + w / 2.0),  -(i * h + h / 2.0));
          painter.drawEllipse(QRect(j * w + w / 4, i * h, w - h / 3 , h - 2));
          painter.translate(j * w + w / 2.0,  i * h + h / 2.0);
          painter.rotate(-45);
          painter.translate(-(j * w + w / 2.0),  -(i * h + h / 2.0));
          break;
        }
        case 2: {
          painter.fillRect(QRect(j * w + 6, i * h + 6, w - 10 , h - 10), b);
          break;
        }
        case 4: {
          painter.drawEllipse(QRect(j * w + w / 4, i * h + 2, w - w / 2 , h - 4));
          break;
        }
        case 5: {
          QPainterPath path;
          QPolygon polygon;
          polygon << QPoint(j * w + w / 3, i * h + 2)
                  << QPoint(j * w + 2 * w / 3, i * h + 2)
                  << QPoint((j+1) * w - 4, i * h + h / 2)
                  << QPoint(j * w + 2 * (w / 3), (i+1) * h - 4)
                  << QPoint(j * w + w / 3, (i+1) * h - 4)
                  << QPoint(j * w + 2, i * h + h / 2);
          path.addPolygon(polygon);
          painter.fillPath(path, b);
          break;
        }
        default: {
          painter.drawEllipse(QRect(j * w + 4, i * h + 4, w - 8 , h - 8));
          break;
        }
      }
    }
  }
  if(finish){
    painter.setFont(QFont(painter.font().family(), 80));
    painter.setPen(Qt::black);
    if(getScore() >= threshold){
      painter.drawText(this->rect(), Qt::AlignCenter, "YOU WON!");
      if(sound && !played){
        player->setMedia(QUrl::fromLocalFile("g.mp3"));
      }
    }else{
      painter.drawText(this->rect(), Qt::AlignCenter, "YOU LOST!");
      if(sound && !played){
        player->setMedia(QUrl::fromLocalFile("a.mp3"));
      }
    }
    if(sound && !played){
      player->setVolume(50);
      player->play();
      played = true;
    }
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
  int y = position.x() / (this->width() / board.width());
  int x = position.y() / (this->height() / board.height());
  if(first_click){
    this->i = x;
    this->j = y;
    first_click = false;
  }else{
    first_click = true;
    if(!((abs(x - this->i) == 1) ^ (abs(y - this->j) == 1))){
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
      if(sound){
        player->setMedia(QUrl::fromLocalFile("p.mp3"));
        player->setVolume(50);
        player->play();
      }
    }
    if(counter == 50){
      finish = true;
      finished();
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
  int y = position.x() / (this->width() / board.width());
  int x = position.y() / (this->height() / board.height());
  if (x == this->i && y == this->j){
    return;
  }
  if(!first_click){
    first_click = true;
    if(!((abs(x - this->i) == 1) ^ (abs(y - this->j) == 1))){
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
      if(sound){
        player->setMedia(QUrl::fromLocalFile("p.mp3"));
        player->setVolume(50);
        player->play();
      }
    }
    if(counter == 50){
      finish = true;
      finished();
    }
    update();
    emit updateScore(getScore(), counter);
  }
  e->accept();
}

int ImageArea::getScore() {
  return board.trios_removed;
}

void ImageArea::start(int size) {
  switch (size) {
  case 10:
      threshold = 1000 * thr;
      break;
  case 20:
      threshold = 4000 * thr;
      break;
  case 30:
      threshold = 128000 * thr;
      break;
  default:
      threshold = 1000;
      break;
  }
  finish = false;
  played = false;
  counter = 0;
  board = Board{size, size};
  board.fill();
  stabilizeBoard();
  board.trios_removed = 0;
  update();
  emit updateScore(0, 0);
}

void ImageArea::setSound(bool enable)
{
  sound = enable;
}
