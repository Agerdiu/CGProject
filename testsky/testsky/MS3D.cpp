#include "MS3D.h"
#include <string>
#include <iomanip>
#define DotProduct(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
/////////////////////////////////////////////////////////////////////////////////////////////////
//										The Model Class
/////////////////////////////////////////////////////////////////////////////////////////////////
MS3DModel::MS3DModel()
{
	m_numMeshes = 0;
	m_pMeshes = NULL;
	m_numMS3DMaterials = 0;
	m_pMS3DMaterials = NULL;
	m_numMS3DTriangles = 0;
	m_pMS3DTriangles = NULL;
	m_numVertices = 0;
	m_pVertices = NULL;
}

MS3DModel::~MS3DModel()
{
	cout << "Model has been destroyed" << endl;
	int i;
	for ( i = 0; i < m_numMeshes; i++ )
		delete[] m_pMeshes[i].m_pMS3DTriangleIndices;
	for ( i = 0; i < m_numMS3DMaterials; i++ )
		delete[] m_pMS3DMaterials[i].m_pTextureFilename;

	m_numMeshes = 0;
	if ( m_pMeshes != NULL )
	{
		delete[] m_pMeshes;
		m_pMeshes = NULL;
	}

	m_numMS3DMaterials = 0;
	if ( m_pMS3DMaterials != NULL )
	{
		delete[] m_pMS3DMaterials;
		m_pMS3DMaterials = NULL;
	}

	m_numMS3DTriangles = 0;
	if ( m_pMS3DTriangles != NULL )
	{
		delete[] m_pMS3DTriangles;
		m_pMS3DTriangles = NULL;
	}

	m_numVertices = 0;
	if ( m_pVertices != NULL )
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
	m_numJoints = 0;
	if (m_joints != 0) {
		delete[] m_joints;
		m_joints = NULL;
	}
}



