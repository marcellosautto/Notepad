#include "Notepad.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
//#include "stdafx.h"
using namespace std;

bool main()
{
	int run = 2;
	while (1)
	{
		Notepad* np = new Notepad();
		
		if (run == 2)
		{
			np->Display();
			run = np->Write();
		}
		else if (run == 1)
		{
			np->loadFile();
			run = np->Write();
		}
		else if (run == 0)
			return false;
	}


}
//CONSTRUCTOR
Notepad::Notepad()
{
	menu = GetStdHandle(STD_OUTPUT_HANDLE);
	filein.open("C:\\temp\\output.text");
	var = ' ';
	x = 0, y = 13;
	rowNum = 0;
	start = current = currentDel = end = printer = nullptr;
	pos.X = 0;
	pos.Y = 0;

	for (int i = 0; i < 10; i++)	//cleans array
		rows[i] = nullptr;
}

//MOVES CURSOR
void Notepad::gotoxy(short xi, short yi)
{
	pos = { xi, yi };
	SetConsoleCursorPosition(menu, pos);
}

//WRITE FUNCTION
bool Notepad::Write()
{

	while (var != 27)
	{
		var = _getch();

		if ((var == 0))
		{
			var = _getch();
			if ((var == 60)) //F1: save linked list to a text file
			{
				saveFile();
			}
			else if ((var == 59)) //F2: write linked list from text file
			{
				return true;
			}

		}

		if ((var == 8) && (end->prev != nullptr)) //backspace unless there is nothing before it
		{
			Backspace();
		}

		else if (var == -32)
		{
			var = _getch();
			Insert();
		}
		else if (var == 13) //Enter
		{
			Tab();
		}
		else if (var != 60) //regular char
		{
			writeChar();
		}

		system("cls");
		Display();
		printList();

		gotoxy(x, y);
	}
	return false;

}

void Notepad::Display()
{
	SetConsoleTextAttribute(menu, 14);
	cout << "          | *************Welcome to My Notepad Editor Program************* |" << endl;
	cout << "          |                                                                |" << endl;
	cout << "          | Press 'F1' To Load Your Last Save (Press 'UP' to Edit Save)    |" << endl;
	cout << "          | Press 'F2' To Save Your Writing to a Notepad File              |" << endl;
	cout << "          | Press 'BACKSPACE' To Delete Written Characters                 |" << endl;
	cout << "          | Press 'LEFT KEY' to Move the Cursor Left                       |" << endl;
	cout << "          | Press 'RIGHT KEY' to Move the Cursor Right                     |" << endl;
	cout << "          | Press 'ENTER' to Start Writing On a New Line (Up to 10 Lines)  |" << endl;
	cout << "          | Press 'UP KEY' to Move the Cursor Up                           |" << endl;
	cout << "          | Press 'DOWN KEY' to Move the Cursor Down                       |" << endl;
	cout << "          |                                                                |" << endl;
	cout << "          | ************************************************************** |" << endl << endl;
	SetConsoleTextAttribute(menu, 15);
}
//PRINTS LINKED LIST
void Notepad::printList()
{
	for (int i = 0; i < 10; i++)
	{
		if (rows[i] != nullptr)
		{
			printer = rows[i];
			while (printer != nullptr)
			{
				if (printer->userChar != 10)
					cout << printer->userChar;
				printer = printer->next;
			}
			cout << endl;

		}

	}
}
//LOADS PREVIOUS SAVE
void Notepad::loadFile()
{
	y = 12;

	//Puts chars into linked list until it reaches the end of the file
	while (!filein.eof())
	{
		filein.get(var);
		Node* temp = new Node(var);

		if (var == 10)	//if nextLine, then start writing on the next row of the array
		{
			rowNum++;
			start = end = rows[rowNum];
			y++;
			x = 0;
		}

		else if (start == nullptr)
		{
			start = temp;
			end = temp;
			rows[rowNum] = start;
			start->prev = rows[rowNum];
		}

		else if (var != 10)
		{
			end->next = temp;
			temp->prev = end;
			end = temp;
			current = end;
			x++;
		}
		
	}
	rowNum--;
	start = end = current = rows[rowNum];
	system("cls");
	Display();
	printList();
	filein.close();

}//SAVES LINKED LIST TO TEXT FILE
void Notepad::saveFile()
{
	fileout.open("C:\\temp\\output.text");
	for (int i = 0; i < 10; i++)
	{
		if (rows[i] != nullptr)
		{
			printer = rows[i];
			while (printer != nullptr)
			{
				fileout << printer->userChar;
				printer = printer->next;
			}
			fileout << endl;

		}

	}

	fileout.close();
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "Your File Has Been Saved.";
	Sleep(750);
}
//DELETES PREV CHAR
void Notepad::Backspace()
{
	if (end == current)
	{
		end = end->prev;
		delete (end->next);
		end->next = nullptr;
		current = end;
		x--;
	}
	else if ((current->next != nullptr && current->prev != nullptr)) //backspace between nodes
	{
		currentDel = current;                    //simulates current position of current
		current->next->prev = current->prev;    //attaches next node to the previous node
		current->prev->next = current->next;    //attaches previous node to the next node
		current = current->prev;                //assigns current to the previous node
		delete (currentDel);                    //deletes the simulator of the current node
		x--;                                    //move cursor

	}
}

//ALLOWS USER TO INSERT CHARACTERS ON THE LINE ABOVE, BELOW, OR TO THE LEFT OR RIGHT
void Notepad::Insert()
{
	if ((var == 75) && (current->prev != nullptr))//left arrow
	{
		current = current->prev;
		x--;
	}

	if ((var == 77) && (current->next != nullptr))//right arrow
	{
		current = current->next;
		x++;
	}


	if ((var == 72) && rowNum > 0)//up arrow
	{
		y--;
		rowNum--;
		x = 0;
		start = rows[rowNum];
		end = start;
		while (end->next != nullptr)	//When 'up' is pressed, current goes to the end of the line above
		{
			end = end->next;
			x++;
		}
		current = end;
		x++;

	}

	if ((var == 80) && (rows[rowNum + 1] != nullptr)) //down arrow
	{
		y++;
		rowNum++;
		x = 0;
		start = rows[rowNum];
		end = start;
		while (end->next != nullptr)    //When 'down' is pressed, current goes to the end of the line below
		{
			end = end->next;
			x++;
		}
		current = end;
		x++;

	}
}
//CREATES NEXT LINE
void Notepad::Tab()
{
	rowNum++;
	start = end = current = rows[rowNum];	//the start, end and current is moved to the beginning of the next line
	y++;
	x = 0;
}
//CREATES LINKED LIST WHEN TYPING NORMALLY/ WHEN INSERTING
void Notepad::writeChar()
{
	Node* temp = new Node(var);
	if (start == nullptr)
	{
		start = temp;
		end = temp;
		current = end;
		x++;
		rows[rowNum] = start;
		start->prev = rows[rowNum];
	}
	else
	{
		if (current == end) //if cursor is at the end, write normally
		{
			end->next = temp;
			temp->prev = end;
			end = temp;
			current = end;
			x++;
		}
		else if (current->next != nullptr && current->prev != nullptr)	//if current is in between two nodes, insert node
		{
			temp->next = current->next;
			current->next->prev = temp;
			temp->prev = current;
			current->next = temp;
			current = temp;
			x++;
		}
		
	}
}
