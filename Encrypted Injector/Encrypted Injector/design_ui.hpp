#pragma once
#include <iostream>
#include <Windows.h>
namespace Encrypted
{
	int iIndexSubmenu = 0;
	struct coords
	{
		int x, y; // default line
	};
	coords current_coords;
	class Design_ui
	{
	public:

		void set_coord_text(int x, int y)
		{
			COORD coords;
			coords.X = x, coords.Y = y;
			current_coords.y += 2; // salto linea;
			//current_coords.x = x, current_coords.y += y;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
		}
		void do_title(const char* title)
		{
			set_coord_text(40, 2); std::cout << title << std::endl;
		}
		COORD get_console_largest_win_size()
		{		
			return GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

		}
	};
	 Design_ui* d; // me da un error raro de obj :cc y tuve que poner un extern 
}