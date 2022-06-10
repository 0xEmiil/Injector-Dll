#pragma once
#include <iostream>
#include "design_ui.hpp"
namespace Encrypted
{
	class settings
	{
	public:
		void settings_sub()
		{ 
			do {
				// obtener atributos de consola | ref: https://stackoverflow.com/questions/33446296/how-to-use-setconsoletextattribute-c
				HANDLE hcons = GetStdHandle(STD_OUTPUT_HANDLE);
				srand(time(NULL));
				int rand_col = rand() % 4 + 1; // gen rand
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "1. Letras Rojas" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "2. Letras Verdes" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "3. Letras Azules" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "4. Letras Cyan Oscuro" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "5. Letras Magneta Oscura" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "6. Letras Gris Oscuro" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "7. Letras Grises" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "8. Letras Amarillas" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "9. Letras Blancas" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "10. Generar Random" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "11. <- Atras" << std::endl;

				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "Seleciona una opcion > "; std::cin >> iIndexSettings;
				switch (iIndexSettings)
				{
				case 1: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_RED); break;
				case 2: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_GREEN); break;
				case 3: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_BLUE); break;
				case 4: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_GREEN | FOREGROUND_BLUE); break;
				case 5: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_RED | FOREGROUND_BLUE); break;
				case 6: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); break;
				case 7: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_INTENSITY); break;
				case 8: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN); break;
				case 9: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); break;
				case 10: Encrypted::current_coords.y = 0; system("cls"); SetConsoleTextAttribute(hcons, FOREGROUND_INTENSITY | rand_col | rand_col | rand_col); break;

				case 11: Encrypted::current_coords.y = 0; system("cls"); break;
				default:
					//system("Color C");
					Encrypted::d->set_coord_text(38, Encrypted::current_coords.y = 12); std::cout << "Error opcion no valida" << std::endl;
					Sleep(200);
					Encrypted::current_coords.y = 0;
					system("cls");
					//system("Color 07");
				}
			} while (iIndexSettings != 11);
		}
		~settings()
		{

	    }
	private:
		int iIndexSettings;
	};
	settings g_settings;
}