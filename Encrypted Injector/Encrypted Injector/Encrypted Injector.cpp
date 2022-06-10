#include <iostream>
#include "design_ui.hpp"
#include "nt_create_thread_ex.hpp"
#include "settings.hpp"

int main()
{
    do {
      /* std::cout << R"(
 ______                             _           _   _____       _           _             
|  ____|                           | |         | | |_   _|     (_)         | |            
| |__   _ __   ___ _ __ _   _ _ __ | |_ ___  __| |   | |  _ __  _  ___  ___| |_ ___  _ __ 
|  __| | '_ \ / __| '__| | | | '_ \| __/ _ \/ _` |   | | | '_ \| |/ _ \/ __| __/ _ \| '__|
| |____| | | | (__| |  | |_| | |_) | ||  __/ (_| |  _| |_| | | | |  __/ (__| || (_) | |   
|______|_| |_|\___|_|   \__, | .__/ \__\___|\__,_| |_____|_| |_| |\___|\___|\__\___/|_|   
                         __/ | |                              _/ |                        
                        |___/|_|                             |__/                         
)" << std::endl;*/
        Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "1. Empezar Injecion" << std::endl;
        Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "2. Configuracion" << std::endl;
        Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "3. Salir" << std::endl;
        Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "Seleciona una opcion > " ; std::cin >> Encrypted::iIndexSubmenu;
        switch (Encrypted::iIndexSubmenu)
        {
        case 1: system("cls"); Encrypted::current_coords.y = 0; Encrypted::g_thread.thread_sub(); break;
        case 2:system("cls"); Encrypted::current_coords.y = 0; Encrypted::g_settings.settings_sub();  break;
        case 3: /* uso debugbreak? */ break;
        default: 
      //      system("Color C");
         //   system("cls");
            Encrypted::d->set_coord_text(38, Encrypted::current_coords.y = 6); std::cout << "Error opcion no valida" << std::endl;
            Sleep(200); 
            Encrypted::current_coords.y = 0;
            system("cls");
         //   system("Color 07");

        }

    } while (Encrypted::iIndexSubmenu != 3);

}


