/*
  �����(�): ��������, ���������, ������ � ����������
  �������� �������: AGRAGE
  e-mail:   sdk96@mail.ru
*/
#if ! defined(_GLMODEL_VILLAGE_)
#define _GLMODEL_VILLAGE_
#pragma once








// ����� �� ������ ������� ���������������
class  xModel {
public:

	static  void  DrawHouse(const LPGLVECTOR3& pos, const LPGLVECTOR3& size, GLuint thigh, GLuint tlow) {
		GLfloat h = size->y + size->y;
	
		const GLfloat tarr_a[4][2] = {	{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
		const GLfloat tarr_b[4][2] = {  {0.0f, 0.0f}, {0.0f, 2.0f}, {2.0f, 0.0f}, {2.0f, 2.0f} };

		xTextura::Set(thigh);

		glBegin(GL_TRIANGLE_STRIP);	// ����� 

			glTexCoord2f(0.0f, 0.0f);// �������� �������
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_a[1]);
			glVertex3f(pos->x, pos->y + h, pos->z + size->z);

			glTexCoord2fv(tarr_a[2]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[0]);// ����� �������
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z); 

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x, pos->y + h, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x, pos->y + h, pos->z - size->z);

			glTexCoord2f(0.0f, 0.0f);// ������ �������
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(pos->x , pos->y + h, pos->z - size->z); 

			glTexCoord2fv(tarr_a[1]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_a[2]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[0]);// ������ �������
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z); 

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x, pos->y + h, pos->z - size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x, pos->y + h, pos->z + size->z);

		glEnd();

		
		xTextura::Set(tlow);

		glBegin(GL_TRIANGLE_STRIP); // ���

			glTexCoord2fv(tarr_b[0]); // �������� �������
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);  

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[0]);// ����� �������
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z + size->z);  

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z);
			
			glTexCoord2fv(tarr_b[0]); // ������ �������
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z); 

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[0]);// ������ �������
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z - size->z);  

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);

		glEnd();
	}



	/////////////////////////////////////////////////////////////////////////////////////


	static  void  ShadowHouse(const LPGLVECTOR3& pos, const LPGLVECTOR3& size, GLfloat* mat) {
			
		GLfloat h  = size->y + size->y;
	
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glMultMatrixf(mat);
	
		glBegin(GL_TRIANGLE_STRIP);	// ����� 
			glVertex3f(size->x, size->y, size->z);
			glVertex3f( 0.0f, h, size->z);
			glVertex3f(-size->x, size->y, size->z);
			glVertex3f(-size->x, size->y, size->z); 
			glVertex3f(-size->x, size->y, -size->z);
			glVertex3f( 0.0f, h, size->z);
			glVertex3f( 0.0f, h, -size->z);
			glVertex3f( 0.0f, h, -size->z); 
			glVertex3f(-size->x, size->y, -size->z);
			glVertex3f( size->x, size->y, -size->z);
			glVertex3f( size->x, size->y, -size->z); 
			glVertex3f(0.0f, h, -size->z);
			glVertex3f( size->x, size->y, size->z);
			glVertex3f( 0.0f, h, size->z);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP); // ���
			glVertex3f(size->x, size->y, size->z);  
			glVertex3f(-size->x, size->y, size->z);
			glVertex3f(size->x, -size->y, size->z);
			glVertex3f(-size->x, -size->y, size->z);
			glVertex3f(-size->x, -size->y, size->z);  
			glVertex3f(-size->x,  size->y, size->z);
			glVertex3f(-size->x, -size->y, -size->z);
			glVertex3f(-size->x,  size->y, -size->z);
			glVertex3f(-size->x, size->y, -size->z); 
			glVertex3f(size->x,  size->y, -size->z);
			glVertex3f(-size->x, -size->y, -size->z);
			glVertex3f(size->x,  -size->y, -size->z);
			glVertex3f(size->x, -size->y, -size->z);  
			glVertex3f(size->x,  size->y, -size->z);
			glVertex3f(size->x, -size->y, size->z);
			glVertex3f(size->x,  size->y, size->z);
		glEnd();
		glPopMatrix();
	}


	// ������� ��������� ��������������
	static void  DrawRect(const LPGLVECTOR3& pos, GLfloat size, GLfloat angleY) {
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP); 
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(size, size,   0.0f);  
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-size, size,  0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(size, -size,  0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-size, -size, 0.0f);
		glEnd();
		glPopMatrix();
	}



	// ����� ��������� ������
	static void  DrawAnimationSprite(const LPGLVECTOR3& pos, const LPGLVECTOR3& size, GLfloat rotY, const LPGLVECTOR2& cadr_pos, const LPGLVECTOR2& cadr_size) {
		GLfloat tarr[4][2] = {  
			{ cadr_pos->x, cadr_pos->y                               },
			{ cadr_pos->x, cadr_pos->y + cadr_size->y                },
			{ cadr_pos->x + cadr_size->x, cadr_pos->y                },
			{ cadr_pos->x + cadr_size->x, cadr_pos->y + cadr_size->y }
		};
	
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP); 
			glTexCoord2fv(tarr[0]);
			glVertex3f( size->x,   size->y, size->z);
			glTexCoord2fv(tarr[1]);
			glVertex3f(-size->x,   size->y, size->z);
			glTexCoord2fv(tarr[2]);
			glVertex3f( size->x,  -size->y, size->z);
			glTexCoord2fv(tarr[3]);
			glVertex3f(-size->x,  -size->y, size->z);
		glEnd();
		glPopMatrix();
	}

};





