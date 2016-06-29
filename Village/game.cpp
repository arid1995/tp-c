/*
	�����(�): ��������, ���������, ������ � ����������
	�������� �������: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"
#include <string>







xGame::xGame(HINSTANCE hInstance) : xDlgScene(hInstance) {
	terrain    = NULL;
	user       = NULL;
	tex_life   = 0u;
	tex_digits = 0u;
	tex_alpha  = 0u;
	tex_cursor = 0u;
	state_game = GAME_MENU;
}





xGame::~xGame() {
}









void  xGame::OnCreate(HDC hDC, HWND hwnd) {

	user    = new xUser(Sound());
	terrain = new xTerrain();  //����� �����
	terrain->LoadTextura(_T("image\\terrain\\floor.jpg")); //��������� ���
	terrain->Create(Sound(), 1024, 16);
	terrain->LoadTerrain(_T("image\\terrain\\map.raw"), 0.3f); // 0.3f	
	//��������� ����� ����� �� map.raw

	skybox.Create(500.0f, 180.0f, 500.0f, -1.0f); //������� ���� 
	skybox.Load(0, _T("image\\skybox\\back.jpg")); //��������� ���� �� ���� ������
	skybox.Load(1, _T("image\\skybox\\front.jpg"));
	skybox.Load(2, _T("image\\skybox\\left.jpg"));
	skybox.Load(3, _T("image\\skybox\\right.jpg"));
	skybox.Load(4, _T("image\\skybox\\top.jpg"));

	tex_digits = xTextura::OpenImageTGA(_T("image\\digits.tga")); //�����
	tex_alpha  = xTextura::OpenImageTGA(_T("image\\alpha.tga")); //�����
	tex_life   = xTextura::OpenImageTGA(_T("image\\life.tga")); //������ ������
	tex_cursor = xTextura::OpenImageTGA(_T("image\\cursor.tga"));  //������

	light_pos = GLVECTOR4(140.0f, 100.0f, -60.0f, 0.0f);
	
	//������� ��������� �����
	xEffect::SetLight(&GLCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 
					  &GLCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					  &GLCOLOR(0.4f, 0.4f, 0.4f, 1.0f));  

	//������ ������� ���������� �����
	terrain->SetLightPos(&light_pos);

	//�������������� ������
	camera.Initialize(GLVECTOR3(160.0f, 3.9f, 0.0f), -45.0f); 

	// ������ ������ ��� ���� � ����� ����
	buttons[0] = sButton(1, _T("������ ������"), Screen().cx / 2 - 140, Screen().cy / 2 + 100, 280, 60,   RGB(30, 100, 255), RGB(230, 100, 155));
	buttons[1] = sButton(2, _T("����� �� ����"), Screen().cx / 2 - 140, Screen().cy / 2 + 190, 280, 60, RGB(30, 100, 255), RGB(230, 100, 155));

	state_game = GAME_MENU;

	//������ � ����  
	camera.CinematicMenu(TRUE);   
}






void  xGame::Initialize(void) {

	//�������������� ������-�����
	camera.CinematicMenu(FALSE); 

	//������ ��������� �������
	camera.Initialize(GLVECTOR3(160.0f, 3.9f, 0.0f), -45.0f);  

	//��������� ��������� ������
	user->Initialize();  
	terrain->SetLightPos(&light_pos);
	terrain->OffsetCameraY(&camera.GetPos());
	terrain->Initialize(&GLVECTOR3(camera.GetPos().x, camera.GetPos().y, -camera.GetPos().z));
}







// ��� �����, ���� ���������� ����� �������� ���������
void  xGame::OnRender(void) {


	if(state_game & GAME_START) {

		// �������� ��������������
		xTextura::Enabled(GL_TRUE);	

		// �������� ���������
		xEffect::BeginLight();   
		xEffect::LightPos(&light_pos);
		user->Display(fElapsed(), fTime());
		xEffect::EndLight();  

		camera.UpdateMove();

		// �������� ��������� ������ ������� �������
		xEffect::ModeCull(GL_CCW); 

		////////////////////// ����� ��������� � ��� ��������
		terrain->BeginMatrix(&GLVECTOR3(0.0f, 0.0f, 0.0f));

		//�������� ����
		skybox.Display(); 

		// �������� ���������
		xEffect::BeginLight();   
		xEffect::LightPos(&light_pos);

		terrain->DisplayObjects();
		terrain->Display();

		terrain->EndMatrix();
		///////////////////////////////////////////////

		
		GLVECTOR3 puser = GLVECTOR3(camera.GetPos().x, camera.GetPos().y, -camera.GetPos().z);
		// ����� �������
		terrain->DisplayRobots(&puser, fElapsed(), fTime());
		// ����� ������

		// ��������� ��������� �����
		xEffect::EndLight();  

		// ����� ��������
		xEffect::ModeClip(0.4f); 
		terrain->DisplayVegetations(&puser, -camera.GetYaw(), fTime(), fElapsed());
		xEffect::EndClip();

		// ��������� ��������������
		xTextura::Enabled(GL_FALSE); 


		////////����� �����//////////
		xEffect::ModeStencil();
		terrain->DisplayShadow();
		terrain->DisplayShadowRobots(&light_pos);
		xEffect::EndStencil(&GLCOLOR(0.0f, 0.0f, 0.0f, 0.3f));
		//////////////////////////////

		// �������� ��������������
		xTextura::Enabled(GL_TRUE); 


		// �������� ����� ����������(blend)
		xEffect::ModeAlpha(&GLCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		terrain->DisplayEffectRobots(&puser, -camera.GetYaw(), fElapsed(), fTime());
		user->DisplayLazer(fElapsed(), fTime(), -camera.GetYaw());
		xEffect::EndAlpha();


		// �������� 2D
		this->Begin2D();
		user->DisplayCross(&Screen());
		
		xEffect::ModeClip(0.6f); 

		//������ �����
		xRect2D::DrawRectTex(tex_life, 5, 5, 32, 32); 

		//����� ����
		xFont::DisplayFontInt(tex_digits, user->GetLife(), 38, 7, 15, 24, &GLVECTOR2(4.0f, 3.0f)); 
		xEffect::EndClip();
		this->End2D();
	
		// ��������� 2D

		// ��������� ��������� back-faces
		xEffect::EndCull(); 
		

		// ������������
		//���������� ������������
		camera.UpdateMove(fElapsed(), fTime()); 
		terrain->CollisionBullet(&puser, user->Bullets());
		if(terrain->CollisionUser(&camera, &user->Size())) {
			// sound
			user->DecLife();

			//�� ������������ �������� GAME OVER
			if(user->GetLife() < 0)	
				state_game = GAME_OVER;
		}

		// ����������� �������
		//������� ������
		camera.MouseMove(NULL, fElapsed()); 

		//����������� ������ - �����
		if(camera.KeyMove(fElapsed(), terrain->CubeSize())) { 
			terrain->OffsetCameraY(&camera.GetPos());
			user->SetMove(TRUE);
		} else
			user->SetMove(FALSE);

			
		// ����� �� ���� � ����� �����
		if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			state_game = GAME_QUIT;
			SendMessage(Handle(), WM_DESTROY, 0, 0);
			return;
		}

	} else if(state_game & GAME_MENU)
		this->DrawMenu();
	 else if(state_game & GAME_OVER)
		this->DrawGameOver();

/*
	if(glGetError() != GL_NO_ERROR)
		logPrint("������");*/
}






