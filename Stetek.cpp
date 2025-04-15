#include "Statek.h"

Statek::Statek(float startX) {
    x = startX; // pozycja statku 
    bitmap = al_load_bitmap("statek.png");
    display = al_create_display(960, 640); // towrzenie wymiarow okna gry 
}


ALLEGRO_DISPLAY* Statek::getDisplay() {
    return display; //zwracanie wartoci display
}
// poruszanie sie w lewo
void Statek::poruszanieWLewo() {
    x -= 5;
    
}
// poruszanie siÍ w prawo
void Statek::poruszanieWPrawo() {
    x += 5;
   
}
// ogranczenia dotyczπce granicy 
void Statek::granica() {
    if (x > al_get_display_width(display) - al_get_bitmap_width(bitmap)) // zapobieganie poruszania sie statku jesli dotrze do prawej krawedzi ekranu
    {
        x = al_get_display_width(display) - al_get_bitmap_width(bitmap); // wartosc x zostaje ustawiona na wartosc szerokosci wyswietlania - szerokosc statku zapobiega 
                                                                         // dalszemu poruszaniu sie statku poza prawa kwaedü ekranu 
    }
    else if (x < 0) //sprawdzanie czy wspolrzedna statku x jest mniejsza niz zero
    {
        x = 0; // ustawienie wartosic x = 0 w celu ograniczenia dalczego poruszania sie statku w lewo
    }
}

float Statek::pobieraniePozycjiStatku() {
    return x; // pozycja statku 
}
