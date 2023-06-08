#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "button.h"


class MainMenu {
    public:
    MainMenu();
    ~MainMenu();
    int Start();
    
    private:
    Button boton_play;
    Button boton_quit;
    int handleEvents();
};

#endif