// ������� ��������� ��� ������ ������-����
void  xGame::OnMouseDown(WPARAM wParam, int x, int y) {
	if (state_game & GAME_START)

		//����� ��������� ���������� ����� �� ��������
		user->OnFire(wParam, &camera.GetPos(), fElapsed(), camera.GetYaw(), camera.GetPitch());
	else if(state_game & GAME_MENU) {
		POINT pt  = { x, y };
		int   cmd = 0;
		for(int i = 0; i < MAX_BUTTONS; i++) {
			if((cmd = buttons[i].MouseDown(&pt)) != 0) {
				if(cmd & 1) {
					state_game = GAME_START;
					this->Initialize();

					//�������� ������ � ������ ����
					sndPlaySound(_T("sound\\music.wav"), SND_FILENAME | SND_ASYNC | SND_LOOP);  

					//������������� � �������
					Sleep(500uL);
					break;
				} else {

					//���������� ����
					state_game = GAME_QUIT;

					//���������� ��������� � �������� ����
					SendMessage(Handle(), WM_DESTROY, 0, 0);
					break;
				}
			}
		}
	}
}
	
	






//������� ������ �� ����� ����
void  xGame::OnDestroy(void) {
	xTextura::Free(tex_digits);
	xTextura::Free(tex_alpha);
	xTextura::Free(tex_life);
	xTextura::Free(tex_cursor);
	DELETE_HEAP(terrain);
	DELETE_HEAP(user);
	skybox.Destroy();
}







