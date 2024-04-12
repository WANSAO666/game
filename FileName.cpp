#include<graphics.h>
#include<string>
#include<iostream>
#include<vector>
using namespace std;
#pragma comment(lib,"MSIMG32.LIB")
const int FPS = 144;//֡��
const int NUMS = 6;//֡��
const int button_gamebegin_x = 500;//��ʼ��Ϸ��ť����
const int button_gamebegin_y = 400;
const int button_gameexit_x = 500;//�˳���Ϸ��ť����
const int button_gameexit_y = 500;
int player_x = 0;//���x����
int player_y = 400;//���y����
int player_speed = 2;//��ұ����ٶ�
int scene_now = 0;//Ŀǰ����һĻ
bool running = true;
ExMessage msg;
IMAGE button_game_begin;
IMAGE button_game_exit;
IMAGE player;
IMAGE ui_background;//���˵�����
IMAGE atlas_player_stand_right[NUMS];//���վ������ͼ��
IMAGE atlas_player_stand_left[NUMS];//���վ������ͼ��
IMAGE atlas_player_run_right[NUMS];//��ұ��ܳ���ͼ��
IMAGE atlas_player_run_left[NUMS];//��ұ��ܳ���ͼ��
IMAGE atlas_enemy_right[NUMS];//���˳��ұ���ͼ��
IMAGE atlas_enemy_left[NUMS];//���˳�����ͼ��
IMAGE atlas_speak[NUMS];//�Ի�ͼ��
inline void putimage_alpha(int x, int y, IMAGE* img) {//�ܴ���͸����������Ⱦ����
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
void loadall() {
	loadimage(&button_game_begin, _T("img/ui_begin.png"), 192, 75);
	loadimage(&button_game_exit, _T("img/ui_exit.png"), 192, 75);
	loadimage(&ui_background, _T("img/ui_background.png"), 1280, 720);
	loadimage(&player, _T("img/player.png"), 200, 200);
	for (int i = 0; i < NUMS; i++) {
		static TCHAR img_path[256];
		_stprintf_s(img_path, _T("img/player_left_%d.png"), i);
		loadimage(&atlas_player_stand_left[i], img_path);
		int width = atlas_player_stand_left[i].getwidth();
		int height = atlas_player_stand_left[i].getheight();
		Resize(&atlas_player_stand_right[i], width, height);
		DWORD* color_buffer_left = GetImageBuffer(&atlas_player_stand_left[i]);
		DWORD* color_buffer_right = GetImageBuffer(&atlas_player_stand_right[i]);
		for (int yy = 0; yy < height; yy++) {
			for (int xx = 0; xx < width; xx++) {
				int idx_left = yy * width + xx;
				int idx_right = yy * width + (width - xx - 1);
				color_buffer_right[idx_right] = color_buffer_left[idx_left];
			}
		}
	}
	for (int i = 0; i < NUMS; i++) {
		static TCHAR img_path[256];
		_stprintf_s(img_path, _T("img/player_run_left_%d.png"), i);
		loadimage(&atlas_player_run_left[i], img_path);
		int width = atlas_player_run_left[i].getwidth();
		int height = atlas_player_run_left[i].getheight();
		Resize(&atlas_player_run_right[i], width, height);
		DWORD* color_buffer_left = GetImageBuffer(&atlas_player_run_left[i]);
		DWORD* color_buffer_right = GetImageBuffer(&atlas_player_run_right[i]);
		for (int yy = 0; yy < height; yy++) {
			for (int xx = 0; xx < width; xx++) {
				int idx_left = yy * width + xx;
				int idx_right = yy * width + (width - xx - 1);
				color_buffer_right[idx_right] = color_buffer_left[idx_left];
			}
		}
	}
	for (int i = 0; i < NUMS; i++) {
		static TCHAR img_path[256];
		_stprintf_s(img_path, _T("img/enemy_left_%d.png"), i);
		loadimage(&atlas_enemy_left[i], img_path);
		int width = atlas_enemy_left[i].getwidth();
		int height = atlas_enemy_left[i].getheight();
		Resize(&atlas_enemy_right[i], width, height);
		DWORD* color_buffer_left = GetImageBuffer(&atlas_enemy_left[i]);
		DWORD* color_buffer_right = GetImageBuffer(&atlas_enemy_right[i]);
		for (int yy = 0; yy < height; yy++) {
			for (int xx = 0; xx < width; xx++) {
				int idx_left = yy * width + xx;
				int idx_right = yy * width + (width - xx - 1);
				color_buffer_right[idx_right] = color_buffer_left[idx_left];
			}
		}
	}

}
int main() {
	loadall();
	initgraph(1280, 720);
	BeginBatchDraw();
	while (running) {
		DWORD frame_strat_time = GetTickCount();
		if (scene_now == 0) {//���˵�����
			static bool is_begin_down = false;
			static bool is_exit_down = false;
			while (peekmessage(&msg)) {
				if (msg.message == WM_LBUTTONDOWN) {//������¾ͱ��
					if (msg.x > button_gamebegin_x-5 && msg.x < button_gamebegin_x + 205 && msg.y > button_gamebegin_y-5 && msg.y < button_gamebegin_y + 55) {
						is_begin_down = true;
					}
					if (msg.x > button_gameexit_x - 5 && msg.x < button_gameexit_x + 205 && msg.y > button_gameexit_y - 5 && msg.y < button_gameexit_y + 55) {
						is_exit_down = true;
					}
				}
				else if (msg.message == WM_LBUTTONUP) {//����ɿ������Ѿ��б�Ǿͽ�����һ�������������¿�ʼ��Ϸ��ť
					if (msg.x > button_gamebegin_x-5 && msg.x < button_gamebegin_x + 205 && msg.y > button_gamebegin_y-5 && msg.y < button_gamebegin_y + 55) {
						if (is_begin_down) {
							scene_now = 1;
							break;
						}
					}
					if (msg.x > button_gameexit_x - 5 && msg.x < button_gameexit_x + 205 && msg.y > button_gameexit_y - 5 && msg.y < button_gameexit_y + 55) {
						if (is_exit_down) {
							running = false;
							break;
						}
					}
				}
			}
			cleardevice();
			putimage_alpha(0, 0, &ui_background);//������
			putimage_alpha(button_gamebegin_x, button_gamebegin_y, &button_game_begin);//����ʼ��Ϸ��ťͼ��
			putimage_alpha(button_gameexit_x, button_gameexit_y, &button_game_exit);//���˳���Ϸ��ťͼ��
			FlushBatchDraw();
		}
		else if (scene_now == 1) {//��һĻ��ʵ�ֶԻ�����Ծ������
			static bool is_left = false;
			static bool is_right = false;
			DWORD frame_strat_time = GetTickCount();
			while (peekmessage(&msg)) {
				if (msg.message == WM_KEYUP) {
					if (msg.vkcode == VK_LEFT) {
						is_left=true;
					}
					else if (msg.vkcode == VK_RIGHT) {
						is_right = true;
					}
				}
				else if (msg.message == WM_KEYDOWN) {
					if (msg.vkcode == VK_LEFT) {
						is_left = false;
					}
					else if (msg.vkcode == VK_RIGHT) {
						is_right = false;
					}
				}
				
			}
			if (is_left) {
				player_x += player_speed;
			}
			if (is_right) {
				player_x -= player_speed;
				if (player_x < -50)player_x =-50;
			}
			cleardevice();
			putimage_alpha(player_x, player_y, &player);
			FlushBatchDraw();
		}
		else if (scene_now == 2) {//�Ի����õ�������Բ

		}
		else if (scene_now == 3) {

		}
		else if (scene_now == 4) {

		}
		else if (scene_now == 5) {

		}
		else if (scene_now == 6) {

		}
		else if (scene_now == 7) {

		}
		DWORD frame_end_time = GetTickCount();
		DWORD frame_delte_time = frame_strat_time - frame_end_time;
		if (frame_delte_time < 1000 / FPS) {
			Sleep(1000 / FPS - frame_delte_time);
		}
	}
	EndBatchDraw();

	return 0;
}