#include "Ufolud.h"
#include "Statek.h"
#include "KodGry.h"


Ufolud::Ufolud() {
}

void Ufolud::tworzenieUfoludka(ALLEGRO_BITMAP* bitmap3, int  pozycjaUfoludaX[55], float  predkoscUfoluda[55], float  pozycjaUfoludaY[55], int czestotliwoscStrzalow, Pocisk  pociski[1000], ALLEGRO_DISPLAY* display, bool& dotkniecieKrawedzi, bool& zmianaKierunkuPoruszania)
{
    for (int i = 0; i <= 54; i++) // tworzenie ufoludow
    {
        al_draw_bitmap(bitmap3, pozycjaUfoludaX[i] += predkoscUfoluda[i], pozycjaUfoludaY[i], 0); //wyswietlenie bitmapy ufoluda

        if (pozycjaUfoludaY[i] > 0) // jesli ufoludek jest na wartosci wiekszej od 0 moze sie poruszac 
        {
            // losowe strzelanie ufoludów do gracza
            MozliwoscStrzelaniaUfoluda(czestotliwoscStrzalow, pozycjaUfoludaY, i, pozycjaUfoludaX, bitmap3, pociski);

            // mozliwosci ruchu dla ufoludow
            if (pozycjaUfoludaX[i] + al_get_bitmap_width(bitmap3) >= al_get_display_width(display) && predkoscUfoluda[i] > 0)
            { 
                dotkniecieKrawedzi = true; //zmiana wartosci na true jesli zostala dotknieta prawa krawedz ekranu
            }
            else if (pozycjaUfoludaX[i] <= 0 && predkoscUfoluda[i] < 0) 
            { 
                dotkniecieKrawedzi = true; //zmiana wartosci na true jesli zostala dotknieta lewa krawedz ekranu 
            }

            if (dotkniecieKrawedzi) 
            { // zmiana kierunku ruchu dla wszystkich ufoludow
                zmianaKierunkuPoruszania = true;
                break; // przerwanie petli bo juz wiadomo ze przynajmniej jeden z ufoludow dotknal krawedzi
            }
        }

    }

    if (zmianaKierunkuPoruszania) { // zmiana kierunku ruchu dla wszystkich ufoludow
        for (int i = 0; i <= 54; i++) {
            predkoscUfoluda[i] = -predkoscUfoluda[i]; // zmiana kierunku ruchu
            pozycjaUfoludaY[i] += 10; // przesuniêcie ufoludow w dol
        }
    }
}

void Ufolud::MozliwoscStrzelaniaUfoluda(int czestotliwoscStrzalow, float  pozycjaUfoludaY[55], int i, int  pozycjaUfoludaX[55], ALLEGRO_BITMAP* bitmap3, Pocisk  pociski[1000])
{
    if (rand() % czestotliwoscStrzalow == 0 && pozycjaUfoludaY[i] > 0) {
        // Szansa na strzal i sprawdzenie czy pozycja ufoluda jest powyzej ekranu
        bool canShoot = true;
        for (int j = 0; j <= 54; j++) {
            if (i != j && pozycjaUfoludaY[j] > pozycjaUfoludaY[i] && abs(pozycjaUfoludaX[j] - pozycjaUfoludaX[i]) < al_get_bitmap_width(bitmap3)) {
                // Sprawdzenie czy istnieje inny ufolud przed aktualnym ufoludem w tej samej kolumnie
                // ktory uniemozliwia strzelanie aktualnemu ufoludowi
                canShoot = false;
                break;
            }
        }
        if (canShoot) 
        {
            // Jesli ufolud moze strzelac pociskiem bezpiecznie
            Pocisk* pocisk = NULL;
            for (int j = 0; j < 1000; j++) 
            {
                if (!pociski[j].aktywny) 
                {
                    // Znalezienie nieaktywnego pocisku w puli pociskow
                    pocisk = &pociski[j];
                    break;
                }
            }
            if (pocisk) 
            {
                // Jesli znaleziono nieaktywny pocisk
                pocisk->x = pozycjaUfoludaX[i] + 20; // Pozycja pocisku na osi X (20 to polowa szerokosci ufoluda)
                pocisk->y = pozycjaUfoludaY[i] + 50; // Pozycja pocisku na osi Y (50 to polowa wysokosci ufoluda)
                pocisk->aktywny = true; // Ustawienie aktywnosci pocisku
                pocisk->PociskGracza = false; // Pocisk nie nalezy do gracza
            }
        }
    }
}

void Ufolud::resetowaniePozycjiUfoludkow(int  pozycjaUfoludaX[55], float  pozycjaUfoludaY[55], float  predkoscUfoluda[55], Pocisk  pociski[1000])
{
    for (int i = 0; i < 55; i++)
    {
        pozycjaUfoludaX[i] = 50 + i % 11 * 50; // ustawienie formacji ufoludow na osi X
        pozycjaUfoludaY[i] = 70.0 + i / 11 * 40.0; // ustawienie formacji ufoludow na osi y
        predkoscUfoluda[i] = 1.0; // ustawienie predkosci poruszania ufoludow 
        pociski[i].aktywny = false; // ustawienie aktywnosci pociskow na false w celu usuniecia ich z ekranu 
    }
}