// ����� ������ ������ ����
void  xGame::DrawMenu(void) {

	const TCHAR* descgame = _T("���������� � ���� ��� � ������� �������");

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(Handle(), &pt);

	// �������� ��������������
	xTextura::Enabled(GL_TRUE);	

	glColor4f(0.4f, 0.4f, 0.9f, 1.0f);

	camera.UpdateMove();

	xEffect::ModeCull(GL_CCW);  

	terrain->BeginMatrix(&GLVECTOR3(0.0f, 0.0f, 0.0f));
	skybox.Display();

	xEffect::BeginLight();   
	xEffect::LightPos(&light_pos);

	terrain->DisplayObjects();
	terrain->Display();

	terrain->EndMatrix();

	xEffect::EndLight();

	// ����� ��������
	xEffect::ModeClip(0.4f); 
	GLVECTOR3 puser = GLVECTOR3(camera.GetPos().x, camera.GetPos().y, -camera.GetPos().z);
	terrain->DisplayVegetations(&puser, -camera.GetYaw(), fTime(), fElapsed());
	xEffect::EndClip();

	camera.UpdateMove(fElapsed(), fTime());

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->Begin2D();

		xEffect::ModeAlpha(&GLCOLOR(0.0f, 0.0f, 0.0f, 0.4f));
		xTextura::Enabled(GL_FALSE);
		for(int i = 0; i < MAX_BUTTONS; i++) { // ����� ������
			xRect2D::DrawRect(buttons[i].color, buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height);
			buttons[i].MouseMove(&pt);
		}
		xTextura::Enabled(GL_TRUE);
		xEffect::EndAlpha();

		xRect2D::DrawRectTex(terrain->tHouse(0), Screen().cx / 2 - 220, 40, 440, 142, 2.0f);

		xEffect::ModeClip(0.6f); 

		xFont::DisplayFontText(tex_alpha, descgame, Screen().cx / 2 - _tcslen(descgame) * 14 / 2, 200, 14, 17, &GLVECTOR2(7.0f, 5.0f)); 
		
	
		GLfloat alpha = 0.2f + 0.8f * powf(cosf(fTime() / 300.0f), 2.0f);
		glColor4f(alpha, 1.0f, alpha, 1.0f);
		const TCHAR* str = _T("");
		xFont::DisplayFontText(tex_alpha, str, Screen().cx / 2 - _tcslen(str) * 38 / 2, 70, 38, 80, &GLVECTOR2(7.0f, 5.0f)); 
		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		const TCHAR* author = _T("�������� ��������� ������ � ����������");
		xFont::DisplayFontText(tex_alpha, author, Screen().cx / 2 - _tcslen(author) * 15 / 2, Screen().cy - 30, 15, 20, &GLVECTOR2(7.0f, 5.0f)); 

		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
		for(int c = 0; c < MAX_BUTTONS; c++) // ����� ������� �� ������
			xFont::DisplayFontText(tex_alpha, buttons[c].caption, buttons[c].x + buttons[c].width / 2 - _tcslen(buttons[c].caption) * 15 / 2, buttons[c].y + buttons[c].height / 2 - 20/2, 15, 20, &GLVECTOR2(7.0f, 5.0f)); 
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		xRect2D::DrawRectTex(tex_cursor, pt.x - 16, pt.y - 16, 32, 32);

		xEffect::EndClip();
	this->End2D();

	xTextura::Enabled(GL_FALSE);
}




	
	
// ����� ������ ������ ���������
void  xGame::DrawGameOver(void) {

	const TCHAR* cmds[2] = { _T("������ ������ �����"), _T("����� �������") };

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->Begin2D();
	xRect2D::DrawRectTex(terrain->tSkull(), 0, 0, Screen().cx, Screen().cy, 5.0f);

	xEffect::ModeClip(0.6f); 

	xTextura::Enabled(GL_FALSE);	
	xEffect::ModeAlpha(&GLCOLOR(0.0f, 0.0f, 0.0f, 0.1f), GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	xRect2D::DrawRect(&GLCOLOR(0.4f, 0.2f, 0.1f, 0.7f), Screen().cx / 2 - 300, Screen().cy / 2 - 100, 600, 200); 
	xRect2D::DrawRect(&GLCOLOR(0.0f, 0.4f, 0.3f, 0.8f), Screen().cx / 2 - 200, Screen().cy / 2 + 120, 400, 120); 
	xEffect::EndAlpha();

	xTextura::Enabled(GL_TRUE);

	
	GLfloat alpha = 0.4f + 0.5f * powf(cosf(fTime() / 400.0f), 2.0f);

	glColor4f(alpha, 0.0f, 0.0f, 1.0f);
	const TCHAR* str = _T("�� ��������� �����");
	xFont::DisplayFontText(tex_alpha, str, Screen().cx / 2 - _tcslen(str) * 30 / 2, Screen().cy / 2 - 40 / 2, 30, 40, &GLVECTOR2(7.0f, 5.0f)); 


	// ����� �������� ������� ������
	glColor4f(alpha, alpha, 0.5f, 1.0f);
	xFont::DisplayFontText(tex_alpha, cmds[0], Screen().cx / 2 - _tcslen(cmds[0]) * 20 / 2, Screen().cy / 2 + 140, 20, 30, &GLVECTOR2(7.0f, 5.0f)); 
	xFont::DisplayFontText(tex_alpha, cmds[1], Screen().cx / 2 - _tcslen(cmds[1]) * 20 / 2, Screen().cy / 2 + 190, 20, 30, &GLVECTOR2(7.0f, 5.0f)); 

	xEffect::EndClip();

	this->End2D();

	// ����� �� ����
	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		state_game = GAME_QUIT;
		SendMessage(Handle(), WM_DESTROY, 0, 0);
	} else if(GetAsyncKeyState(VK_RETURN) & 0x8000) { // ������ ������ ������
		state_game = GAME_START;
		this->Initialize();
		Sleep(500uL);
	}
}