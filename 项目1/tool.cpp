#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<easyx.h>
#include"tool.h"

void drawImg(int x, int y, IMAGE* src)
{
	// ������ʼ��
	DWORD* pwin = GetImageBuffer();			//���ڻ�����ָ��
	DWORD* psrc = GetImageBuffer(src);		//ͼƬ������ָ��
	int win_w = getwidth();				//���ڿ��
	int win_h = getheight();
	int src_w = src->getwidth();				//ͼƬ���
	int src_h = src->getheight();

	// ������ͼ��ʵ�ʳ���
	int real_w = (x + src_w > win_w) ? win_w - x : src_w;			// �������ұ߽�
	int real_h = (y + src_h > win_h) ? win_h - y : src_h;			// �������±߽�
	if (x < 0) { psrc += -x;			real_w -= -x;	x = 0; }	// ��������߽�
	if (y < 0) { psrc += (src_w * -y);	real_h -= -y;	y = 0; }	// �������ϱ߽�


	// ������ͼ��ʼλ��
	pwin += (win_w * y + x);

	// ʵ��͸����ͼ
	for (int iy = 0; iy < real_h; iy++)
	{
		for (int ix = 0; ix < real_w; ix++)
		{
			byte a = (byte)(psrc[ix] >> 24);//����͸��ͨ����ֵ[0,256) 0Ϊ��ȫ͸�� 255Ϊ��ȫ��͸��
			if (a > 100)
			{
				pwin[ix] = psrc[ix];
			}
		}
		//������һ��
		pwin += win_w;
		psrc += src_w;
	}
}