bool MS3DModel::loadModelData( const char *filename )
{
	cout << "read file" << endl;
	ifstream inputFile( filename, ios::in | ios::binary);
	if ( inputFile.fail())
		return false;	// "Couldn't open the model file."

	char pathTemp[PATH_MAX+1];
	int pathLength;
	for ( pathLength = strlen( filename ); pathLength--; ) {
		if ( filename[pathLength] == '/' || filename[pathLength] == '\\' ) {
			break;
		}
	}
	strncpy( pathTemp, filename, pathLength );
	pathTemp[pathLength] = '\0';
	inputFile.seekg( 0, ios::end );
	long fileSize = inputFile.tellg();
	inputFile.seekg( 0, ios::beg );

	byte *pBuffer = new byte[fileSize];
	inputFile.read((char*)pBuffer, fileSize );
	inputFile.close();

	const byte *pPtr = pBuffer;
	MS3DHeader *pHeader = ( MS3DHeader* )pPtr;
	pPtr += sizeof( MS3DHeader );

	if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
		return false; // "Not a valid Milkshape3D model file."

	if ( pHeader->m_version < 3 )
		return false; // "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	int nVertices = *( word* )pPtr; 
	m_numVertices = nVertices;
	m_pVertices = new Vertex[nVertices];
	pPtr += sizeof( word );

	int i;
	for ( i = 0; i < nVertices; i++ )
	{
		MS3DVertex *pVertex = ( MS3DVertex* )pPtr;
		m_pVertices[i].m_boneID = pVertex->m_boneID;
		memcpy( m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
		pPtr += sizeof( MS3DVertex );
	}

	int nMS3DTriangles = *( word* )pPtr;
	m_numMS3DTriangles = nMS3DTriangles;
	m_pMS3DTriangles = new MS3DTriangle[nMS3DTriangles];
	pPtr += sizeof( word );

	for ( i = 0; i < nMS3DTriangles; i++ )
	{
		MS3DTriangle_info *pMS3DTriangle = ( MS3DTriangle_info* )pPtr;
		int vertexIndices[3] = { pMS3DTriangle->m_vertexIndices[0], pMS3DTriangle->m_vertexIndices[1], pMS3DTriangle->m_vertexIndices[2] };
		float t[3] = { 1.0f-pMS3DTriangle->m_t[0], 1.0f-pMS3DTriangle->m_t[1], 1.0f-pMS3DTriangle->m_t[2] };
		memcpy( m_pMS3DTriangles[i].m_vertexNormals, pMS3DTriangle->m_vertexNormals, sizeof( float )*3*3 );
		memcpy( m_pMS3DTriangles[i].m_s, pMS3DTriangle->m_s, sizeof( float )*3 );
		memcpy( m_pMS3DTriangles[i].m_t, t, sizeof( float )*3 );
		memcpy( m_pMS3DTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
		pPtr += sizeof( MS3DTriangle_info);
	}


	int nGroups = *( word* )pPtr;
	m_numMeshes = nGroups;
	m_pMeshes = new Mesh[nGroups];
	pPtr += sizeof( word );
	for ( i = 0; i < nGroups; i++ )
	{
		pPtr += sizeof( byte );	// flags
		pPtr += 32;				// name

		word nMS3DTriangles = *( word* )pPtr;
		pPtr += sizeof( word );
		int *pMS3DTriangleIndices = new int[nMS3DTriangles];
		for ( int j = 0; j < nMS3DTriangles; j++ )
		{
			pMS3DTriangleIndices[j] = *( word* )pPtr;
			pPtr += sizeof( word );
		}

		char MS3DMaterialIndex = *( char* )pPtr;
		pPtr += sizeof( char );
	
		m_pMeshes[i].m_MS3DMaterialIndex = MS3DMaterialIndex;
		m_pMeshes[i].m_numMS3DTriangles = nMS3DTriangles;
		m_pMeshes[i].m_pMS3DTriangleIndices = pMS3DTriangleIndices;
	}

	int nMS3DMaterials = *( word* )pPtr;
	m_numMS3DMaterials = nMS3DMaterials;
	m_pMS3DMaterials = new MS3DMaterial[nMS3DMaterials];
	pPtr += sizeof( word );
	for ( i = 0; i < nMS3DMaterials; i++ )
	{
		MS3DMaterial_info *pMS3DMaterial = ( MS3DMaterial_info* )pPtr;
		memcpy( m_pMS3DMaterials[i].m_ambient, pMS3DMaterial->m_ambient, sizeof( float )*4 );
		memcpy( m_pMS3DMaterials[i].m_diffuse, pMS3DMaterial->m_diffuse, sizeof( float )*4 );
		memcpy( m_pMS3DMaterials[i].m_specular, pMS3DMaterial->m_specular, sizeof( float )*4 );
		memcpy( m_pMS3DMaterials[i].m_emissive, pMS3DMaterial->m_emissive, sizeof( float )*4 );
		m_pMS3DMaterials[i].m_shininess = pMS3DMaterial->m_shininess;
		string str_file = filename;
		str_file = str_file.substr(0, str_file.rfind("/")).append("/");
		if ( strncmp( pMS3DMaterial->m_texture, ".\\", 2 ) == 0) {
			// MS3D 1.5.x relative path
			char* tmp2=(char*)str_file.append(pMS3DMaterial->m_texture).c_str();
			m_pMS3DMaterials[i].m_pTextureFilename = new char[strlen(tmp2) + 1];
			strcpy(m_pMS3DMaterials[i].m_pTextureFilename, tmp2);
			//strcpy( pathTemp + pathLength, pMS3DMaterial->m_texture + 1 );
			//m_pMS3DMaterials[i].m_pTextureFilename = new char[strlen( pathTemp )+1];
			//strcpy( m_pMS3DMaterials[i].m_pTextureFilename, pathTemp );
		}
		else
		{
			// MS3D 1.4.x or earlier - absolute path
			//strcpy(pathTemp + pathLength, pMS3DMaterial->m_texture + 1);
			//m_pMS3DMaterials[i].m_pTextureFilename = new char[strlen( pMS3DMaterial->m_texture )+1];
			//strcpy( m_pMS3DMaterials[i].m_pTextureFilename, pMS3DMaterial->m_texture );
			char* tmp2 = (char*)str_file.append(pMS3DMaterial->m_texture).c_str();
			m_pMS3DMaterials[i].m_pTextureFilename = new char[strlen(tmp2) + 1];
			strcpy(m_pMS3DMaterials[i].m_pTextureFilename, tmp2);
		}
		pPtr += sizeof( MS3DMaterial_info );
	}
	m_animationFps=*(float*)pPtr;
	pPtr += sizeof(float);
	m_currentTime= *(float*)pPtr;
	pPtr += sizeof(float);
	m_totalFrames=*(int*)pPtr;
	pPtr += sizeof(int);
	//pPtr += sizeof(float) * 2 + sizeof(int);
	int n_Joint = *(word*)pPtr;
	m_numJoints = n_Joint;
	m_joints = new MS3DJoint[n_Joint];
	cout << n_Joint << endl;
	pPtr += sizeof(word);
	for (int i = 0; i < n_Joint; i++) {
		MS3DJoint* tmpJoint = (MS3DJoint*)pPtr;
		m_joints[i].m_ucpFlags = tmpJoint->m_ucpFlags;                 
		pPtr += sizeof(unsigned char);
		memcpy(m_joints[i].m_cName, tmpJoint->m_cName, sizeof(char) * 32);
		pPtr += sizeof(char)*32;
		memcpy(m_joints[i].m_cParent, tmpJoint->m_cParent, sizeof(char) * 32);
		pPtr += sizeof(char) * 32;
		memcpy(m_joints[i].m_fRotation, tmpJoint->m_fRotation, sizeof(float) * 3);
		pPtr += sizeof(float) * 3;
		memcpy(m_joints[i].m_fPosition, tmpJoint->m_fPosition, sizeof(float) * 3);
		pPtr += sizeof(float) * 3;
		m_joints[i].m_usNumRotFrames = tmpJoint->m_usNumRotFrames;
		pPtr += sizeof(unsigned short);
		m_joints[i].m_usNumTransFrames = tmpJoint->m_usNumTransFrames;
		pPtr += sizeof(unsigned short);
		m_joints[i].m_RotKeyFrames = new MS3DKeyFrame[tmpJoint->m_usNumRotFrames];
		m_joints[i].m_TransKeyFrames = new MS3DKeyFrame[tmpJoint->m_usNumTransFrames];
		
		cout << tmpJoint->m_cName << endl;
		for (int j = 0; j < tmpJoint->m_usNumRotFrames; j++) {
			MS3DKeyFrame *tmpFrame= (MS3DKeyFrame*)pPtr;
			m_joints[i].m_RotKeyFrames[j].m_fTime = tmpFrame->m_fTime;
			m_joints[i].m_RotKeyFrames[j].m_fTime *= m_animationFps;
			pPtr += sizeof(float);
			//cout << "Time " << m_joints[i].m_RotKeyFrames[j].m_fTime << endl;
			memcpy(m_joints[i].m_RotKeyFrames[j].m_fParam, tmpFrame->m_fParam,sizeof(float)*3);
			pPtr += sizeof(float)*3;
			//cout << "Rotation " << tmpFrame->m_fParam[0] << " " << tmpFrame->m_fParam[1] << " " << tmpFrame->m_fParam[2] <<endl;
		}
		for (int j = 0; j < tmpJoint->m_usNumTransFrames; j++) {
			MS3DKeyFrame *tmpFrame = (MS3DKeyFrame*)pPtr;
			m_joints[i].m_TransKeyFrames[j].m_fTime = tmpFrame->m_fTime;
			m_joints[i].m_TransKeyFrames[j].m_fTime *= m_animationFps;
			pPtr += sizeof(float);
			memcpy(m_joints[i].m_TransKeyFrames[j].m_fParam, tmpFrame->m_fParam, sizeof(float) * 3);
			pPtr += sizeof(float) * 3;
			//cout <<"Transfrom " <<tmpFrame->m_fParam[0] << " " << tmpFrame->m_fParam[1] << " " << tmpFrame->m_fParam[2] << endl;
		}
	}
	for (int i = 0; i < m_numVertices; i++) {
		if (m_pVertices[i].m_location[0] < Box.Xmin)
			Box.Xmin = m_pVertices[i].m_location[0];
		if (m_pVertices[i].m_location[1] < Box.Ymin)
			Box.Ymin = m_pVertices[i].m_location[1];
		if (m_pVertices[i].m_location[2] < Box.Zmin)
			Box.Zmin = m_pVertices[i].m_location[2];
		if (m_pVertices[i].m_location[0] > Box.Xmax)
			Box.Xmax = m_pVertices[i].m_location[0];
		if (m_pVertices[i].m_location[1] > Box.Ymax)
			Box.Ymax = m_pVertices[i].m_location[1];
		if (m_pVertices[i].m_location[2] > Box.Zmax)
			Box.Zmax = m_pVertices[i].m_location[2];

	}
	reloadTextures();
	delete[] pBuffer;

	return true;
}



void MS3DModel::draw() 
{
	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );
	glPushMatrix();
	for ( int i = 0; i < m_numMeshes; i++ )			// draw in groups
	{
		int MS3DMaterialIndex = m_pMeshes[i].m_MS3DMaterialIndex;
		if ( MS3DMaterialIndex >= 0 )
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT,   m_pMS3DMaterials[MS3DMaterialIndex].m_ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE,   m_pMS3DMaterials[MS3DMaterialIndex].m_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR,  m_pMS3DMaterials[MS3DMaterialIndex].m_specular );
			glMaterialfv( GL_FRONT, GL_EMISSION,  m_pMS3DMaterials[MS3DMaterialIndex].m_emissive );
			glMaterialf(  GL_FRONT, GL_SHININESS, m_pMS3DMaterials[MS3DMaterialIndex].m_shininess );
			if (m_pMS3DMaterials[MS3DMaterialIndex].loader.ID > 0)
			{
				glBindTexture( GL_TEXTURE_2D, m_pMS3DMaterials[MS3DMaterialIndex].loader.ID );
				glEnable( GL_TEXTURE_2D );
			}
			else
				glDisable( GL_TEXTURE_2D );
		}
		else
		{
			glDisable( GL_TEXTURE_2D );
		}
		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < m_pMeshes[i].m_numMS3DTriangles; j++ )
			{
				int MS3DTriangleIndex = m_pMeshes[i].m_pMS3DTriangleIndices[j];
				const MS3DTriangle* pTri = &m_pMS3DTriangles[MS3DTriangleIndex];

				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];
					float normal_tmp1[3],normal[3];
					float vertex_tmp[3],vertex[3];
					VectorIRotate(pTri->m_vertexNormals[k], m_joints[m_pVertices[index].m_boneID].matGlobalSkeleton, normal_tmp1);
					VectorRotate(normal_tmp1, m_joints[(int)m_pVertices[index].m_boneID].matGlobal, normal);
					//cout << (int)m_pVertices[index].m_boneID << endl;
					VectorITransform(m_pVertices[index].m_location, m_joints[m_pVertices[index].m_boneID].matGlobalSkeleton, vertex_tmp);
					VectorTransform(vertex_tmp, m_joints[(int)m_pVertices[index].m_boneID].matGlobal, vertex);
					glNormal3fv(normal);
					glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
					glVertex3fv(vertex);
				}
			}
		}
		glEnd();
	}
	glPopMatrix();
	if (texEnabled) {
		glEnable(GL_TEXTURE_2D);
	}
	else
		glDisable( GL_TEXTURE_2D );
}



