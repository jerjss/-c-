#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include"tool.h"
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

enum Curtain
{
	WIDTH = 480,			//窗口的宽和高与背景图相等
	HEIGHT = 860,
	BULLET_NUM = 20,			//子弹数量	
	ENEMY_NUM = 12,			//敌机数量
	SMALL,
	BIG,
};
//菜单
IMAGE Start;
IMAGE Over;
IMAGE Setting;

//游戏内图片
IMAGE	bk;
IMAGE img_player1;
IMAGE img_player2;
IMAGE img_bullet1;
IMAGE img_bullet2;
IMAGE img_enemy1;
IMAGE img_enemy2;
IMAGE img_boom1;

struct Bullet {
	int x;          // 子弹的 x 坐标
	int y;          // 子弹的 y 坐标
	int speed;      // 子弹的速度
	bool live;      // 子弹的生存状态
	int type;       // 子弹的类型
};
struct button		//按钮结构体
{
	int x;
	int y;
	int width;
	int height;
	COLORREF color;		//颜色
	char* pText;				//文字
};
struct Plane//结构
{
	int x;
	int y;
	bool live;//是否存活  
	int width;	
	int height;   //宽度和高度
	int hp;			//血量
	int type;		//种类
	DWORD lastHitTime;
	Bullet bullet[BULLET_NUM];      // 玩家1的子弹数组
	Bullet bullet2[BULLET_NUM];     // 玩家2的子弹数组
}player1,player2,bullet[BULLET_NUM],bullet2[BULLET_NUM],enemy[ENEMY_NUM],boom1[ENEMY_NUM];
const int HIT_COOLDOWN = 1000;
bool gameOver = false;			 //标志是否游戏结束
bool reStart = false;					//标志是否重开游戏

void loadImg() {
	//加载菜单等相关图片
	loadimage(&Start, "./menu/start.jpg", getwidth(), getheight());
	loadimage(&Over, "./menu/over.jpg", getwidth(), getheight());
	loadimage(&Setting, "./menu/setting.jpg", getwidth(), getheight());

	//加载背景图片
	loadimage(&bk, "./images//background.jpg");
	//加载玩家图片
	loadimage(&img_player1, "./images/playerShip1_blue.png");
	loadimage(&img_player2, "./images/playerShip2_orange.png");

	//加载子弹		
	loadimage(&img_bullet1, "./images/laserBlue03.png");
	loadimage(&img_bullet2, "./images/laserRed05.png");
	//加载敌机
	loadimage(&img_enemy1, "./images/enemyBlack1.png");
	loadimage(&img_enemy2, "./images/super enemy1.png");
	//加载爆炸图片
	loadimage(&img_boom1, "./images/boom1.png");
}
void gameDraw() {
	//绘制
	loadImg();
	putimage(0, 0, &bk);
	if (player1.live) {
		drawImg(player1.x, player1.y, &img_player1);
	}
	if (player2.live) {
		drawImg(player2.x, player2.y, &img_player2);
	}
	//绘制爆炸
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (boom1[i].live) {
			drawImg(boom1[i].x, boom1[i].y, &img_boom1);
		}
	}
	//绘制子弹
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet[i].live) {
			drawImg(bullet[i].x, bullet[i].y, &img_bullet1);
		}
	}
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet2[i].live) {
			drawImg(bullet2[i].x, bullet2[i].y, &img_bullet2);
		}
	}
	//绘制敌机
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live) {
			if (enemy[i].type == BIG) {
				drawImg(enemy[i].x, enemy[i].y, &img_enemy2);
			}
			else
			{
				drawImg(enemy[i].x, enemy[i].y, &img_enemy1);
			}
		}
	}
}
void menuDraw() {
	loadImg();
	putimage(0, 0, &Start);
}
void overDraw() {
	loadImg();
	putimage(0, 0, &Over);
}

bool Timer(int ms, int id) {					//计时器
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}

	return false;
}
bool Timer2(int ms, int id) {					//计时器
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}

	return false;
}