////////////////////////////////////////////////////////////////////////////////////////////////////




// ����� ������������� �� �����
class  xRobotTM {
private:
	xFigure circle;
	GLuint  id_cube;
	GLuint  tex_circle;
	GLuint  tex_head;
public:

	// ��������
	void  Create(const TCHAR* texture_head, const TCHAR*  texture_circle) {
		id_cube  = xFigure::CreateCube(2.2f, 3.0f, 2.0f, GLVECTOR3(0.0f, 4.4f, 0.0f));
		circle.CreateSphere(1.4, 10, 10);
		tex_circle = xTextura::OpenImage(texture_circle);
		tex_head   = xTextura::OpenImage(texture_head, GL_TRUE);
	}

	// ����� �� �������
	void  Display(const LPGLVECTOR3& pos, const LPGLVECTOR3& rot, GLfloat rot_circle) {
		glPushMatrix();
		glTranslatef(pos->x, pos->y + 2.0f, pos->z);
		glRotatef(rot->y, 0.0f, 1.0f, 0.0f); 
		glRotatef(rot->z, 0.0f, 0.0f, 1.0f);   

		xTextura::Set(tex_head);
		glCallList(id_cube);
		triangle();

		glRotatef(rot_circle, 1.0f, 0.0f, 0.0f);  // �������� �������
		xTextura::Set(tex_circle);
		circle.Display();

		glPopMatrix();
	}


	// ����� �� �������
	void  DisplayShadow(const LPGLVECTOR3& pos, const LPGLVECTOR3& rot, GLfloat* mat) {
		glPushMatrix();
		glMultMatrixf(mat);
		glTranslatef(pos->x, pos->y + 2.0f, pos->z);
		glRotatef(rot->y, 0.0f, 1.0f, 0.0f); 
		glCallList(id_cube);
		triangle(true);
		glPopMatrix();
	}


	// �����������
	void  Destroy(void) {
		xFigure::DestroyCube(id_cube);
		circle.Destroy();
		xTextura::Free(tex_head);
		xTextura::Free(tex_circle);
	}


	GLuint GetTex(void) {
		return tex_head;
	}

private:

	// ������� ������������ ��� ���� ����� �� ������������� � ����� ��������� �� ������� ������� GL_CW
	void  triangle(bool shadow = false) {

		if(! shadow) {
			xTextura::Enabled(GL_FALSE);
			glColor4f(0.5f, 0.5f, 0.5f, 1.0f); 
		}

		glBegin(GL_TRIANGLES);  // ������������ ��� �������
			glNormal3f(-1.0f, 0.0f,  0.0f);
			glVertex3f(-2.1f, 1.7f,  1.0f);
			glVertex3f(-2.1f, 1.7f, -1.0f);
			glVertex3f(-1.2f, 0.0f,  0.0f);
			glVertex3f(-2.1f, 1.7f, -1.0f);
			glVertex3f(-2.1f, 1.7f,  1.0f);
			glVertex3f(-1.2f, 0.0f,  0.0f);
			glNormal3f(1.0f,  0.0f,  0.0f);
			glVertex3f(2.1f,  1.7f,  1.0f);
			glVertex3f(2.1f,  1.7f, -1.0f);
			glVertex3f(1.2f,  0.0f,  0.0f);
			glVertex3f(2.1f,  1.7f, -1.0f);
			glVertex3f(2.1f,  1.7f,  1.0f);
			glVertex3f(1.2f,  0.0f,  0.0f);
		glEnd();
		glBegin(GL_TRIANGLES);  // ������� �����
			glColor4f(0.7f, 0.3f, 0.1f, 1.0f); 
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 7.0f,  2.0f);
			glVertex3f(0.0f, 9.5f, -4.0f);
			glVertex3f(0.0f, 7.0f, -2.0f);
			glNormal3f(1.0f, 0.0f,  0.0f);
			glVertex3f(0.0f, 7.0f,  2.0f);
			glVertex3f(0.0f, 7.0f, -2.0f);
			glVertex3f(0.0f, 9.5f, -4.0f);
		glEnd();

