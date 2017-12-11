#pragma once
//
//  BMP.h
//  GLUTTest
//
//  Created by C-TEN on 2017/10/22.
//  Copyright © 2017年 C-TEN. All rights reserved.
//

#ifndef BMP_h
#define BMP_h
typedef unsigned char BYTE;//1字节
typedef unsigned short WORD;//2字节
typedef unsigned int DWORD;//4字节
typedef long LONG;

//BMP文件头（14字节）
typedef struct tagBITMAPFILEHEADER {
	//WORD bfType;//位图文件的类型，必须为BM(在结构体中读取会发生错误，所以在函数中读取)
	DWORD bfSize;//位图文件的大小，以字节为单位
	WORD bfReserved1;//位图文件保留字，必须为0
	WORD bfReserved2;//位图文件保留字，必须为0
	DWORD bfOffBits;//位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
}BITMAPFILEHEADER;

//BMP信息头（40字节）
typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;//本结构所占用字节数
	LONG biWidth;//位图的宽度，以像素为单位
	LONG biHeight;//位图的高度，以像素为单位
	WORD biPlanes;//目标设备的级别，必须为1
	WORD biBitCount;//每个像素所需的位数，必须是1（双色），4(16色），8(256色）16(高彩色)或24（真彩色）之一
	DWORD biCompression;//位图压缩类型，必须是0（不压缩）,1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
	DWORD biSizeImage;//位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位
	LONG biXPelsPerMeter;//位图水平分辨率，每米像素数
	LONG biYPelsPerMeter;//位图垂直分辨率，每米像素数
	DWORD biClrUsed;//位图实际使用的颜色表中的颜色数
	DWORD biClrImportant;//位图显示过程中重要的颜色数
}BITMAPINFOHEADER;

//调色板
//只有8位位图才用调色板，用像素值作为索引（0~255），调色板中RGB值都是一样的，范围是0~255
//一个unsigned char的范围刚好是0~255，所以用BYTE类型存储8位位图的像素
typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;//保留，必须为0
}RGBQUAD;

//像素信息
//8位BMP图1个字节代表一个像素，所以可以不用结构存储像素素组，直接用一个指针即可
typedef struct tagIMAGEDATA {
	BYTE blue;
	BYTE green;
	BYTE red;
}IMAGEDATA;

#endif /* BMP_h */
