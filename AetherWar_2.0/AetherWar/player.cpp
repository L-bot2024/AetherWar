#include "player.h"
#include <QTimer>

Player::Player()
{

    this->setPixmap(QPixmap("://resource/jetthing2.png"));
    this->setPos(100,200);

    mMoveSpeed = 1;

    //射击冷却(子弹生成)
    mShootCooldownTimer.setInterval(300); //冷却时间：300ms
    mShootCooldownTimer.setSingleShot(true);
    connect(&mShootCooldownTimer, &QTimer::timeout, this, [=]() {
        mCanShoot = true; //冷却结束，允许再次射击
    });

}
