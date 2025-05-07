#include "bullet.h"

Bullet::Bullet(QPoint _pos,QPixmap _pixmap,int _type)
{
    this->setPos(_pos); //位置
    this->setPixmap(_pixmap); //图片
    this->mBulletType = _type; //类型
    mSpeed = 2; //速度

    this->setScale(0.6); //缩放
}

//子弹移动函数
void Bullet::BulletMove(QPoint _dir)
{
    this->moveBy(_dir.x()*mSpeed,_dir.y()*mSpeed);
}

//敌机子弹移动函数
void Bullet::EnemyBulletMove(QPoint _dir)
{
    this->moveBy(_dir.x()*mSpeed,_dir.y()*mSpeed);
}
