#pragma once
#include <iostream>
#include <vector>

#define WINDOW_WID			1200
#define WINDOW_HEI			800
#define GROUND_POS_Y		730

class PLAYER {
public:
	int id;						// �ٸ� Ŭ���̾�Ʈ �ĺ� ���� - -1 // ���� �Ⱦ��� ��Ȳ�� -1
	short role;					// � ĳ���� ���� - f w e => fire water earth
	int x, y;					// ���ϴ� ��ǥ
	int hei = 100, wid = 60;	// ĳ���� ũ��

	int direction;				// �̵����� (�ִϸ��̼ǿ��� ���)
	float wid_v{};
	float wid_a{};

	int ground = 730;		// Stage�� �ٴ� ���� (����� y = 0, ���������� +)
	float g = 3.f;			// �߷� ������ ���� ���� ����
	float v = 0;			// �ӵ�
	short Frame = 0;		// �ִϸ��̼� ������

	int C_img_Frame{};
	int C_img_x{}, C_img_y{};
	int C_img_X_Size_01{}, C_img_Y_Size_01{};
	int C_img_X_Size_02{}, C_img_Y_Size_02{};

	bool on = true;
	bool is_Jumping = true;
	bool is_Push = true;
	bool Down = true;

	PLAYER() : id(-1), role('f') {}
	~PLAYER() {

	}
};

class OBJECT {
	bool On = false;			// ��� ����

public:
	int x{}, y{};				// ������ ���� �»�� (x, y) ��ġ��ǥ
	int image_x{}, image_y{};	// ����� �̹��� ������ ������ (��������Ʈ �̹���)
	int wid{}, hei{};			// ���α���, ���α���
	int MaxWid{};				// ��ü ��������Ʈ ���α���
	int imageMoveWid{};			// ��������Ʈ �̹��� �̵�

	OBJECT() {};
	OBJECT(int pos_x, int pos_y, int WID, int HEI, int MAXWID, int IMAGEMOVEWID, bool ON)
		: x(pos_x), y(pos_y), wid(WID), hei(HEI), MaxWid(MAXWID), imageMoveWid(IMAGEMOVEWID), On(ON) {}

	// �浹 Ȯ�� �Լ�, Player(fire boy or water girl) �Է��� �޾� this Object�� �浹 ��
	bool Collision(PLAYER& pl) {
		if (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) <= (wid + pl.wid) / 2)
			&&
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) <= (hei + pl.hei) / 2)
			) {
			return true;
		}
		else return false;
	}

	bool Ft_Collision(PLAYER& pl) {
		if (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) < (wid + pl.wid) / 2)
			&&
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) < (hei + pl.hei) / 2)
			) {
			return true;
		}
		else return false;
	}

	void SetVisible(bool in) {
		On = in;
	}

	bool GetVisible() {
		return On;
	}

	bool ChangeFrame(int direction, bool replay) {
		if (MaxWid == image_x + imageMoveWid) {
			if (replay)
				image_x = 0;
			return true;
		}
		else {
			image_x += (imageMoveWid * direction);
			return false;
		}
	}
};

class Stage
{
public:
	// RECTANGLE Rt;

	short stage;						// ���� ��������
	bool clear;							// Stage Ŭ���� ����
	int average;
	bool time_over;						// ���� Stage���� Timer Over ����
	int count;							// �ִϸ��̼� ������ ��ȣ
	int red_total;						// ���� ���� �� ����
	int blue_total;						// �Ķ� ���� �� ����
	bool stair;							// Fire boy�� Water girl �� �� �� ����� �ö�

	OBJECT Ground{ 0, GROUND_POS_Y, WINDOW_WID, WINDOW_HEI - GROUND_POS_Y, 0, 0, true };
	std::vector<OBJECT> Red_Jewel;
	std::vector<OBJECT> Blue_Jewel;
	std::vector<OBJECT> Trap;
	std::vector<OBJECT> Ft;			// ���� ������Ʈ

	// OBJECT Trap[90];		
	OBJECT Die;				// ����� ������ ���� (Max x = 7950, move x = 159)
	OBJECT blue_door;		// (Max x = 1260, move x = 60) // Max = 1296, Move = 54
	OBJECT red_door;		// (Max x = 1260, move x = 60) // Max = 1250, Move = 50
	OBJECT button[5];		// ��� ��ư(block1.PNG ����)	(Down BTN Max y = 7, move y = 1) else (Up BTN Max y = 15, move y = 1)
	OBJECT block[5];		// (Max x = 40, move x = 2)

public:
	void title() {}
	void lobby() {}
	void Stage_1();
	void Stage_2();
	void Stage_3();
};

extern Stage currentStage;