#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
using namespace std;

class Notepad
{
	struct Node
	{
		char userChar;
		Node* next;
		Node* prev;
		Node()
		{
			userChar = ' ';
			next = prev = NULL;
		}
		Node(char var)
		{
			next = prev = NULL;
			userChar = var;
		}
	} *start,*current, *currentDel, *end, *rows[10], *printer;

public:
	ifstream filein;
	ofstream fileout;
	//variables
	COORD pos;
	HANDLE menu;
	char var;
	int x, y, rowNum;

	//functions
	Notepad();

	void printList(),
		Display(),
		writeChar(),
		Tab(),
		Insert(),
		Backspace(),
		saveFile(),
		loadFile(),
		gotoxy(short x, short y);
	bool Write();


};





