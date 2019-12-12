//1.背景图像，人物图像，怪兽图像等铺设
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")  //实现透明贴图

//定义全局变量          
#define pictureWidth 8022 //背景图片宽度
#define pictureHigh 525  //背景图片高度
#define Width 1024   //画面宽度
#define High 525      //画面高度
#define roleWidth 39    //人物宽度
#define roleHigh 65   //人物高度
#define MonsterWidth 128  //怪兽宽度
#define MonsterWidth 128   //怪兽高度

//函数声明
int startmenu();     //开始界面
void ksdz(int stepi, int stepj); //开始界面的动作
void startup();   //数据初始化
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
void RoleWalk(int left, int right, int judge);  //人物行走
void show();  //显示画面
void updateWithoutInput();  //与输入无关的更新
void updateWithInput();  //与输入有关的更新
int iscontinue();   //判断是否继续

int role_x, role_y;  //人物画布坐标
int real_x, real_y;  //人物实际坐标
int speed_x, speed_y;  //人物速度
int step_left, step_right; //人物行走脚步
int walkjudge;  //人物行走判断1向左走，2向右走
int bk_x, bk_y;  //背景图片绘制坐标
int judgeJump;  //判断是否跳跃 0在地面，1为向上，2为向下
int flour; //地面高度
int jumpHigh, jumpHighTop;  //人物跳跃限制
int ksjm_x, ksjm_y; //开始界面图片坐标
int jx_x, jx_y;  //继续界面的坐标

IMAGE img_ksjm;//开始界面
IMAGE img_gz;  //规则图片
IMAGE img_bk;  //背景画面
IMAGE img_ty;  //跳跃图片
IMAGE img_jx;  //是否继续图片
IMAGE img_role1r, img_role2r, img_role3r, img_role4r, img_role5r, img_role6r, img_role7r, img_role8r, img_role9r, img_role10r;  //人物图片  向右
IMAGE img_role1l, img_role2l, img_role3l, img_role4l, img_role5l, img_role6l, img_role7l, img_role8l, img_role9l, img_role10l;  //人物图片  向向左

struct kd  //坑洞
{
	int start_x, end_x;
};
kd KD[3];