void enemyHp(int i) {
	int flag = rand() % 10;
	if (flag >=0 && flag<= 1) {		//0~9
		enemy[i].type = BIG;
		enemy[i].hp = 6;
		enemy[i].width = 185;
		enemy[i].height = 93;
	}
	else
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 83;
		enemy[i].height = 54;
	}
}
void gameInit() {				//游戏的初始化
	//角色初始化
	//初始化子弹
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].live = false;			//初始状态下并不存在	
	}
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet2[i].x = 0;
		bullet2[i].y = 0;
		bullet2[i].live = false;
		
	}

	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].live = false;
		enemyHp(i);

	}

}
void Player1init() {
	//角色初始化
	player1.x = WIDTH / 2 - 153;
	player1.y = HEIGHT - 120;
	player1.live = true;
	player1.hp = 10;
	player1.width = 60;
	player1.height = 35;
	player1.lastHitTime = 0; // 初始化上次受伤时间
}
void Player2init() {
	player2.x = WIDTH / 2 + 71;
	player2.y = HEIGHT - 120;
	player2.live = true;
	player2.hp = 10;
	player2.width = 60;
	player2.height = 35;
	player2.lastHitTime = 0; // 初始化上次受伤时间
}

void createBullet() {
	if(player1.live){
	for (int i = 0; i < BULLET_NUM; i++) {
			if (!bullet[i].live) {
				mciSendString("play dan from 0", NULL, 0, NULL);
				bullet[i].x = player1.x + 36;
				bullet[i].y = player1.y;
				bullet[i].live = true;
				break;
			}
		}
	}
}
void createBullet2() {
	if(player2.live){
		for (int i = 0; i < BULLET_NUM; i++) {
			if (!bullet2[i].live) {
				mciSendString("play dan from 0", NULL, 0, NULL);
				bullet2[i].x = player2.x + 36;
				bullet2[i].y = player2.y;
				bullet2[i].live = true;
				break;
			}
		}
	}
}
void createBoom() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!boom1[i].live) {
			boom1[i].live = true;
			break;
		}
	}
}
void checkBoomlive() {		//检查爆炸是否存在
	for (int i=0; i < ENEMY_NUM; i++) {
		if (boom1[i].live) {
			boom1[i].live = false;
			
		}
	}
}
void CreateEnemy() {			//产生敌机，与子弹的生成类似
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live) {
			enemy[i].live = true;
			enemyHp(i);
			enemy[i].x = rand() % (WIDTH - 100);
			enemy[i].y = -100;
			break;
		}
	}
}

void Playermove(int speed) {
	// 玩家1的移动控制
	if (GetAsyncKeyState('W')) {
		if (player1.y > 0) {
			player1.y -= speed;
		}
	}
	if (GetAsyncKeyState('S')) {
		if (player1.y < HEIGHT - 82) {
			player1.y += speed;
		}
	}
	if (GetAsyncKeyState('A')) {
		if (player1.x > -41) {
			player1.x -= speed;
		}
	}
	if (GetAsyncKeyState('D')) {
		if (player1.x < WIDTH - 41) {
			player1.x += speed;
		}
	}
	if (GetAsyncKeyState(VK_SPACE) && Timer(150, 1)) {
		createBullet();
}

	// 玩家2的移动控制
	if (GetAsyncKeyState(VK_UP)) {
		if (player2.y > 0) {
			player2.y -= speed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		if (player2.y < HEIGHT - 82) {
			player2.y += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		if (player2.x > -41) {
			player2.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		if (player2.x < WIDTH - 41) {
			player2.x += speed;
		}
	}
	if (GetAsyncKeyState('P') && Timer2(150, 1)) {
		mciSendString("play dan from 0", NULL, 0, NULL);
		createBullet2();
	}
}
void BulletMove(int speed) {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet[i].live != 0) {

			bullet[i].y -= speed;
			if (bullet[i].y < 0) {
				
				bullet[i].live = false;
			}
		}
	}
}
void Bullet2Move(int speed) {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet2[i].live != 0) {

			bullet2[i].y -= speed;
			if (bullet2[i].y < 0) {
				
				bullet2[i].live = false;
			}
		}
	}
}
void EnemyMove(int speed) {		//敌机的移动
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live != 0) {

			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT+100) {
				enemy[i].live = false;
			}
		}
	}
	checkBoomlive();
}

