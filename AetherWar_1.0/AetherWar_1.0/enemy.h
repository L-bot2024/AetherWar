#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>

class Enemy : public QGraphicsPixmapItem
{

public:

    Enemy(QPoint _pos,QPixmap _pixmap);

    //敌机移动函数
    void EnemyMove(QPoint _dir = QPoint(-1, 0));

    float mShootspeed; //射击速度
    float mMoveSpeed; //移动速度

};

#endif // ENEMY_H
