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

IMAGE	bk;
IMAGE img_player1;
IMAGE img_player2;
IMAGE img_bullet1;
IMAGE img_bullet2;
IMAGE img_enemy1;
IMAGE img_enemy2;

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
}player1,player2,bullet[BULLET_NUM],bullet2[BULLET_NUM],enemy[ENEMY_NUM];
const int HIT_COOLDOWN = 1000;

void loadImg() {
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
	//
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
void EnemyMove(int speed) {		//敌机的移动
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live != 0) {

			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT+100) {
				enemy[i].live = false;
			}
		}
	}
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
		outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 50, "Both Players Died! Game Over!");
		// 延时2秒
		Sleep(2000);
		closegraph();
		exit(0);
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
			enemy[i].live = false;
		}
	}
	Gameover();
}

//初始化按钮的属性
struct button* creatButton(int x, int y, int width, int height, COLORREF color, const char* pText) {
	struct button* pB = (struct button*)malloc(sizeof(struct button));
	pB->x = x;
	pB->y = y;
	pB->width = width;
	pB->height = height;
	pB->color = color;
	pB->pText = (char*)malloc(strlen(pText) + 1);			//统计可见长度再加一
	strcpy(pB->pText, pText);												//代码拷贝到按钮里面的文字
	return pB;
}
//按钮的绘制 
void drawButton(struct button* pB) {
	setfillcolor(pB->color);
	settextstyle(35, 0, "小篆");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);
	outtextxy(pB->x + 3, pB->y + 8, pB->pText);
}
int mouseInButton(struct button* pB, MOUSEMSG m) {
	if (pB->x <= m.x && m.x <= pB->x + pB->width && pB->y <= m.y && m.y <= pB->height + pB->y) {
		pB->color = RED;
		return 1;
	}
	pB->color = YELLOW;
	return 0;
}
int clickButton(struct button* pB, MOUSEMSG m) {
	if (mouseInButton(pB, m) && m.uMsg == WM_LBUTTONDOWN) {
		return 1;
	}
	return 0;
}
	






int main() {
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
	BeginBatchDraw();//双缓冲绘图
	
	while (1)
	{
		gameDraw();
		FlushBatchDraw();
		Playermove(1);
		BulletMove(3);
		Bullet2Move(3);
		if (Timer(500,0)) {
			CreateEnemy();
		}
		if (Timer(5, 2)) {
			EnemyMove(1);
		}
		Fight();

	}
	

	EndBatchDraw();
	return 0;
}