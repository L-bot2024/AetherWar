#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Player : public QObject, public QGraphicsPixmapItem
{

public:
    Player();

    float mMoveSpeed; //移动速度

    QTimer mShootCooldownTimer; //射击冷却定时器
    bool mCanShoot = true; //是否可以射击

};

#endif // PLAYER_H
