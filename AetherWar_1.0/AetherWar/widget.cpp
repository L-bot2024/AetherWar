#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(1024,512);
    this->setFocusPolicy(Qt::StrongFocus);

    //设置大小
    mGramView.setSceneRect(QRect(0,0,1024,512));
    mGameScene.setSceneRect(QRect(0,0,1024,512));
    mStartScene.setSceneRect(QRect(0,0,1024,512));

    //载入图片
    mStartScene.addPixmap(QPixmap(":/resource/cover.png"));
    mBackground1.setPixmap(QPixmap("://resource/background.png"));
    mBackground2.setPixmap(QPixmap("://resource/background.png"));
    mBackground3.setPixmap(QPixmap("://resource/background.png"));
    mBackground2.setPos(1024,0);
    mPlane.setPixmap(QPixmap("://resource/jetthing2.png"));
    mPlane.setPos(100,100);

    //添加到场景
    mGameScene.addItem(&mBackground1);
    mGameScene.addItem(&mBackground2);
    mGameScene.addItem(&mPlane);
    mEndScene.addItem(&mBackground3);

    //设置场景
    mGramView.setScene(&mStartScene);
    mGramView.setParent(this);
    mGramView.show();

    //结束按钮
    exitBtn = new QPushButton("Exit Game");
    exitBtn->setStyleSheet("background-color: #696969; color: white; font-size: 18px;");
    exitBtn->resize(180,40);
    mEndScene.addWidget(exitBtn)->setPos(400, 320);
    connect(exitBtn, &QPushButton::clicked, this, &Widget::close);

    //分数显示
    mScoreText = new QGraphicsTextItem("Score: 0");
    mScoreText->setFont(QFont("Arial", 20));
    mScoreText->setDefaultTextColor(Qt::white);
    mScoreText->setPos(0, 0);
    mGameScene.addItem(mScoreText);

    //创建定时器
    mBgTimer = new QTimer(this);
    mPlaneMoveTimer = new QTimer(this);
    mBulletMoveTimer = new QTimer(this);
    mEnemyCreateTimer = new QTimer(this);
    mEnemyMoveTimer = new QTimer(this);
    mEnemyShootTimer = new QTimer(this);
    mEnemyBulletMoveTimer = new QTimer(this);


    //建立连接(信号和槽)
    //背景移动
    connect(mBgTimer,&QTimer::timeout,this,&Widget::BgMove);
    //玩家飞机移动
    connect(mPlaneMoveTimer,&QTimer::timeout,this,&Widget::PlaneEvent);
    //玩家子弹移动
    connect(mBulletMoveTimer,&QTimer::timeout,[this](){
        for(auto bullet : mBulletList)
        {
            bullet->BulletMove();
        }
        //碰撞检测
        Widget::Collision();
        Widget::CheckPlayerHit();
    });
    //敌机创建
    connect(mEnemyCreateTimer,&QTimer::timeout,this,&Widget::EnemyCreate);
    //敌机移动
    connect(mEnemyMoveTimer,&QTimer::timeout,[this](){
        for(auto enemy : mEnemyList)
        {
            enemy->EnemyMove();
        }
    });
    //敌机射击
    connect(mEnemyShootTimer,&QTimer::timeout,[this](){
        for (Enemy* enemy : mEnemyList) {
            EnemyShoot(enemy);
        }
    });
    //敌机子弹移动
    connect(mEnemyBulletMoveTimer,&QTimer::timeout,[this](){
        for(auto bullet : mEnemyBulletList)
        {
            bullet->EnemyBulletMove();
        }
    });
}

//背景移动函数
void Widget::BgMove()
{
    mBackground1.moveBy(-2,0);
    mBackground2.moveBy(-2,0);

    if(mBackground1.x()<=-1024)
        mBackground1.setX(1024);
    else if(mBackground2.x()<=-1024)
        mBackground2.setX(1024);
}

//飞机按键控制
void Widget::PlaneEvent()
{
    for(int keycode : KeyList)
    {
        switch(keycode)
        {
        case Qt::Key_W:
            mPlane.moveBy(0, -mPlane.mMoveSpeed);
            break;
        case Qt::Key_S:
            mPlane.moveBy(0, mPlane.mMoveSpeed);
            break;
        case Qt::Key_A:
            mPlane.moveBy(-mPlane.mMoveSpeed, 0);
            break;
        case Qt::Key_D:
            mPlane.moveBy(mPlane.mMoveSpeed, 0);
            break;
        case Qt::Key_Space:
            if (mPlane.mCanShoot)
            {
                PlaneShoot(); //射击
                //重置冷却
                mPlane.mCanShoot = false;
                mPlane.mShootCooldownTimer.start();
            }
            break;

        default:
            break;
        }
    }
}

//按压按键移入按键容器
void Widget::keyPressEvent(QKeyEvent *event)
{
    //点击任意键开始游戏
    if (!mGameStarted)
    {
        mGameStarted = true;
        mScore = 0;
        mGramView.setScene(&mGameScene);
        //开启定时器
        mBgTimer->start(10);
        mPlaneMoveTimer->start(7);
        mBulletMoveTimer->start(3);
        mEnemyCreateTimer->start(2000);
        mEnemyMoveTimer->start(15);
        mEnemyShootTimer->start(2000);
        mEnemyBulletMoveTimer->start(10);
        mGameStartTime = QTime::currentTime();
        return;
    }

    //按压按键添加按键组合
    switch (event->key())
    {
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
    case Qt::Key_Space:
        KeyList.append(event->key());
        break;
    }

    //设置边界(控制飞机不出界并把活动范围限制在左半边)
    if(mPlane.x()<0)
    {
        mPlane.setX(0);
    }
    if(mPlane.y()<-10)
    {
        mPlane.setY(-10);
    }
    if(mPlane.x()>this->width()/2-mPlane.pixmap().width())
    {
        mPlane.setX(this->width()/2-mPlane.pixmap().width());
    }
    if(mPlane.y()>this->height()-mPlane.pixmap().height()+10)
    {
        mPlane.setY(this->height()-mPlane.pixmap().height()+10);
    }


}

