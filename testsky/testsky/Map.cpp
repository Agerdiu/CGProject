  #include "Map.h"
#include "glut\glut.h"
using namespace std;
// 从*.raw文件中加载高度数据
#define MAP_SIZE 1024
void Map::LoadFile(string objfile,string mtlfile)
{
	 loader.loadFromFile((char*)objfile.c_str(), (char*)mtlfile.c_str());
	 double tmp = 0;
	 for (int i = 0; i < loader.m_mtls.size(); i++) {
		 tmp+=loader.m_mtls[i]->triNum * 3;
		 for (int j = 0; j < loader.m_mtls[i]->triNum * 3; j++) {
			 MiddleX += loader.m_mtls[i]->triVertexs[3 * j];
			 MiddleY += loader.m_mtls[i]->triVertexs[3 * j + 1];
			 MiddleZ += loader.m_mtls[i]->triVertexs[3 * j + 2];
		 }
	 }
	 MiddleX /= tmp;
	 MiddleY /= tmp;
	 MiddleZ /= tmp;
	 for (int i = 0; i < loader.m_mtls.size(); i++) {
		 for (int j = 0; j < loader.m_mtls[i]->triNum * 3; j++) {
			 loader.m_mtls[i]->triVertexs[3 * j]/= MiddleX/100;
			 loader.m_mtls[i]->triVertexs[3 * j + 1]/= MiddleY/100;
			 loader.m_mtls[i]->triVertexs[3 * j + 2]/= MiddleZ/100;
		 }
	 }
}
int Map::getHeight(int X, int Y)   // 下面的函数返回(x,y)点的高度  
{
	return 0;
}
void Map::Render() {
	loader.renderObj();
}