void MS3DModel::reloadTextures()
{
	for ( int i = 0; i < m_numMS3DMaterials; i++ )
	{
		if ( strlen( m_pMS3DMaterials[i].m_pTextureFilename ) > 0 )
		{
			if (!m_pMS3DMaterials[i].loader.LoadBitmap(m_pMS3DMaterials[i].m_pTextureFilename))
			{
				cout << "Error: failed to load image!" << endl;
				continue;
			}
			glGenTextures(1, &(m_pMS3DMaterials[i].loader.ID));
			glBindTexture(GL_TEXTURE_2D, (m_pMS3DMaterials[i].loader.ID));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_pMS3DMaterials[i].loader.imageWidth, m_pMS3DMaterials[i].loader.imageHeight, 
				GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)m_pMS3DMaterials[i].loader.image);
		}
		else
		{
			m_pMS3DMaterials[i].loader.ID = 0;
		}
	}
}

int MS3DModel::FindJointsByName(char *name)
{
	for (int i = 0; i < m_numJoints; i++)
	{
		if (strcmp(m_joints[i].m_cName, name) == 0)
			return i;
	}

	return -1;
}
void MS3DModel::SetupJoints()
{
	for (int i = 0; i < m_numJoints; i++)
	{
		MS3DJoint *joint = &m_joints[i];
		joint->parent_index = FindJointsByName(joint->m_cParent);
		//cout <<joint->m_cName<<" "<< joint->parent_index << " " << joint->m_cParent << endl;
	}

	for (int i = 0; i < m_numJoints; i++)
	{
		MS3DJoint *joint = &m_joints[i];
		AngleMatrix(joint->m_fRotation, joint->matLocalSkeleton);
		joint->matLocalSkeleton[0][3] = joint->m_fPosition[0];
		joint->matLocalSkeleton[1][3] = joint->m_fPosition[1];
		joint->matLocalSkeleton[2][3] = joint->m_fPosition[2];

		if (joint->parent_index == -1)
		{
			memcpy(joint->matGlobalSkeleton, joint->matLocalSkeleton, sizeof(joint->matGlobalSkeleton));
			memcpy(joint->matGlobal, joint->matGlobalSkeleton, sizeof(joint->matGlobalSkeleton));
			memcpy(joint->matLocal, joint->matLocalSkeleton, sizeof(joint->matLocalSkeleton));
		}
		else
		{
			MS3DJoint* parentJoint = &m_joints[joint->parent_index];
			R_ConcatTransforms(parentJoint->matGlobalSkeleton, joint->matLocalSkeleton, joint->matGlobalSkeleton);
			memcpy(joint->matGlobal, joint->matGlobalSkeleton, sizeof(joint->matGlobalSkeleton));
			memcpy(joint->matLocal, joint->matLocalSkeleton, sizeof(joint->matLocalSkeleton));
		}

		SetupTangents();
	}
}
void MS3DModel::SetupTangents() 
{
	for (int j = 0; j < m_numJoints; j++)
	{
		MS3DJoint *joint = &m_joints[j];
		int numPositionKeys = (int)joint->m_usNumTransFrames;
		joint->m_tangent=new tangent[numPositionKeys];

		// clear all tangents (zero derivatives)
		for (int k = 0; k < numPositionKeys; k++)
		{
			joint->m_tangent[k].tangentIn[0] = 0.0f;
			joint->m_tangent[k].tangentIn[1] = 0.0f;
			joint->m_tangent[k].tangentIn[2] = 0.0f;
			joint->m_tangent[k].tangentOut[0] = 0.0f;
			joint->m_tangent[k].tangentOut[1] = 0.0f;
			joint->m_tangent[k].tangentOut[2] = 0.0f;
		}

		// if there are more than 2 keys, we can calculate tangents, otherwise we use zero derivatives
		if (numPositionKeys > 2)
		{
			for (int k = 0; k < numPositionKeys; k++)
			{
				// make the curve tangents looped
				int k0 = k - 1;
				if (k0 < 0)
					k0 = numPositionKeys - 1;
				int k1 = k;
				int k2 = k + 1;
				if (k2 >= numPositionKeys)
					k2 = 0;

				// calculate the tangent, which is the vector from key[k - 1] to key[k + 1]
				float tangent[3];
				tangent[0] = (joint->m_TransKeyFrames[k2].m_fParam[0] - joint->m_TransKeyFrames[k0].m_fParam[0]);
				tangent[1] = (joint->m_TransKeyFrames[k2].m_fParam[1] - joint->m_TransKeyFrames[k0].m_fParam[1]);
				tangent[2] = (joint->m_TransKeyFrames[k2].m_fParam[2] - joint->m_TransKeyFrames[k0].m_fParam[2]);

				// weight the incoming and outgoing tangent by their time to avoid changes in speed, if the keys are not within the same interval
				float dt1 = joint->m_TransKeyFrames[k1].m_fTime - joint->m_TransKeyFrames[k0].m_fTime;
				float dt2 = joint->m_TransKeyFrames[k2].m_fTime - joint->m_TransKeyFrames[k1].m_fTime;
				float dt = dt1 + dt2;
				joint->m_tangent[k1].tangentIn[0] = tangent[0] * dt1 / dt;
				joint->m_tangent[k1].tangentIn[1] = tangent[1] * dt1 / dt;
				joint->m_tangent[k1].tangentIn[2] = tangent[2] * dt1 / dt;

				joint->m_tangent[k1].tangentOut[0] = tangent[0] * dt2 / dt;
				joint->m_tangent[k1].tangentOut[1] = tangent[1] * dt2 / dt;
				joint->m_tangent[k1].tangentOut[2] = tangent[2] * dt2 / dt;

			}
		}
	}
}
void MS3DModel::SetFrame(float frame)
{
	//cout << endl;
	//cout << "current frame " << frame << endl;
	if (frame < 0.0f)
	{
		for (int i = 0; i < m_numJoints; i++)
		{
			MS3DJoint *joint = &m_joints[i];
			memcpy(joint->matLocal, joint->matLocalSkeleton, sizeof(joint->matLocal));
			memcpy(joint->matGlobal, joint->matGlobalSkeleton, sizeof(joint->matGlobal));
		}
	}
	else
	{
		for (int i = 0; i < m_numJoints; i++)
		{
			//cout << endl;
			//cout << "current Joints " << i << endl;
			EvaluateJoint(i, frame);
		}
	}

	m_currentTime = frame;
}
void MS3DModel::EvaluateJoint(int index, float frame)
{
	MS3DJoint *joint = &m_joints[index];

	//
	// calculate joint animation matrix, this matrix will animate matLocalSkeleton
	//
	float pos[3] = { 0.0f, 0.0f, 0.0f };
	int numPositionKeys = joint->m_usNumTransFrames;
	//cout << "numPositionKeys   " << numPositionKeys << endl;
	if (numPositionKeys > 0)
	{
		int i1 = -1;
		int i2 = -1;

		// find the two keys, where "frame" is in between for the position channel
		for (int i = 0; i < (numPositionKeys - 1); i++)
		{
			//cout << "Time "<<joint->m_TransKeyFrames[i].m_fTime << endl;
			if (frame >= joint->m_TransKeyFrames[i].m_fTime && frame < joint->m_TransKeyFrames[i+1].m_fTime)
			{
				i1 = i;
				i2 = i + 1;
				break;
			}
		}
		//cout << "i1 " << i1 << "  i2 " << i2 << endl;
		// if there are no such keys
		if (i1 == -1 || i2 == -1)
		{
			// either take the first
			if (frame < joint->m_TransKeyFrames[0].m_fTime)
			{
				pos[0] = joint->m_TransKeyFrames[0].m_fParam[0];
				pos[1] = joint->m_TransKeyFrames[0].m_fParam[1];
				pos[2] = joint->m_TransKeyFrames[0].m_fParam[2];
			}

			// or the last key
			else if (frame >= joint->m_TransKeyFrames[numPositionKeys - 1].m_fTime)
			{
				pos[0] = joint->m_TransKeyFrames[numPositionKeys - 1].m_fParam[0];
				pos[1] = joint->m_TransKeyFrames[numPositionKeys - 1].m_fParam[1];
				pos[2] = joint->m_TransKeyFrames[numPositionKeys - 1].m_fParam[2];
			}
		}

		// there are such keys, so interpolate using hermite interpolation
		else
		{
			MS3DKeyFrame *p0 = &joint->m_TransKeyFrames[i1];
			MS3DKeyFrame *p1 = &joint->m_TransKeyFrames[i2];
			tangent *m0 = &joint->m_tangent[i1];
			tangent *m1 = &joint->m_tangent[i2];

			// normalize the time between the keys into [0..1]
			float t = (frame - joint->m_TransKeyFrames[i1].m_fTime) / (joint->m_TransKeyFrames[i2].m_fTime - joint->m_TransKeyFrames[i1].m_fTime);
			float t2 = t * t;
			float t3 = t2 * t;

			// calculate hermite basis
			float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
			float h2 = -2.0f * t3 + 3.0f * t2;
			float h3 = t3 - 2.0f * t2 + t;
			float h4 = t3 - t2;

			// do hermite interpolation
			pos[0] = h1 * p0->m_fParam[0] + h3 * m0->tangentOut[0] + h2 * p1->m_fParam[0] + h4 * m1->tangentIn[0];
			pos[1] = h1 * p0->m_fParam[1] + h3 * m0->tangentOut[1] + h2 * p1->m_fParam[1] + h4 * m1->tangentIn[1];
			pos[2] = h1 * p0->m_fParam[2] + h3 * m0->tangentOut[2] + h2 * p1->m_fParam[2] + h4 * m1->tangentIn[2];
		}
		//if(index==5)
		//cout << "pos " << pos[0] << " " << pos[1] << " " << pos[2] << endl;
	}

	float quat[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	int numRotationKeys = joint->m_usNumRotFrames;
	if (numRotationKeys > 0)
	{
		int i1 = -1;
		int i2 = -1;

		// find the two keys, where "frame" is in between for the rotation channel
		for (int i = 0; i < (numRotationKeys - 1); i++)
		{
			if (frame >= joint->m_RotKeyFrames[i].m_fTime && frame < joint->m_RotKeyFrames[i+1].m_fTime)
			{
				i1 = i;
				i2 = i + 1;
				break;
			}
		}

		// if there are no such keys
		if (i1 == -1 || i2 == -1)
		{
			// either take the first key
			if (frame < joint->m_RotKeyFrames[0].m_fTime)
			{
				AngleQuaternion(joint->m_RotKeyFrames[0].m_fParam, quat);
			}

			// or the last key
			else if (frame >= joint->m_RotKeyFrames[numRotationKeys - 1].m_fTime)
			{
				AngleQuaternion(joint->m_RotKeyFrames[numRotationKeys - 1].m_fParam, quat);
			}
		}

		// there are such keys, so do the quaternion slerp interpolation
		else
		{
			float t = (frame - joint->m_RotKeyFrames[i1].m_fTime) / (joint->m_RotKeyFrames[i2].m_fTime - joint->m_RotKeyFrames[i1].m_fTime);
			float q1[4];
			AngleQuaternion(joint->m_RotKeyFrames[i1].m_fParam, q1);
			float q2[4];
			AngleQuaternion(joint->m_RotKeyFrames[i2].m_fParam, q2);
			QuaternionSlerp(q1, q2, t, quat);
		}
	}

	// make a matrix from pos/quat
	float matAnimate[3][4];
	QuaternionMatrix(quat, matAnimate);
	matAnimate[0][3] = pos[0];
	matAnimate[1][3] = pos[1];
	matAnimate[2][3] = pos[2];

	// animate the local joint matrix using: matLocal = matLocalSkeleton * matAnimate
	R_ConcatTransforms(joint->matLocalSkeleton, matAnimate, joint->matLocal);

	// build up the hierarchy if joints
	// matGlobal = matGlobal(parent) * matLocal
	if (joint->parent_index == -1)
	{
		memcpy(joint->matGlobal, joint->matLocal, sizeof(joint->matGlobal));
	}
	else
	{
		MS3DJoint *parentJoint = &m_joints[joint->parent_index];
		R_ConcatTransforms(parentJoint->matGlobal, joint->matLocal, joint->matGlobal);
	}
	//cout << "print matAnimate" << endl;
	//printMatrix(matAnimate);
	//cout << "print matGlobal" << endl;
	//printMatrix(m_joints[index].matGlobal);
	//cout << "print matGlobalSkeleton" << endl;
	//printMatrix(m_joints[index].matGlobalSkeleton);
}
bool MS3DModel::is_In_Model(Point3 p)
{
	if (Box.PointIsOrNotInAABBBoundingBox(p) == 1)
		return true;
	return false;
}
bool MS3DModel::is_Collide(MS3DModel model) {
	if (Box.IsOrNotInterection(&model.Box))
		return true;
	return false;
}
void MS3DModel::setDir(Vector3 dir) {
	direction = dir.normalize();
}
void MS3DModel::Translate(double x, double y, double z) {
	position.x = position.x + x;
	position.y = position.y + y;
	position.z = position.z + z;
	Box.Xmax += x;
	Box.Xmin += x;
	Box.Ymax += y;
	Box.Ymin += y;
	Box.Zmax += z;
	Box.Zmin += z;
}
void MS3DModel::move(Vector3 direction, double speed) {
	this->direction = direction.normalize();
	position.x -= direction.x * speed*0.0005;
	position.y -= direction.y * speed*0.0005;
	position.z -= direction.z * speed*0.0005;
	Box.Xmax -= direction.x * speed*0.0005;
	Box.Xmin -= direction.x * speed*0.0005;
	Box.Ymax -= direction.y * speed*0.0005;
	Box.Ymin -= direction.y * speed*0.0005;
	Box.Zmax -= direction.z * speed*0.0005;
	Box.Zmin -= direction.z * speed*0.0005;
}
void MS3DModel::explosion() {
	if (explode_time > 1000) {
		visible = false;
		return;
	}
	explode_time++;
}
void MS3DModel::render() {
	Vector3 dir2 = Vector3(direction.x, 0, direction.z).normalize();
	double angle1 = acos(dir2.dotProduct(Vector3(0, 0, 1)));
	Vector3 axis = direction.crossProduct(dir2).normalize();
	double angle2 = acos(direction.normalize().dotProduct(dir2));
	Vector3 axis2 = dir2.crossProduct(Vector3(0, 0, 1)).normalize();

	if (visible && !explode) {
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotated(-angle2 * 180 / 3.1416, axis.x, axis.y, axis.z);
		glRotated(-angle1 * 180 / 3.1416, axis2.x, axis2.y, axis2.z);
		glScalef(size, size, size);
		draw();
		glPopMatrix();
	}
	if (visible&&explode) {
		glPushMatrix();
		BoxLists.clear();
		explosion();
		Box.Xmin = 0;
		Box.Xmax = 0;
		Box.Ymin = 0;
		Box.Ymax = 0;
		Box.Zmin = 0;
		Box.Zmax = 0;
		glPopMatrix();
	}
}
void AngleMatrix(const float* angles, float(*matrix)[4])
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	angle = angles[2];
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1];
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0];
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (Z * Y) * X
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr*sp*cy + cr*-sy;
	matrix[1][1] = sr*sp*sy + cr*cy;
	matrix[2][1] = sr*cp;
	matrix[0][2] = (cr*sp*cy + -sr*-sy);
	matrix[1][2] = (cr*sp*sy + -sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}