int CheckHit_Bullet(struct Plane p1, struct Plane p2) {
	if (p1.x > p2.x && p1.x<p2.x + p2.width &&
		p1.y>p2.y && p1.y < p2.y + p2.height) {
		return 1;
	}
	return 0;
}
bool CheckHit_Player(struct Plane p1, struct Plane p2) {
	if (p1.x<p2.x + p2.width && p1.x + p1.width>p2.x &&
		p1.y<p2.y + p2.height && p1.y + p1.height>p2.y) {
		return true;
	}
	return false;
}
void Gameover() {
	if (!player1.live && !player2.live) {
		gameOver = true;
	}
}
void Fight() {
	DWORD currentTime = clock(); // 获取当前时间
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live)
			continue;
		for (int j = 0; j < BULLET_NUM; j++) {
			if (!bullet[j].live)
				continue;
			if (CheckHit_Bullet(bullet[j], enemy[i])) {
				mciSendString("play hit from 0", NULL, 0, NULL);
				bullet[j].live = false;
				enemy[i].hp--;
			}
		}
		for (int j = 0; j < BULLET_NUM; j++) {
			if (!bullet2[j].live)
				continue;
			if (CheckHit_Bullet(bullet2[j], enemy[i])) {
				mciSendString("play hit from 0", NULL, 0, NULL);
				bullet2[j].live = false;
				enemy[i].hp--;
			}
		}
		if (CheckHit_Player(player1, enemy[i])) {
			if (currentTime - player1.lastHitTime > HIT_COOLDOWN) {
				player1.hp--;
				player1.lastHitTime = currentTime; // 更新上次受伤时间
				printf("Player1 hit by enemy %d, HP: %d\n", i, player1.hp);
				if (player1.hp <= 0) {
					player1.hp = 0;
					player1.live = false;
				}
			}
		}
		if (CheckHit_Player(player2, enemy[i])) {
			if (currentTime - player2.lastHitTime > HIT_COOLDOWN) {
				player2.hp--;
				player2.lastHitTime = currentTime; // 更新上次受伤时间
				if (player2.hp <= 0) {
					player2.hp = 0;
					player2.live = false;
				}
			}
		}
		if (enemy[i].hp <= 0) {//如果敌机血量小于0则不再绘制敌机
			for (int k = 0; k < ENEMY_NUM; k++) {
				if (!boom1[k].live) {
					createBoom();
					boom1[k].x = enemy[i].x;
					boom1[k].y = enemy[i].y;
					enemy[i].live = false;
					break;
				}
				
			}
		}
	}

	Gameover();
}

//鼠标是否点击了某个区域
bool isInRect(int x, int y, int w, int h, int mx, int my) {
	if (mx > x && mx<x + w &&
		my > y && my < y + h) {
		return true;
	}
	return false;
}

//游戏程序
void gameRun() {
	gameDraw();
	FlushBatchDraw();
	Playermove(7);
	BulletMove(20);
	Bullet2Move(20);
	if (Timer(800, 0)) {
		CreateEnemy();
	}
	if (Timer(5, 2)) {
		EnemyMove(4);
	}
	Fight();
}



int main() {
	ExMessage msg;
	//手动创建一个图形窗口
	initgraph(WIDTH,HEIGHT);		//宽和高
	mciSendString("open images/ZHANDOU.mp3 alias bgm", NULL, 0, NULL);
	mciSendString("open images/PENG.wav alias hit", NULL, 0, NULL);
	mciSendString("open images/zidan.wav alias dan", NULL, 0, NULL);
	mciSendString("open hit from 0", NULL, 0, NULL);
	mciSendString("play bgm repeat from 0", NULL, 0, NULL);
	mciSendString("setaudio bgm volume to 1000", NULL, 0, NULL);
	gameInit();
	Player1init();
	Player2init();

	loadImg();
	while (1)
	{
		menuDraw();
		while (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (isInRect(150, 310, 170, 50, msg.x, msg.y)) {
					// 处理点击"start"区域的逻辑
					printf("Start clicked\n");
					while (1) {
						BeginBatchDraw();//双缓冲绘图
						gameRun();
						EndBatchDraw();
						if (gameOver) {
							break;
						}
				
						
					}
					while (1) {
						BeginBatchDraw();
						overDraw();
						EndBatchDraw();
						while (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
							if (msg.message == WM_LBUTTONDOWN) {
								if (isInRect(135, 510, 220, 50, msg.x, msg.y)) {
									// 点击了“Exit”按钮
									printf("Exit clicked\n");
									closegraph(); // 关闭图形窗口
									exit(0); // 退出程序

								}
								else if (isInRect(135, 380, 220, 50, msg.x, msg.y)) {
									// 点击了“Restart”按钮
									printf("Restart clicked\n");
									reStart = true;
									break; // 退出游戏结束界面循环
								}
							}
						}
					}
					break;
				}
				if (isInRect(150, 395, 170, 50, msg.x, msg.y)) {
					// 处理点击"setting"区域的逻辑
					printf("Setting clicked\n");
				}
				if (isInRect(150, 470, 170, 50, msg.x, msg.y)) {
					// 处理点击"exit"区域的逻辑
					printf("Exit clicked\n");
					closegraph();
					exit(0);
					return 0;
				}
			}
		}
	}


	EndBatchDraw();
	return 0;
}