		if(! shadow) {
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			xTextura::Enabled(GL_TRUE);
		}
	}

};




/////////////////////////////////////////////////////////////////////////////////////////////////////





// �������� ����� � �������� ��� �����
class  xRobotFly {
private:
	xFigure  circle;
	GLuint   tex_circle;
public:

	// ��������
	void  Create(const TCHAR*  texture_circle) {
		circle.CreateSphere(3.0, 14, 14);
		tex_circle = xTextura::OpenImage(texture_circle);
	}


	// ����� �� �������
	void  Display(const LPGLVECTOR3& pos, GLfloat angleY, GLfloat fangle) {

		GLfloat isin = sinf(fangle);

		xTextura::Set(tex_circle);
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);

		circle.Display();

		glRotatef(30.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(false);

		glRotatef(-60.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(true);

		glPopMatrix();
	}


	// ����� �� �������
	void  DisplayShadow(const LPGLVECTOR3& pos, GLfloat angleY, GLfloat fangle, GLfloat* mat) {
		GLfloat isin = sinf(fangle);
		glPushMatrix();
		glMultMatrixf(mat);
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);
		circle.Display();

		glRotatef(30.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(false, true);

		glRotatef(-60.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(true, true);

		glPopMatrix();
	}


	// �����������
	void  Destroy(void) {
		circle.Destroy();
		xTextura::Free(tex_circle);
	}

private:

	// ������ "�������" ������� ��������� ������� ����� ������ ��� �� �������� ��������� �������� ������
	void  triangle(bool left, bool shadow = false) {

		if(! shadow) {
			glColor4f(0.9f, 0.9f, 0.0f, 1.0f);
			xTextura::Enabled(GL_FALSE);
		}

		if(! left) {
			glBegin(GL_TRIANGLE_STRIP);  // ������ �����
				glNormal3f(0.0f, -1.0f,  0.0f);
				glVertex3f(2.8f, 0.5f,  1.5f);
				glVertex3f(2.8f, 0.5f, -1.5f);
				glVertex3f(9.5f, 0.5f,  1.5f);
				glVertex3f(9.5f, 0.5f, -1.5f);
				glNormal3f(0.0f, 1.0f,  0.0f);
				glVertex3f(9.5f, 0.5f, -1.5f);
				glVertex3f(2.8f, 0.5f, -1.5f);
				glVertex3f(9.5f, 0.5f,  1.5f);
				glVertex3f(2.8f, 0.5f,  1.5f);
			glEnd();
		} else {
			glBegin(GL_TRIANGLE_STRIP);   // ����� �����
				glNormal3f(0.0f, 1.0f,  0.0f);
				glVertex3f(-2.8f, 0.5f,  1.5f);
				glVertex3f(-2.8f, 0.5f, -1.5f);
				glVertex3f(-9.5f, 0.5f,  1.5f);
				glVertex3f(-9.5f, 0.5f, -1.5f);
				glNormal3f(0.0f, -1.0f,  0.0f);
				glVertex3f(-9.5f, 0.5f, -1.5f);
				glVertex3f(-2.8f, 0.5f, -1.5f);
				glVertex3f(-9.5f, 0.5f,  1.5f);
				glVertex3f(-2.8f, 0.5f,  1.5f);
			glEnd();
		}

		if(! shadow) {
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			xTextura::Enabled(GL_TRUE);
		}
	}

};







#endif