void R_ConcatTransforms(const float in1[3][4], const float in2[3][4], float out[3][4])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];
}
void AngleQuaternion(float* angles, float* quaternion)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	// FIXME: rescale the inputs to 1/2 angle
	angle = angles[2] * 0.5;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1] * 0.5;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0] * 0.5;
	sr = sin(angle);
	cr = cos(angle);

	quaternion[0] = sr*cp*cy - cr*sp*sy; // X
	quaternion[1] = cr*sp*cy + sr*cp*sy; // Y
	quaternion[2] = cr*cp*sy - sr*sp*cy; // Z
	quaternion[3] = cr*cp*cy + sr*sp*sy; // W
}
void QuaternionMatrix(float* quaternion, float(*matrix)[4])
{
	matrix[0][0] = 1.0 - 2.0 * quaternion[1] * quaternion[1] - 2.0 * quaternion[2] * quaternion[2];
	matrix[1][0] = 2.0 * quaternion[0] * quaternion[1] + 2.0 * quaternion[3] * quaternion[2];
	matrix[2][0] = 2.0 * quaternion[0] * quaternion[2] - 2.0 * quaternion[3] * quaternion[1];

	matrix[0][1] = 2.0 * quaternion[0] * quaternion[1] - 2.0 * quaternion[3] * quaternion[2];
	matrix[1][1] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[2] * quaternion[2];
	matrix[2][1] = 2.0 * quaternion[1] * quaternion[2] + 2.0 * quaternion[3] * quaternion[0];

	matrix[0][2] = 2.0 * quaternion[0] * quaternion[2] + 2.0 * quaternion[3] * quaternion[1];
	matrix[1][2] = 2.0 * quaternion[1] * quaternion[2] - 2.0 * quaternion[3] * quaternion[0];
	matrix[2][2] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[1] * quaternion[1];
}
void QuaternionSlerp(float* p, float* q, float t, float* qt)
{
	int i;
	float omega, cosom, sinom, sclp, sclq;

	// decide if one of the quaternions is backwards
	float a = 0;
	float b = 0;
	for (i = 0; i < 4; i++) {
		a += (p[i] - q[i])*(p[i] - q[i]);
		b += (p[i] + q[i])*(p[i] + q[i]);
	}
	if (a > b) {
		for (i = 0; i < 4; i++) {
			q[i] = -q[i];
		}
	}

	cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

	if ((1.0 + cosom) > 0.00000001) {
		if ((1.0 - cosom) > 0.00000001) {
			omega = acos(cosom);
			sinom = sin(omega);
			sclp = sin((1.0 - t)*omega) / sinom;
			sclq = sin(t*omega) / sinom;
		}
		else {
			sclp = 1.0 - t;
			sclq = t;
		}
		for (i = 0; i < 4; i++) {
			qt[i] = sclp * p[i] + sclq * q[i];
		}
	}
	else {
		qt[0] = -p[1];
		qt[1] = p[0];
		qt[2] = -p[3];
		qt[3] = p[2];
		sclp = sin((1.0 - t) * 0.5 * Q_PI);
		sclq = sin(t * 0.5 * Q_PI);
		for (i = 0; i < 3; i++) {
			qt[i] = sclp * p[i] + sclq * qt[i];
		}
	}
}
void VectorRotate(float const* in1, const float in2[3][4], float * out)
{
	out[0] = DotProduct(in1, in2[0]);
	out[1] = DotProduct(in1, in2[1]);
	out[2] = DotProduct(in1, in2[2]);
}
void VectorIRotate(float const* in1, const float in2[3][4], float *out)
{
	out[0] = in1[0] * in2[0][0] + in1[1] * in2[1][0] + in1[2] * in2[2][0];
	out[1] = in1[0] * in2[0][1] + in1[1] * in2[1][1] + in1[2] * in2[2][1];
	out[2] = in1[0] * in2[0][2] + in1[1] * in2[1][2] + in1[2] * in2[2][2];
}

void VectorTransform(float const* in1, const float in2[3][4], float * out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

void VectorITransform( float const* in1, const float in2[3][4], float* out)
{
	float tmp[3];
	tmp[0] = in1[0] - in2[0][3];
	tmp[1] = in1[1] - in2[1][3];
	tmp[2] = in1[2] - in2[2][3];
	VectorIRotate(tmp, in2, out);
}
void printMatrix(const float in2[3][4]) {
	cout.setf(std::ios::left);
	cout << setw(20) << in2[0][0] << setw(20) << in2[0][1] << setw(20) << in2[0][2]
		<< setw(20) << in2[0][3] << endl;
	cout << setw(20) << in2[1][0] << setw(20) << in2[1][1] << setw(20) << in2[1][2]
		<< setw(20) << in2[1][3] << endl;
	cout << setw(20) << in2[2][0] << setw(20) << in2[2][1] << setw(20) << in2[2][2]
		<< setw(20) << in2[2][3] << endl;
}
void MS3DModel::DrawModelEdge() {
	Box.DrawAABBBoundingBox();
}