#include "enemy.h"

Enemy::Enemy(QPoint _pos,QPixmap _pixmap)
{
    this->mMoveSpeed = 1;
    this->mShootspeed = 1000;
    this->setPos(_pos);
    this->setPixmap(_pixmap);
}

//敌机移动函数
void Enemy::EnemyMove(QPoint _dir)
{
    this->moveBy(_dir.x()*mMoveSpeed,_dir.y()*mMoveSpeed);
}
