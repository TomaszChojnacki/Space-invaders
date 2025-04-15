#ifndef UFOLUD_H
#define UFOLUD_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <stdbool.h>
#include <math.h>
#include <iostream>
#include "Statek.h"

// struktura przechowujaca informacje o pocisku
typedef struct Pocisk {
    float x; //pozycja x pocisku
    float y; // pozycja y pocisku
    bool aktywny; // informacja czy pocisk jest aktywny (czy powinien byc wyswietlany i poruszany)
    bool PociskGracza; // informacja czy pocisk nalezy do gracza

} Pocisk;

class Ufolud {    
public:
    Ufolud();
    void tworzenieUfoludka(ALLEGRO_BITMAP* bitmap3, int  pozycjaUfoludaX[55], float  predkoscUfoluda[55], float  pozycjaUfoludaY[55], int czestotliwoscStrzalow, Pocisk  pociski[1000], ALLEGRO_DISPLAY* display, bool& dotkniecieKrawedzi, bool& zmianaKierunkuPoruszania);
    void MozliwoscStrzelaniaUfoluda(int czestotliwoscStrzalow, float  pozycjaUfoludaY[55], int i, int  pozycjaUfoludaX[55], ALLEGRO_BITMAP* bitmap3, Pocisk  pociski[1000]);
    void resetowaniePozycjiUfoludkow(int  pozycjaUfoludaX[55], float  pozycjaUfoludaY[55], float  predkoscUfoluda[55], Pocisk  pociski[1000]);
};

#endif // UFOLUD_H
