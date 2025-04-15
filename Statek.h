#ifndef STATEK_H
#define STATEK_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>



class Statek {
private:
    float x; 
    ALLEGRO_BITMAP* bitmap; 
    ALLEGRO_DISPLAY* display;

public:
    Statek(float startX);
    void poruszanieWLewo();
    void poruszanieWPrawo();
    void granica();
    float pobieraniePozycjiStatku();
    ALLEGRO_DISPLAY* getDisplay();
};

#endif // STATEK_H
