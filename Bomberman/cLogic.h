#pragma once
#include <list>

using namespace std;

#define LatticeNum_L   13    //格子行数
#define LatticeNum_H   17    //格子列数
#define Level_Num      6     //关卡数量
#define LatticeNullNum 146   //空位置
#define Lattice_Len    50    //格子的边长
#define ManMoveStep    10    //每一步走多远


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
	Type_Stone,    //石头
	Type_Brike,    //砖头
	Type_Bomb,     //炸弹
	Type_Fire,     //火光
	Type_Door,     //门
	Type_BrikeDoor,//石头门
	Type_Man,      //人
	Type_Num
};

//每关砖头个数数组
const int c_iBrikeNum[Level_Num]=
{
	10,20,30,40,50,60
};

//保存位置信息的结构体
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

	void GameBegin();//游戏开始
	void InitMap();  // 初始化地图信息
	void UpdateMap(); //更新地图

	void ManMove(WPARAM wParam);                    //人物移动
	void MoveTo(MoveDir dir, stObjectPos& stPos);
	bool CanMove(MoveDir dir, stObjectPos& stPos);
	void ManStop();                                 //停止走动

	MoveDir m_curMoveDic;   //当前移动方向
	bool m_bIsDown;  //是否已经按下方向键
	stObjectPos m_fireManPos;
	stObjectPos m_doorPos;
	int m_curLevel;   //当前关卡
	LatticeType m_Lattice[LatticeNum_L][LatticeNum_H];   //格子信息



};
