/*
	�����(�): ��������, ���������, ������ � ����������
	�������� �������: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"







// ������� ����������� ����������� ��������� ����� BMP/DIB, WMF/EMF, JPG/JPEG/JFIF, GIF, ICO/CUR
GLuint  xTextura::OpenImage(const TCHAR* filename, GLboolean mapping) {

	IPicture* picture = xTextura::PictureLoad(filename);
	if(picture == NULL)
		return 0u;
		
	HBITMAP bitmap = NULL;
	if(picture->get_Handle((OLE_HANDLE*)(HBITMAP*)&bitmap) != S_OK) {
		picture->Release();
		return 0u;
	}

	BITMAP info;
	ZeroMemory(&info, sizeof(BITMAP));
	if(! GetObject(bitmap, sizeof(BITMAP), (LPVOID)&info)) {
		picture->Release();
		return 0u;
	}

	// ���������� ������ 24-32(TRUE COLOR) ������ ������
	if(info.bmBitsPixel < 24u) {
		picture->Release();
		return 0u;
	}


	DWORD size = (DWORD)info.bmWidth * info.bmHeight * (info.bmBitsPixel >> 3);
	GLubyte*  buf = new  GLubyte[size];
	if(buf == NULL) {
		picture->Release();
		return 0u;
	}
          
	GetBitmapBits(bitmap, size, (LPVOID)buf);  // �������� ������ �������� ��������
	picture->Release();

	GLubyte* rastrs = new GLubyte[info.bmWidth * info.bmHeight * 4L];
	if(rastrs == NULL) {
		delete[] buf;
		return 0u;
	}

	info.bmBitsPixel >>= 3;

	if(info.bmBitsPixel == 3) { // 24-���
		RGBTRIPLE color;
		for(long y = 0L; y < info.bmHeight; y++) {
			for(long x = 0L; x < info.bmWidth; x++) {
				color = *(RGBTRIPLE*) ((buf) + (y * info.bmWidth + x) * info.bmBitsPixel);
				*(DWORD*)((rastrs) + (y * info.bmWidth + x) * 4) = (DWORD) 0xFF000000uL | (color.rgbtBlue << 16) | (color.rgbtGreen << 8) | color.rgbtRed;
			}
		}
	} else if(info.bmBitsPixel == 4) {  //32-���
		RGBQUAD color;
		for(long y = 0L; y < info.bmHeight; y++) {
			for(long x = 0L; x < info.bmWidth; x++) {
				color = *(RGBQUAD*) ((buf) + (y * info.bmWidth + x) * info.bmBitsPixel);
				*(DWORD*)((rastrs) + (y * info.bmWidth + x) * 4) = (DWORD) 0xFF000000uL | (color.rgbBlue << 16) | (color.rgbGreen << 8) | color.rgbRed;
			}
		}
	}

	delete[] buf;
	buf = NULL;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	GLuint id = 0u;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if(mapping == GL_TRUE) {
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  (GLint) info.bmWidth,  (GLint) info.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rastrs);

	delete[] rastrs;
	rastrs = NULL;

	return id;
}







// ������� ��������� TGA-���� ������(��� ������ RLE) 32-������(���������� ��� �����-������ color key)
GLuint  xTextura::OpenImageTGA(const TCHAR* filename) {

	TGAFILEHEADER hdrtga;
	ZeroMemory(&hdrtga, sizeof(TGAFILEHEADER));

	HANDLE fp = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fp == INVALID_HANDLE_VALUE)
		return 0u;

	DWORD dread = 0uL;
	ReadFile(fp, (LPVOID)&hdrtga, sizeof(TGAFILEHEADER), &dread, NULL);

	if((dread != sizeof(TGAFILEHEADER)) || (hdrtga.pixel_size < 32)) { // ���������� ������ 32 ������ � ����� �������
		CloseHandle(fp);
		return 0u;
	}

	hdrtga.pixel_size >>= 3;
	DWORD image_size    = DWORD(hdrtga.width * hdrtga.height * hdrtga.pixel_size); // ��������� ������ ������

	BYTE*  buf = new BYTE[image_size];
	if(buf == NULL) {  // �� ����� ��������, �������� ������(� ������������ 100% ��� PDP-11 ������ � �������)
		CloseHandle(fp);
		return 0u;
	}

	ReadFile(fp, (LPVOID)buf, image_size, &dread, NULL);
	CloseHandle(fp);

	// �������� �������� ������ � ������ �������� RGB �� BGR
	BYTE     tmp;
	RGBQUAD* prgb;
	for(BYTE* ptr = buf; ptr < buf + image_size; ptr += hdrtga.pixel_size) {
		prgb          = (RGBQUAD*)ptr;
		tmp           = prgb->rgbRed;
		prgb->rgbRed  = prgb->rgbBlue;
		prgb->rgbBlue = tmp;
	}

	GLuint id = 0u;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  (GLint)hdrtga.width, (GLint)hdrtga.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

	delete[] buf;
	buf = NULL;
	return id;
}






// ������� ����������� ����������� ��������� ����� BMP/DIB, WMF/EMF, JPG/JPEG/JFIF, GIF, ICO/CUR
// � ������������ � ����� Microsoft Visual C++ 6.0 � �� ����� 1998 ���� ��� �� ���� ������� � Microsoft � GDI+
// ������� � Visual C++ 7.0 � ������ ������ ������ ���������� atlimage ������� ��������� ������ CImage,
// �� ��������� �������� � ��������� ������ BMP, JPG, EMF, GIF, PNG, TIF � �.�.
IPicture*  xTextura::PictureLoad(const TCHAR* filename) {

	HGLOBAL    mem     = NULL;
	DWORD      dr      = 0uL;
	IStream*   stream  = NULL;
	IPicture*  picture = NULL;
	DWORD      size    = 0uL;

	HANDLE fp = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fp == INVALID_HANDLE_VALUE)
		return  NULL;

#ifdef _WIN64
	LARGE_INTEGER fsize = {0};
	GetFileSizeEx(fp, &fsize);
	size = static_cast<DWORD>(fsize.QuadPart);
#else
	size = GetFileSize(fp, NULL);
	if(size == INVALID_FILE_SIZE)
		dr = GetFileSize(fp, &size);
#endif

	if((size == 0uL) || (size == INVALID_FILE_SIZE)) {
		size = SetFilePointer(fp, 0L, NULL, FILE_END);
		SetFilePointer(fp, 0L, NULL, FILE_BEGIN);
	}

	if((mem = GlobalAlloc(GMEM_FIXED, size)) == NULL) {
		CloseHandle(fp);
		return NULL;
	}
	
	LPBYTE  ptr = (LPBYTE)GlobalLock(mem);
	ReadFile(fp, (LPVOID)ptr, size, &dr, NULL);
	CloseHandle(fp);

	if(CreateStreamOnHGlobal((HGLOBAL)ptr, TRUE, &stream) != S_OK) {
		GlobalUnlock(mem);
		GlobalFree(mem);
		return NULL;
	}

	if(OleLoadPicture(stream, 0, TRUE, IID_IPicture, (LPVOID*)&picture) != S_OK) {
		GlobalUnlock(mem);
		GlobalFree(mem);
		return NULL;
	}
	GlobalUnlock(mem);
	GlobalFree(mem);
	return picture;
}




void  xTextura::Set(GLuint tid) {
	glBindTexture(GL_TEXTURE_2D, tid);
}



void  xTextura::Enabled(GLboolean enabled) {
	if(enabled != GL_TRUE)
		glDisable(GL_TEXTURE_2D);
	else
		glEnable(GL_TEXTURE_2D);
}



// ����� ������� �������� �� �����������
void  xTextura::Free(GLuint tid) {
	if(glIsTexture(tid) == GL_TRUE)
		glDeleteTextures(1, &tid);
}





//****************************************************************************************************


xFigure::xFigure(void) {
	obj     = NULL;
	id      = 0u;
}

	


xFigure::~xFigure() {
	this->Destroy();
}





void  xFigure::CreateSphere(GLdouble radius, GLint slices, GLint stacks) {
	id  = glGenLists(1);	
	obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GLU_SMOOTH);
	glNewList(id, GL_COMPILE);
	gluSphere(obj, radius, slices, stacks);
	glEndList();
}





void  xFigure::CreateDisk(GLdouble radius1, GLdouble radius2, GLint slices, GLint loops) {
	id  = glGenLists(1);	
	obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GLU_SMOOTH);
	glNewList(id, GL_COMPILE);
	gluDisk(obj, radius1, radius2, slices, loops);
	glEndList();
}





void  xFigure::CreateCylinder(GLdouble radius1, GLdouble radius2, GLdouble height, GLint slices, GLint stacks) {
	id  = glGenLists(1);	
	obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GLU_SMOOTH);
	glNewList(id, GL_COMPILE);
	gluCylinder(obj, radius1, radius2, height, slices, stacks);
	glEndList();
}



	

GLuint  xFigure::CreateCube(GLfloat width, GLfloat height, GLfloat depth, const GLVECTOR3& pos) {
	GLuint id = glGenLists(1);
	glNewList(id, GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(  width + pos.x, pos.y + height, depth + pos.z); // �������� ������
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( -width + pos.x, pos.y + height, depth + pos.z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f( -width + pos.x, pos.y - height, depth + pos.z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(  width + pos.x, pos.y - height, depth + pos.z);

			glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f( -width + pos.x, pos.y + height, -depth + pos.z); // ������ ������
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(  width + pos.x, pos.y + height, -depth + pos.z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(  width + pos.x, pos.y - height, -depth + pos.z);
			glTexCoord2f(1.0f, 1.0f);		
			glVertex3f( -width + pos.x, pos.y - height, -depth + pos.z);

			glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f( -width + pos.x, pos.y + height,  depth + pos.z);  // ����� ������
			glTexCoord2f(0.0f, 0.0f);		
			glVertex3f( -width + pos.x, pos.y + height, -depth + pos.z);
			glTexCoord2f(0.0f, 1.0f);		
			glVertex3f( -width + pos.x, pos.y - height, -depth + pos.z);
			glTexCoord2f(1.0f, 1.0f);		
			glVertex3f( -width + pos.x, pos.y - height,  depth + pos.z);
				
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f( width + pos.x, pos.y + height, -depth + pos.z);  // ������ ������
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( width + pos.x, pos.y + height,  depth + pos.z);
			glTexCoord2f(0.0f, 1.0f);		
			glVertex3f( width + pos.x, pos.y - height,  depth + pos.z);
			glTexCoord2f(1.0f, 1.0f);		
			glVertex3f( width + pos.x, pos.y - height, -depth + pos.z);
				
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f( -width + pos.x, pos.y - height, -depth + pos.z);  // ������ ������
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(  width + pos.x, pos.y - height, -depth + pos.z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(  width + pos.x, pos.y - height,  depth + pos.z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f( -width + pos.x, pos.y - height,  depth + pos.z);

			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(  width + pos.x, pos.y + height, -depth + pos.z);  // ������� ������
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( -width + pos.x, pos.y + height, -depth + pos.z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f( -width + pos.x, pos.y + height,  depth + pos.z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(  width + pos.x, pos.y + height,  depth + pos.z);
		glEnd();
	glEndList();
	return id;
}




	
void  xFigure::Display(void) {
	glCallList(id);
}
	




void  xFigure::Display(const LPGLVECTOR3& hpos) {
	glPushMatrix();
	glTranslatef(hpos->x, hpos->y, hpos->z);
	glCallList(id);
	glPopMatrix();
}





void  xFigure::Display(const LPGLVECTOR3& hpos, const LPGLVECTOR3& hrot) {
	glPushMatrix();
	glTranslatef(hpos->x, hpos->y, hpos->z);
	glRotatef(hrot->x, 1.0f, 0.0f, 0.0f);
	glRotatef(hrot->y, 0.0f, 1.0f, 0.0f);
	glRotatef(hrot->z, 0.0f, 0.0f, 1.0f);
	glCallList(id);
	glPopMatrix();
}





void  xFigure::Display(const LPGLVECTOR3& hpos, const LPGLVECTOR3 hrot, const LPGLVECTOR3& hscale) {
	glPushMatrix();
	glTranslatef(hpos->x, hpos->y, hpos->z);
	glScalef(hscale->x, hscale->y, hscale->z);

	if(hrot != NULL) {
		glRotatef(hrot->x, 1.0f, 0.0f, 0.0f);
		glRotatef(hrot->y, 0.0f, 1.0f, 0.0f);
		glRotatef(hrot->z, 0.0f, 0.0f, 1.0f);
	}

	glCallList(id);
	glPopMatrix();
}




void  xFigure::Destroy(void) {
	if(obj != NULL)
		gluDeleteQuadric(obj);
	obj = NULL;
	glDeleteLists(id, 1);
	id = 0u;
}





void   xFigure::DestroyCube(GLuint id) {
	glDeleteLists(id, 1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////





void  xCamera::Initialize(const GLVECTOR3& pos, GLfloat lock_up, GLfloat lock_down) {
	deg_roll   = 0.0f;
	deg_yaw    = 0.0f;
	deg_pitch  = 0.0f;
	lup        = lock_up;
	ldown      = lock_down;
	camera_pos = pos;
	c_width    = GetSystemMetrics(SM_CXSCREEN) / 2;
	c_height   = GetSystemMetrics(SM_CYSCREEN) / 2;
	look       = 0.0f;
	streif     = 0.0f;
	uron       = FALSE;
	vec_uron   = 0.0f;
}





// ����� �� ���������� ������(������� ����) gl-��������� dx-�������
void  xCamera::UpdateMove(void) {
	 
	GLfloat mat[16];
	GLQUATERNION pitch = 0.0f;
	GLQUATERNION head  = 0.0f;

	glLoadIdentity();

	QuaternionFromAxisAngle(&pitch, &GLVECTOR3(1.0f, 0.0f, 0.0f), deg_pitch); // ���-X
	QuaternionFromAxisAngle(&head,  &GLVECTOR3(0.0f, 1.0f, 0.0f), deg_yaw);   // ���-Y

	// ����������� ����������
	GLQUATERNION q = pitch * head;

	// ������� ������� 4x4 �� �����������
	MatrixIdentity(mat);
	QuaterionToMatrix(&q, mat);  

	look   = GLVECTOR3(mat[8], mat[9], mat[10]);
	streif = GLVECTOR3(mat[0], mat[1], mat[2]);

	// ����������� MODELVIEW-�������
	glMultMatrixf(mat);

	glTranslatef(-camera_pos.x, -camera_pos.y, camera_pos.z);
}







// ����� ��� �������� ������ ��� ������ ���� ������ ����� ��� �� ����-X,Y
void  xCamera::MouseMove(HWND hwnd, GLfloat felapsed)  {
	POINT pt;
	GetCursorPos(&pt);

	if(hwnd != NULL)
		ScreenToClient(hwnd, &pt);

	GLfloat speed = fNUM(felapsed, 1.9f);
	GLfloat delta = 0.0f;

	// �������� �� ���-�
	delta = (pt.x < c_width) ? (-GLfloat(c_width - pt.x) * speed) : (GLfloat(pt.x - c_width) * speed);
	deg_yaw += delta;

	if(pt.y < c_height) {  // �������� �� ���-Y
		delta      = GLfloat(c_height - pt.y) * speed;
		deg_pitch += -delta;

		if(deg_pitch < lup) 
			deg_pitch += delta;

	} else if(pt.y > c_height) {
		delta      = GLfloat(pt.y - c_height) * speed;
		deg_pitch += delta;

		if(deg_pitch > ldown) 
			deg_pitch += -delta;
	}

	SetCursorPos(c_width, c_height);
}







// ����� ��� ����������� ������� ��� ������ ������ ����������
bool  xCamera::KeyMove(GLfloat felapsed, GLfloat lock_cube) {
	GLfloat num = fNUM(felapsed, 40.0f); // 20.0f
	bool  iskey = false;

	BYTE keys[256];
	if(GetKeyboardState(keys)) {  // ��������� ��������� ������

		if((keys[VK_LEFT] & 0x80) || (keys['A'] & 0x80)) {
			this->move_left_right(-num);
			iskey = true;
		} else if((keys[VK_RIGHT] & 0x80) || (keys['D'] & 0x80)) { 
			this->move_left_right(num);
			iskey = true;
		}

		if((keys[VK_UP] & 0x80) || (keys['W'] & 0x80)) {
			this->move_back_front(num);
			iskey = true;
		} else if((keys[VK_DOWN] & 0x80) || (keys['S'] & 0x80)) {
			this->move_back_front(-num);
			iskey = true;
		}
	}
		
	// ���������� �������� �� ����� ����� ������ �� SKYBOX
	if(camera_pos.x > lock_cube)
		camera_pos.x = lock_cube;
	else if(camera_pos.x < -lock_cube)
		camera_pos.x = -lock_cube;

	if(camera_pos.z > lock_cube)
		camera_pos.z = lock_cube;
	else if(camera_pos.z < -lock_cube)
		camera_pos.z = -lock_cube;

	return iskey;
}





// ����� ��������� �����-������� �� �����
void  xCamera::StartUron(const LPGLVECTOR3& pvec, int type) {
	uron     = TRUE;
	vec_uron = (type & 2) ? *pvec * 8.0f : *pvec * 4.0f;
}





// ��������� �������� ������ � �.�.
void  xCamera::UpdateMove(GLfloat felapsed, GLfloat ftime) {

	GLfloat num;

	if(uron) { // ��������� �����
		num = fNUM(felapsed, 30.0f);
		camera_pos.x -= vec_uron.x * num;
		camera_pos.z += vec_uron.z * num;
		vec_uron     *= 0.9f;
		if((vec_uron.x < 0.01f && vec_uron.z < 0.01f))
			uron = FALSE;
	}

	
	if(imenu) { // �������� ������� � ����
		static GLfloat angle = 0.0f;
		deg_yaw      = fDEGREE(atan2f(-camera_pos.z, camera_pos.x));
		camera_pos.x = 160.0f * cosf(angle);
		camera_pos.z = 160.0f * sinf(angle);
		angle       += fNUM(felapsed, 0.2f);
	}
}






// ����������� ������ ����� � ������(������)
void  xCamera::move_left_right(GLfloat velocity) {
	GLVECTOR3    vec;
	GLQUATERNION quat;
	QuaternionPitchYawRoll(&quat, deg_yaw, deg_pitch, deg_roll);  // ������� ���� ������ � ����������
	QuaternionFromVector(&vec, &quat, &streif);  // ���������� ���������� �� ������ mat[0], mat[1], mat[2]
	vec.y      = 0.0f;
	camera_pos += vec * velocity;
}






// ����������� ������ ����� � �����
void  xCamera::move_back_front(GLfloat velocity) {
	GLVECTOR3    vec;
	GLQUATERNION quat;
	QuaternionPitchYawRoll(&quat, deg_yaw, deg_pitch, deg_roll);  // ������� ���� ������ � ����������
	QuaternionFromVector(&vec, &quat, &look);  // ���������� ���������� ��  ������ mat[8] mat[9] mat[10]
	vec.y      = 0.0f;
	camera_pos += vec * velocity;
}






// ��������� ������ ������ ��� ���������� ���������
void  xCamera::SetY(GLfloat h) {
	camera_pos.y = h;
}





