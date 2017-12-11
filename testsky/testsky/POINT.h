#pragma once
//
//  POINT.h
//  GLUTTest
//
//  Created by C-TEN on 2017/10/22.
//  Copyright © 2017年 C-TEN. All rights reserved.
//

#ifndef POINT_h
#define POINT_h

#include <iostream>

using namespace std;
class POINT
{
//private:
public:
	int x, y;
public:
	POINT() {
		x = 0;
		y = 0;
	}
	void setpoint(int a, int b)
	{
		x = a;
		y = b;
	}
	void showpoint()
	{
		cout << "(" << x << "," << y << ")" << endl;
	}
};

#endif /* POINT_h */