//松开按键移除按键容器
void Widget::keyReleaseEvent(QKeyEvent* event)
{
    if(KeyList.contains(event->key()))
    {
        KeyList.removeOne(event->key());
    }
}

//玩家飞机射击(子弹创建)函数
void Widget::PlaneShoot()
{
    //创建子弹
    QPixmap bulletImg(":/resource/bullet.png");
    QPoint pos(mPlane.x()+23,mPlane.y()+40);
    Bullet* bullet = new Bullet(pos,bulletImg,Bullet::BT_Player);

    //添加到场景
    mGameScene.addItem(bullet);

    //添加到子弹容器
    mBulletList.append(bullet);
}

//敌机飞机射击(子弹创建)函数
void Widget::EnemyShoot(Enemy* enemy)
{
    //创建子弹
    QPixmap bulletImg(":/resource/bullet_1.png");
    QPoint pos(enemy->x(), enemy->y());
    Bullet* bullet = new Bullet(pos,bulletImg,Bullet::BT_Enemy);

    //添加到场景
    mGameScene.addItem(bullet);

    //添加到子弹容器
    mEnemyBulletList.append(bullet);
}

//敌机创建函数
void Widget::EnemyCreate()
{
    QPixmap pixmap(":/resource/enemy_2.png");
    int randY = rand()%512-pixmap.height();
    Enemy* enemy = new Enemy(QPoint(1024,randY),pixmap);

    //添加到场景
    mGameScene.addItem(enemy);

    //添加到容器
    mEnemyList.append(enemy);
}

//爆炸特效函数
void Widget::PlayExplosion(QPoint pos)
{
    //加载所有爆炸帧
    QList<QPixmap> explosionFrames = {
        QPixmap(":/resource/explosion1.png"),
        QPixmap(":/resource/explosion2.png"),
        QPixmap(":/resource/explosion3.png"),
        QPixmap(":/resource/explosion4.png")
    };

    //创建图元并加入场景
    QGraphicsPixmapItem* explosion = new QGraphicsPixmapItem(explosionFrames[0]);
    explosion->setPos(pos.rx(),pos.ry()-20);
    mGameScene.addItem(explosion);

    //创建帧计数器和定时器
    QTimer* animTimer = new QTimer(this);
    int* frameIndex = new int(1); //从第1帧开始
    animTimer->start(80); //每帧80ms

    connect(animTimer, &QTimer::timeout, this, [=]() mutable {
        if (*frameIndex < explosionFrames.size())
        {
            explosion->setPixmap(explosionFrames[*frameIndex]);
            (*frameIndex)++;
        }
        else
        {
            //动画结束，释放内存
            animTimer->stop();
            mGameScene.removeItem(explosion);
            delete explosion;
            delete frameIndex;
            animTimer->deleteLater();
        }
    });

}

//碰撞检测函数(敌机与子弹)
void Widget::Collision()
{
    QList<Bullet*> bulletsToRemove;
    QList<Enemy*> enemiesToRemove;

    for (Bullet* bullet : mBulletList)
    {
        for (Enemy* enemy : mEnemyList)
        {
            if (bullet->collidesWithItem(enemy))
            {
                PlayExplosion(enemy->pos().toPoint());

                //加分
                mScore++;

                //更新分数显示
                UpdateScoreText();

                bulletsToRemove.append(bullet);
                enemiesToRemove.append(enemy);
                break;
            }
        }
    }

    // 实际删除子弹和敌机
    for (Bullet* bullet : bulletsToRemove)
    {
        mGameScene.removeItem(bullet);
        mBulletList.removeOne(bullet);
        delete bullet;
    }

    for (Enemy* enemy : enemiesToRemove)
    {
        mGameScene.removeItem(enemy);
        mEnemyList.removeOne(enemy);
        delete enemy;
    }
}

//更新分数显示函数
void Widget::UpdateScoreText()
{
    mScoreText->setPlainText(QString("Score: %1").arg(mScore));
}

//碰撞检测函数(子弹与玩家)
void Widget::CheckPlayerHit()
{
    for (int i=0;i< mEnemyBulletList.size();i++)
    {
        if (mEnemyBulletList[i]->collidesWithItem(&mPlane))
        {
            // 播放爆炸
            PlayExplosion(mPlane.pos().toPoint());

            // 停止游戏
            EndGame();
            break;
        }
    }
}

//游戏结束函数
void Widget::EndGame()
{
    // 停止所有定时器
    if (mBgTimer) mBgTimer->stop();
    if (mPlaneMoveTimer) mPlaneMoveTimer->stop();
    if (mBulletMoveTimer) mBulletMoveTimer->stop();
    if (mEnemyShootTimer) mEnemyShootTimer->stop();
    if (mEnemyBulletMoveTimer) mEnemyBulletMoveTimer->stop();

    // 计算游戏时间
    int seconds = mGameStartTime.secsTo(QTime::currentTime());

    // 显示分数与时间
    QGraphicsTextItem* scoreText = mEndScene.addText(
        QString("Game Over\nScore: %1\nTime: %2s").arg(mScore).arg(seconds),
        QFont("Arial", 30));
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setPos(400, 100);

    // 切换到结束场景
    mGramView.setScene(&mEndScene);

}

Widget::~Widget()
{
    delete ui;
}


