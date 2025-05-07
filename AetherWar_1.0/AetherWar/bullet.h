#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>

class Bullet: public QGraphicsPixmapItem
{

public:
    //子弹类型
    enum BulletType
    {
        BT_Player, //玩家
        BT_Enemy //敌方
    };

    Bullet(QPoint _pos,QPixmap _pixmap,int _type);

    //玩家子弹移动函数
    void BulletMove(QPoint _dir = QPoint(1,0));
    //敌机子弹移动函数
    void EnemyBulletMove(QPoint _dir = QPoint(-1,0));

protected:
    int mBulletType;
    int mSpeed;



};

#endif // BULLET_H
