#pragma once
#include <list>

using namespace std;

#define LatticeNum_L   13    //��������
#define LatticeNum_H   17    //��������
#define Level_Num      6     //�ؿ�����
#define LatticeNullNum 146   //��λ��
#define Lattice_Len    50    //���ӵı߳�
#define ManMoveStep    10    //ÿһ���߶�Զ


enum MoveDir
{
	Dir_Up,
	Dir_Down,
	Dir_Left,
	Dir_Right,
	Dir_NULL
};

enum  LatticeType
{
	Type_Stone,    //ʯͷ
	Type_Brike,    //שͷ
	Type_Bomb,     //ը��
	Type_Fire,     //���
	Type_Door,     //��
	Type_BrikeDoor,//ʯͷ��
	Type_Man,      //��
	Type_Num
};

//ÿ��שͷ��������
const int c_iBrikeNum[Level_Num]=
{
	10,20,30,40,50,60
};

//����λ����Ϣ�Ľṹ��
struct stObjectPos
{
	int x;
	int y;
	stObjectPos()
	{
		x = 0; y = 0;
	}
};

class cLogic
{
public:
	cLogic();
	~cLogic();

	void GameBegin();//��Ϸ��ʼ
	void InitMap();  // ��ʼ����ͼ��Ϣ
	void UpdateMap(); //���µ�ͼ

	void ManMove(WPARAM wParam);                    //�����ƶ�
	void MoveTo(MoveDir dir, stObjectPos& stPos);
	bool CanMove(MoveDir dir, stObjectPos& stPos);
	void ManStop();                                 //ֹͣ�߶�

	MoveDir m_curMoveDic;   //��ǰ�ƶ�����
	bool m_bIsDown;  //�Ƿ��Ѿ����·����
	stObjectPos m_fireManPos;
	stObjectPos m_doorPos;
	int m_curLevel;   //��ǰ�ؿ�
	LatticeType m_Lattice[LatticeNum_L][LatticeNum_H];   //������Ϣ



};
