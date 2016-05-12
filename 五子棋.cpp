#include <stdio.h>
#include<iostream>
#include<stdlib.h>
#include <iomanip>
using namespace std;
#define N 16

int chessboard[N + 1][N + 1] = { 0 };
int whoseTurn = 0;
int judgewinner(int x, int y);
int judgechess(int x, int y);
void playChess(void);
void printChessboard(void);
void initGame(void);



int main()
{
	initGame();

	while (1)
	{
		whoseTurn++;

		playChess();

	}
	return 0;
}
int judgewinner(int x, int y){
	int i, j;
	int t = 2 - whoseTurn % 2;

	for ( i = x-4,j=y; i <= x; i++)
	{
		if (i >= 1 && i <= N - 4 && t == chessboard[i][j] && t == chessboard[i + 1][j] && t == chessboard[i + 2][j] && t == chessboard[i + 3][j] && t == chessboard[i + 4][j])
			return 1;
	}

	for (i = x, j = y - 4; j <= y; j++)
	{
		if (j >= 1 && j <= N - 4 && t == chessboard[i][j] && t == chessboard[i][j + 1] && t == chessboard[i][j + 1] && t == chessboard[i][j + 3] && t == chessboard[i][j + 4])
			return 1;
	}
	for (i = x - 4, j = y - 4; i <= x, j <= y; i++, j++)
	{
		if (i >= 1 && i <= N - 4 && j >= 1 && j <= N - 4 && t == chessboard[i][j] && t == chessboard[i + 1][j + 1] && t == chessboard[i + 2][j + 2] && t == chessboard[i + 3][j + 3] && t == chessboard[i + 4][j + 4])
			return 1;
	}

	for (i = x + 4, j = y - 4; i >= 1, j <= y; i--, j++)
	{
		if (i >= 1 && i <= N - 4 && j >= 1 && j <= N - 4 && t == chessboard[i][j] && t == chessboard[i - 1][j + 1] && t == chessboard[i - 2][j + 2] && t == chessboard[i - 3][j + 3] && t == chessboard[i - 4][j + 4])
			return 1;
	}

	return 0;
}
int judgechess(int x, int y){	
	int i, j, k, l=0;
	k = N*N;
	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
			if (chessboard[i][j] !=0){
				l=l+1;
				if (l==k)
					return 3;
			}
	}
	if (chessboard[x][y]!=0)
		return 1;
	if (x > N || y > N||x < 1||y < 1)
		return 2;

	return 0;
}
void playChess(void){
	int i, j, winner;

	if (1 == whoseTurn % 2)
	{
		cout << "轮到玩家1，请输入棋子的位置，格式为行号+空格+列号：";
		cin >> i >> j;
		while (judgechess(i, j) == 1)
		{
			cout << "重复落子，请重新选择落子"<<endl;
			cout << "轮到玩家1，请输入棋子的位置，格式为行号+空格+列号：";
			cin >> i >> j;
			
		}
		while (judgechess(i, j) == 2)
		{
			cout << "落子越界，请重新落子" << endl;
			cout << "轮到玩家1，请输入棋子的位置，格式为行号+空格+列号：";
			cin >> i >> j;

		}
		chessboard[i][j] = 1;
	}
	else
	{
		cout << "轮到玩家2，请输入棋子的位置，格式为行号+空格+列号：";
		cin >> i >> j;
		while (judgechess(i, j) == 1)
		{
			cout << "重复落子，请重新选择落子" << endl;
			cout << "轮到玩家2，请输入棋子的位置，格式为行号+空格+列号：";
			cin >> i >> j;		
		}
		while (judgechess(i, j) == 2)
		{
			cout << "落子越界，请重新落子" << endl;
			cout << "轮到玩家2，请输入棋子的位置，格式为行号+空格+列号：";
			cin >> i >> j;
		}
		chessboard[i][j] = 2;
	}

	system("cls");
	printChessboard();
	if (judgechess(i,j)==3)
	{
		cout << "棋盘已下满，未分出胜负" << endl;
		exit(0);
	}

	if (judgewinner(i, j))
	{
		if (1 == whoseTurn % 2){
			cout << "玩家1胜" << endl;
			exit(0);
		}

		else{
			cout << "玩家2胜" << endl;
			exit(0);
		}
	}
}
void printChessboard(void){
	int i, j;

	for (i = 0; i <= N; i++)
	{
		for (j = 0; j <= N; j++)
		{
			if (0 == i)
				cout << setw(3) << j;
			else if (j == 0)
				cout << setw(3) << i;
			else if (1 == chessboard[i][j])
				cout << " ●";
			else if (2 == chessboard[i][j])
				cout << " ○";
			else
				cout << "  *";
		}
		cout << endl;
	}
}
void initGame(void){
	char c;
	cout << "欢迎请输入y进入游戏：";
	c = getchar();
	if ('y' != c&&'Y' != c)
	{
		exit(0);
	}
	system("cls");
	printChessboard();
}