#include "stdafx.h"
#include "cLogic.h"

cLogic::cLogic()
{
	m_curLevel = 0;
	m_bIsDown = false;
	m_curMoveDic = Dir_NULL;
}

cLogic::~cLogic()
{

}

void cLogic::InitMap()
{
	for (int i = 0; i < LatticeNum_L; ++i)
	{
		for (int j = 0; j < LatticeNum_H; ++j)
		{
			if (i % 2 == 0 && j % 2 == 0)
			{
				m_Lattice[i][j] = Type_Stone;
			}
			else
			{
				m_Lattice[i][j] = Type_Num;
			}
		}
	}
}

bool IsHaveNum(int* posArray, int iValue)
{
	while (*posArray != '\0')
	{
		if (*posArray == iValue)
			return true;
		posArray++;
	}
	return false;
}

void cLogic::UpdateMap()
{
	int brikeNum = c_iBrikeNum[m_curLevel];
	m_doorPos.x = 0;
	m_doorPos.y = 0;
	//一共146个位置可以放砖头， 门一定要放在砖头里面

	//随机选择brikerNum个地方放砖头
	int brikePosArray[100] = { 0 };  //记录哪些格子需要放砖头
	int iRandValue = 1;
	for (int i = 0; i < brikeNum;)
	{
		srand(GetTickCount() + iRandValue);
		iRandValue = rand() % LatticeNullNum;
		if (IsHaveNum(brikePosArray, iRandValue))
		{
			continue;
		}
		brikePosArray[i] = iRandValue;
		++i;
	}
	//随机砖头里的某个位置放门
	srand(GetTickCount());
	int dooorPos = rand()%brikeNum;

	//已经确定哪个格子是什么， 把数据填到格子信息里m_lattice
	int numIndex = 0;
	int doorNumIndex = 0;
	for (int i = 0; i < LatticeNum_L; i++)
	{
		for (int j = 0; j < LatticeNum_H; j++)
		{
			if ( (i<=3&&j<=3) || m_Lattice[i][j]==Type_Stone)
			{
				continue;
			}
			//要放砖头的格子
			if (IsHaveNum(brikePosArray, numIndex))
			{
				m_Lattice[i][j] = Type_Brike;
				if (dooorPos == doorNumIndex)
				{
					m_Lattice[i][j] = Type_BrikeDoor;
				}
				doorNumIndex++;
			}
			numIndex++;
		}
	}

	//人物要放的位置
	m_Lattice[1][0] = Type_Man;
	m_fireManPos.x = 0;
	m_fireManPos.y = 1 * 50;
}

void cLogic::GameBegin()
{
	m_curLevel = 0;
	InitMap();
	UpdateMap();
}

void cLogic::ManMove(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
		MoveTo(Dir_Up, m_fireManPos);
		break;
	case VK_DOWN:
		MoveTo(Dir_Down, m_fireManPos);
		break;
	case VK_LEFT:
		MoveTo(Dir_Left, m_fireManPos);
		break;
	case VK_RIGHT:
		MoveTo(Dir_Right, m_fireManPos);
		break;
	default:
		break;
	}
}

void cLogic::MoveTo(MoveDir dir, stObjectPos& stPos)
{
	if (!CanMove(dir, stPos))
	{
		return;
	}
	m_curMoveDic = dir;
	//中点在哪个格子里面
	int j = (stPos.x + Lattice_Len / 2) / Lattice_Len;  //第j列
	int i = (stPos.y + Lattice_Len / 2) / Lattice_Len;  //第i行

	switch (dir)
	{
	case Dir_Up:
		stPos.y -= ManMoveStep;
		stPos.x = j*Lattice_Len;
		break;
	case Dir_Down:
		stPos.y += ManMoveStep;
		stPos.x = j*Lattice_Len;
		break;
	case Dir_Left:
		stPos.x -= ManMoveStep;
		stPos.y = i*Lattice_Len;
		break;
	case Dir_Right:
		stPos.x += ManMoveStep;
		stPos.y = i*Lattice_Len;
		break;
	default:
		break;
	}
}

bool cLogic::CanMove(MoveDir dir, stObjectPos& stPos)
{
	//判断将要移动到的点是在哪个格子里面
	//这个格子里面如果是砖头，石头，炸弹，边界就不能走

	//先判断对象的中点是在哪个格子里
	int xMid = stPos.x + Lattice_Len / 2;
	int yMid = stPos.y + Lattice_Len / 2;
	int j = xMid / Lattice_Len;
	int i = yMid / Lattice_Len;

	switch (dir)
	{
	case Dir_Up:
		if (stPos.y % Lattice_Len != 0)  //本来就在往上或往下的半路上
		{
			return true;
		}
		else
		{
			i -= 1;
		}
		break;
	case Dir_Down:
		if (stPos.y % Lattice_Len != 0)
		{
			return true;
		}
		else
		{
			i += 1;
		}
		break;
	case Dir_Left:
		if (stPos.x % Lattice_Len != 0)
		{
			return true;
		}
		else
		{
			j -= 1;
		}
		break;
	case Dir_Right:
		if (stPos.x % Lattice_Len != 0)
		{
			return true;
		}
		else
		{
			j += 1;
		}
		break;
	default:
		break;
	}

	//将要走到的位置是第i行第j列
	if (i<0 || j<0 || i>LatticeNum_L - 1 || j>LatticeNum_H - 1)
	{
		return false; //不可走出边界
	}

	if (m_Lattice[i][j] == Type_Stone || m_Lattice[i][j] == Type_Brike
		|| m_Lattice[i][j] == Type_BrikeDoor || m_Lattice[i][j] == Type_Bomb)
	{
		return false;
	}

	return true;
}

void cLogic::ManStop()
{
	m_bIsDown = false;
}