struct zc  //管道
{
	int start_x, end_x;
	int zc_high;
};
zc GD[6];
int startmenu()    //开始界面
{
	role_x = 160;
	role_y = 390;
	initgraph(Width, High);
	int flag = 1;
	while (1) {
	putimage(ksjm_x, ksjm_y, &img_ksjm);//导入人物图片
	transparentimage(NULL, role_x, role_y, &img_role1r);
	FlushBatchDraw();                   //输出图像
		if (GetAsyncKeyState(0x31) & 0x8000) {//开始游戏
			ksdz(36, 8);
			break;
		}
		else if (GetAsyncKeyState(0x32) & 0x8000) {//游戏规则
			ksdz(48, 4);
			putimage(ksjm_x, ksjm_y, &img_gz);
			FlushBatchDraw();               //输出规则图片
			while (!(GetAsyncKeyState(0x1B) & 0x8000));//询问是否退出（判断）
			role_x = 160;
			role_y = 390;

		}
		else if (GetAsyncKeyState(0x33) & 0x8000) {//退出
			ksdz(60, 2);
			flag = 0;
			break;
		}
	}
	Sleep(100);
	role_x = 100;  //人物坐标
	role_y = 415;
	return flag;
	
}
void ksdz(int stepi,int stepj)//开始界面的动作
{
	for (int i = 0; i < stepi; i++) {
		putimage(ksjm_x, ksjm_y, &img_ksjm);
		FlushBatchDraw();
		walkjudge = 2;
		role_x += speed_x;
		step_right++;
		RoleWalk(step_left, step_right % 10, walkjudge);
		Sleep(20);
		cleardevice();
	}
	putimage(ksjm_x, ksjm_y, &img_ksjm);
	transparentimage(NULL, role_x, role_y, &img_role2r);
	FlushBatchDraw();
	Sleep(200);
	transparentimage(NULL, role_x, role_y, &img_role2l);
	FlushBatchDraw();
	Sleep(200);
	transparentimage(NULL, role_x, role_y, &img_role2r);
	FlushBatchDraw();
	Sleep(500);
	cleardevice();
	for (int j = 0; j < stepj; j++) {
		putimage(ksjm_x, ksjm_y, &img_ksjm);
		FlushBatchDraw();
		walkjudge = 3;
		role_y -= speed_y * 2;
		real_y -= speed_y * 2;
		RoleWalk(step_left, step_right % 10, walkjudge);
		Sleep(20);
		cleardevice();
	}
	walkjudge = 2;
	Sleep(200);
}
void startup()   //数据初始化
{
	role_x = 100;  //人物坐标
	role_y = 415;
	real_x = 100;  //人物实际坐标
	real_y = 415;
	flour = 415;  //地面高度
	speed_x = 5;  // 人物前进速度
	speed_y = 7;  //人物跳跃的速度
	step_left = 0;  //人物行走图片循环
	step_right = 0;
	walkjudge = 2;  //人物行走判断
	bk_x = 0;   //背景绘制坐标
	bk_y = 0;
	ksjm_x = 0; //开始界面的坐标 
	ksjm_y = 0;
	jx_x = 0;//继续界面的坐标
	jx_y = 0;
	judgeJump = 0;  //判断是否跳跃 0表示否，1表示是
	jumpHigh = 0;  //人物跳跃高度为0
	jumpHighTop = High / 2 - 70;  //人物跳跃极限高度

	KD[0].start_x = 2610;  //空地坐标初始化
	KD[0].end_x = 2686;

	KD[1].start_x = 3255;
	KD[1].end_x = 3367;

	KD[2].start_x = 5790;
	KD[2].end_x = 5863;

	GD[0].start_x = 1062;  //管道坐标初始化
	GD[0].end_x = 1135;
	GD[0].zc_high = 17;

	GD[1].start_x = 1438;
	GD[1].end_x = 1511;
	GD[1].zc_high = 55;

	GD[2].start_x = 1743;
	GD[2].end_x = 1811;
	GD[2].zc_high = 93;

	GD[3].start_x = 2160;
	GD[3].end_x = 2228;
	GD[3].zc_high = 93;

	GD[4].start_x = 6173;
	GD[4].end_x = 6240;
	GD[4].zc_high = 18;

	GD[5].start_x = 6777;
	GD[5].end_x = 6845;
	GD[5].zc_high = 18;
	initgraph(Width, High);
	loadimage(&img_ksjm, _T("C:\\Users\\zym\\Desktop\\Picture material\\开始界面.png"));  //开始界面图片导入
	loadimage(&img_gz, _T("C:\\Users\\zym\\Desktop\\Picture material\\规则图片.png"));//规则图片的导入
	loadimage(&img_bk, _T("C:\\Users\\zym\\Desktop\\Picture material\\背景图.png"));  //背景图片导入
	loadimage(&img_ty, _T("C:\\Users\\zym\\Desktop\\Picture material\\跳跃r9.png"));  //跳跃图片的导入

	loadimage(&img_role1r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物1r.png"));//人物图像导入
	loadimage(&img_role2r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物2r.png"));//人物图像导入
	loadimage(&img_role3r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物3r.png"));//人物图像导入
	loadimage(&img_role4r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物4r.png"));//人物图像导入
	loadimage(&img_role5r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物5r.png"));//人物图像导入    向右
	loadimage(&img_role6r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物6r.png"));//人物图像导入
	loadimage(&img_role7r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物7r.png"));//人物图像导入
	loadimage(&img_role8r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物8r.png"));//人物图像导入
	loadimage(&img_role9r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物9r.png"));//人物图像导入
	loadimage(&img_role10r, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物10r.png"));//人物图像导入

	loadimage(&img_role1l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物1l.png"));//人物图像导入
	loadimage(&img_role2l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物2l.png"));//人物图像导入
	loadimage(&img_role3l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物3l.png"));//人物图像导入
	loadimage(&img_role4l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物4l.png"));//人物图像导入
	loadimage(&img_role5l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物5l.png"));//人物图像导入    向左
	loadimage(&img_role6l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物6l.png"));//人物图像导入
	loadimage(&img_role7l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物7l.png"));//人物图像导入
	loadimage(&img_role8l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物8l.png"));//人物图像导入
	loadimage(&img_role9l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物9l.png"));//人物图像导入
	loadimage(&img_role10l, _T("C:\\Users\\zym\\Desktop\\Picture material\\人物10l.png"));//人物图像导入

	loadimage(&img_jx,_T("C:\\Users\\zym\\Desktop\\Picture material\\continue.png"));  //询问是否继续图片

	BeginBatchDraw();
}

// 半透明贴图函数
// 参数：
//		dstimg: 目标 IMAGE 对象指针。NULL 表示默认窗体
//		x, y:	目标贴图位置
//		srcimg: 源 IMAGE 对象指针。NULL 表示默认窗体

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	// 使用 Windows GDI 函数实现半透明位图
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void RoleWalk(int left,int right,int judge)  //人物行走
{
	if (judge == 1) {
		switch (left)
		{
		case 0:transparentimage(NULL, role_x, role_y, &img_role1l);  break;
		case 1:transparentimage(NULL, role_x, role_y, &img_role2l);  break;
		case 2:transparentimage(NULL, role_x, role_y, &img_role3l);  break;
		case 3:transparentimage(NULL, role_x, role_y, &img_role4l);  break;
		case 4:transparentimage(NULL, role_x, role_y, &img_role5l);  break;
		case 5:transparentimage(NULL, role_x, role_y, &img_role6l);  break;
		case 6:transparentimage(NULL, role_x, role_y, &img_role7l);  break;
		case 7:transparentimage(NULL, role_x, role_y, &img_role8l);  break;
		case 8:transparentimage(NULL, role_x, role_y, &img_role9l);  break;
		case 9:transparentimage(NULL, role_x, role_y, &img_role10l);  break;
		default:break;
		}
	}
	else if (judge == 2)
	{
		switch (right)
		{
		case 0:transparentimage(NULL, role_x, role_y, &img_role1r);  break;
		case 1:transparentimage(NULL, role_x, role_y, &img_role2r);  break;
		case 2:transparentimage(NULL, role_x, role_y, &img_role3r);  break;
		case 3:transparentimage(NULL, role_x, role_y, &img_role4r);  break;
		case 4:transparentimage(NULL, role_x, role_y, &img_role5r);  break;
		case 5:transparentimage(NULL, role_x, role_y, &img_role6r);  break;
		case 6:transparentimage(NULL, role_x, role_y, &img_role7r);  break;
		case 7:transparentimage(NULL, role_x, role_y, &img_role8r);  break;
		case 8:transparentimage(NULL, role_x, role_y, &img_role9r);  break;
		case 9:transparentimage(NULL, role_x, role_y, &img_role10r);  break;
		default:break;
		}
	}
	else if (judge == 3) {
		transparentimage(NULL, role_x, role_y, &img_ty);
	}
	FlushBatchDraw();
}

void show()  //显示画面
{
	putimage(bk_x, bk_y, &img_bk);
	RoleWalk(step_left, step_right, walkjudge);

	FlushBatchDraw();
}
void updateWithoutInput()  //与输入无关的更新
{
	if (judgeJump == 0 && role_y < flour)  //没有跳高情况下
	{
		role_y += speed_y;
		real_y += speed_y;
	}
	if (judgeJump == 1)
	{
		role_y -= speed_y * 2;
		real_y -= speed_y * 2;
		jumpHigh += speed_y * 2;
		if (jumpHigh >= jumpHighTop)
		{
			judgeJump = 0;
			jumpHigh = 0;
		}
	}

	for (int i = 0; i < 3; i++)  //坑地下落判断
	{
		if (real_x + roleWidth / 2 > KD[i].start_x&& real_x + roleWidth / 2 < KD[i].end_x || real_y > flour)
		{
			if (judgeJump == 0)
			{
				role_y += speed_y;
				real_y += speed_y;
			}
		}
	}

	for (int i = 0; i < 6; i++) // 管道判断
	{
		//if ()
	}
}

void updateWithInput()  //与输入有关的更新
{

	if ((GetAsyncKeyState(0x44) & 0x8000)) //d
	{
		if (role_x >= Width / 2 && bk_x > Width - pictureWidth)
		{
			bk_x -= speed_x;
			real_x += speed_x;
			walkjudge = 2;
			step_right++;
		}
		else if (role_x < pictureWidth)
		{
			walkjudge = 2;
			role_x += speed_x;
			real_x += speed_x;
			step_right++;
		}
	}
	else if ((GetAsyncKeyState(0x41) & 0x8000))  //a
	{
		if (role_x <= Width / 2 && bk_x > 0)
		{
			bk_x += speed_x;
			real_x -= speed_x;
			walkjudge = 1;
			step_left++;
		}
		else if (role_x > 0)
		{
			walkjudge = 1;
			role_x -= speed_x;
			real_x -= speed_x;
			step_left++;
		}
	}
	else if ((GetAsyncKeyState(0x57) & 0x8000))  //跳跃
	{
		if (role_y >= flour)
			judgeJump = 1;

	}


	if (step_right >= 9)
		step_right = 0;
	if (step_left >= 9)
		step_left = 0;


}
int iscontinue()   //判断是否继续
{
	int c = 1;
	while (!((GetAsyncKeyState(0x1B) & 0x8000) || (GetAsyncKeyState(0x20) & 0x8000))) {
		putimage(jx_x, jx_y, &img_jx);
		FlushBatchDraw();
	}
	if ((GetAsyncKeyState(0x1B) & 0x8000))
		c = 0;
	else if ((GetAsyncKeyState(0x20) & 0x8000))
		c = 1;
	return c;
}

int main(void)
{
	int con=1;  //判断是否继续的参数
	while (con==1) {
		startup();
		int flag = startmenu(); //开始界面
		while (flag)
		{
			show();
			updateWithoutInput();
			updateWithInput();
			Sleep(10);
			cleardevice();
		}
		if (flag == 1) {
			con = iscontinue(); //判断是否继续
		}
	}
	return 0;
}
