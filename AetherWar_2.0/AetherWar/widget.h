#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QtGlobal>
#include <QTime>
#include <QPushButton>
#include "player.h"
#include "bullet.h"
#include "enemy.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void BgMove(); //背景移动函数
    void PlaneEvent(); //飞机活动函数
    void PlaneShoot(); //飞机射击函数
    void EnemyCreate(); //敌机创建函数
    void EnemyShoot(Enemy* enemy); //敌机射击函数
    void Collision(); //碰撞检测函数(子弹与敌机)
    void PlayExplosion(QPoint pos); //爆炸特效函数
    void CheckPlayerHit(); //碰撞检测函数(子弹与玩家)
    void EndGame(); //游戏结束函数
    void UpdateScoreText(); //更新分数显示函数
    //按键
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    Ui::Widget *ui;

    QGraphicsView mGramView; //游戏视图
    QGraphicsScene mGameScene; //游戏场景
    QGraphicsScene mStartScene; //开始游戏场景
    QGraphicsScene mEndScene; //游戏结束场景


    //元素
    QGraphicsPixmapItem mBackground1; //背景1
    QGraphicsPixmapItem mBackground2; //背景2
    QGraphicsPixmapItem mBackground3; //背景3
    QGraphicsTextItem* mScoreText; //分数显示
    Player mPlane; //玩家飞机

    //退出游戏按钮
    QPushButton* exitBtn;

    //定时器
    QTimer* mBgTimer; //背景移动
    QTimer* mPlaneMoveTimer; //飞机移动
    QTimer* mBulletMoveTimer; //子弹移动
    QTimer* mEnemyCreateTimer; //敌机创建
    QTimer* mEnemyMoveTimer; //敌机移动
    QTimer* mEnemyShootTimer; //敌机发射
    QTimer* mEnemyBulletMoveTimer; //敌机子弹移动

    //容器
    QList<int> KeyList; //按键组合
    QList<Bullet*> mBulletList; //玩家子弹容器
    QList<Enemy*> mEnemyList; //敌机容器
    QList<Bullet*> mEnemyBulletList; //敌机子弹容器

    //游戏是否已经开始
    bool mGameStarted = false;

    //游戏时间
    QTime mGameStartTime;

    //游戏分数
    int mScore;





};
#endif // WIDGET_H
