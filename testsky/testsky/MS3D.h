// Thanks to Brett Porter and Mete Ciragan for help with this MS3D model loading code

#ifndef MS3D_H
#define MS3D_H


#include <windows.h>
#include <iostream>
#include <fstream>
#include <gl/gl.h>
#include "glut\glut.h"
//#include "CBMPLoader.h"
#include "AABBbox.h"
#include <vector>
#include "Vector.h"
using namespace std;
/* 
	MS3D STRUCTURES 
*/
#ifndef byte
typedef unsigned char byte;
#endif // byte

#ifndef word
typedef unsigned short word;
#endif 
#define	Q_PI	3.14159265358979323846
#ifdef _MSC_VER							// byte-align structures
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#	define PATH_MAX _MAX_PATH
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#	include <limits.h>
#else
#	error you must byte-align these structures with the appropriate compiler directives
#endif

struct MS3DHeader						// File Header
{
	char m_ID[10];
	int m_version;
} ;

struct MS3DVertex						// Vertex info
{
	byte m_flags;
	float m_vertex[3];
	char m_boneID;
	byte m_refCount;
} ;

struct MS3DTriangle_info					// MS3DTriangle info
{
	word m_flags;
	word m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	byte m_smoothingGroup;
	byte m_groupIndex;
};

struct MS3DMaterial_info						// MS3DMaterial info
{
    char m_name[32];
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    float m_shininess;					// 0.0f - 128.0f
    float m_transparency;				// 0.0f - 1.0f
    byte m_mode;						// 0, 1, 2 (unused now)
    char m_texture[128];
    char m_alphamap[128];
} ;

struct MS3DKeyFrame
{
	float m_fTime;
	float m_fParam[3];
};
struct tangent
{
	float tangentIn[3];
	float tangentOut[3];
};


struct MS3DJoint
{
	unsigned char m_ucpFlags;                                //编辑器标志
	char m_cName[32];                                             //本Joint的名字
	char m_cParent[32];                                            //本Joint的父Joint名字
	float m_fRotation[3];                                           //初始旋转
	float m_fPosition[3];                                            //初始平移
	unsigned short m_usNumRotFrames;                 //旋转帧的数量
	unsigned short m_usNumTransFrames;            //平移帧的数量
	MS3DKeyFrame* m_RotKeyFrames;                //所有的旋转帧
	MS3DKeyFrame* m_TransKeyFrames;            //所有的平移帧
	tangent* m_tangent;
	int parent_index;
	float matLocalSkeleton[3][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0} };
	float matGlobalSkeleton[3][4]= { {1, 0, 0, 0}, { 0,1,0,0 }, { 0,0,1,0 } };
	float matLocal[3][4];
	float matGlobal[3][4];

};



struct Mesh								//	Mesh struct
{
	int m_MS3DMaterialIndex;
	int m_numMS3DTriangles;
	int *m_pMS3DTriangleIndices;
};

struct MS3DMaterial							//	MS3DMaterial properties
{
	float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
	float m_shininess;
	//GLuint m_texture;
	CBMPLoader loader;
	char *m_pTextureFilename;
};

struct MS3DTriangle						//	MS3DTriangle struct
{
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	int m_vertexIndices[3];
};

struct Vertex							//	Vertex struct
{
	char m_boneID;
	float m_location[3];
};



//////////////////////////////////////
//The Model Class
//////////////////////////////////////
class MS3DModel
{
	public:
	
		MS3DModel();

		virtual ~MS3DModel();

		bool loadModelData( const char *filename );

		void draw();

		void reloadTextures();
		
		void setsize(double Size) { size = Size; };

		void SetFrame(float frame);

		int FindJointsByName(char* name);

		void SetupJoints();

		void SetupTangents();

		void EvaluateJoint(int index, float frame);
	//protected:

		int m_numMeshes;
		Mesh *m_pMeshes;

		int m_numMS3DMaterials;
		MS3DMaterial *m_pMS3DMaterials;

		int m_numMS3DTriangles;
		MS3DTriangle *m_pMS3DTriangles;

		int m_numVertices;
		Vertex *m_pVertices;

		int m_numJoints;
		struct MS3DJoint* m_joints;
		double size=1;

		float m_animationFps;
		float m_currentTime;
		int m_totalFrames;
		Vector3 position = Vector3(0, 0, 0);
		Vector3 direction = Vector3(0, 0, 1);
		vector<AABBbox>BoxLists;
		AABBbox Box = AABBbox(10000, 10000, 10000, -10000, -10000, -10000);
		void DrawModelEdge();
		void render();
		bool is_In_Model(Point3 p);
		bool is_Collide(MS3DModel model);
		void Translate(double x, double y, double z);
		void setDir(Vector3 dir);
		void move(Vector3 dir, double speed);
		bool visible = true;
		bool explode = false;
		int explode_time = 0;
		void explosion();
};
void AngleMatrix(const float* angles, float(*matrix)[4]);
void R_ConcatTransforms(const float in1[3][4], const float in2[3][4], float out[3][4]);
void AngleQuaternion(float* angles, float* quaternion);
void QuaternionMatrix(float* quaternion, float(*matrix)[4]);
void QuaternionSlerp(float* p, float* q, float t, float* qt);
void VectorRotate(float const* in1, const float in2[3][4], float* out);
void VectorIRotate(float const* in1, const float in2[3][4], float*out);
void VectorTransform(float const* in1, const float in2[3][4], float* out);
void VectorITransform(float const* in1, const float in2[3][4], float* out);
void printMatrix(const float in2[3][4]);
#endif
