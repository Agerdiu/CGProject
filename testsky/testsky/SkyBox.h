#pragma once
//
//  SkyBox.h
//  GLUTTest
//
//  Created by C-TEN on 2017/10/22.
//  Copyright © 2017年 C-TEN. All rights reserved.
//

#ifndef SkyBox_h
#define SkyBox_h



//#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"
#include <iostream>
using namespace std;
const unsigned int MAP_WIDTH =  1024;
const unsigned int CELL_WIDTH = 16;
const unsigned int MAP = MAP_WIDTH * CELL_WIDTH / 2;
#define GL_CLAMP_TO_EDGE    0x812F

/** 天空盒类 */
class CSkyBox
{
public:
	/** 构造函数 */
	CSkyBox();
	~CSkyBox();

	/** 初始化 */
	bool Init();

	/** 渲染天空 */
	void  CreateSkyBox(float x, float y, float z,
		float width, float height,
		float length);

private:

	CBMPLoader  m_texture[6];   /**< 天空盒纹理 */

};


#endif /* SkyBox_h */
