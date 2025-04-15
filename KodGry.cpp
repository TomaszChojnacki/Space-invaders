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
#include "Ufolud.h"
#include "KodGry.h"
#define PREDKOSC_POCISKU 2 // definowanie z jaka predkoscia ma poruszac sie pocisk


void niszczenieObiektow(ALLEGRO_BITMAP* bullet_bitmap, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* button, Statek& statek, ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer, ALLEGRO_BITMAP* bitmap, ALLEGRO_BITMAP* bitmap3, ALLEGRO_BITMAP* bitmap2, ALLEGRO_BITMAP* tlo, ALLEGRO_BITMAP* boom, ALLEGRO_SAMPLE* sample, ALLEGRO_SAMPLE* strzal, ALLEGRO_SAMPLE* boomA, ALLEGRO_SAMPLE* obrazenia);
void kolizjePociskow(int i, Pocisk  pociski[1000]);
void pociski_gracza_ufoluda(Pocisk  pociski[1000], int i);
void sprawdzanie_kolizji(float  pozycjaUfoludaY[55], Pocisk  pociski[1000], int i, int  pozycjaUfoludaX[55], ALLEGRO_BITMAP* boom, int& punktygracza, bool& eksplozjaskonczona, float startTime, float czasWybuchu, ALLEGRO_SAMPLE* boomA);
void gameOverAction(Ufolud ufolud, bool& gameOver, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* informacjekoniec, int width, int& punktygracza, bool& mainWhile, int& playerLives, int  points[55], float  d[55], float  M[55], Pocisk  pociski[1000], Statek& statek, bool& graj, bool& menu);
void wygranieGry(bool& wszystkieZestrzelone, float  d[55], ALLEGRO_EVENT_QUEUE* queue, bool& mainWhile, int width, ALLEGRO_FONT* informacjekoniec, int& playerLives, int  points[55], float  M[55], Pocisk  pociski[1000], int& czestotliwoscStrzalow, Statek& statek, bool& graj, Ufolud& ufolud, int& punktygracza, bool& menu);

int main()
{
    initAllegro();
    float startX = 420; // punkt startowy pozycji statku
    Statek statek(startX); // dodanie obiektu statku
    Ufolud ufolud; // dodanie obiektu ufoluda
    ALLEGRO_BITMAP* bitmap2 = al_load_bitmap("tlo1.png"); //ladowanie bitmapy tla gry
    ALLEGRO_KEYBOARD_STATE keyState; // zmienna okreslajaca status klawiatury
    ALLEGRO_FONT* font = al_load_ttf_font("Amatic-Bold.ttf", 50, 0); // dodanie stylu czcionki
    ALLEGRO_FONT* font1 = al_load_ttf_font("Amatic-Bold.ttf", 38, 0); // dodanie stylu czcionki o innej wielkosci
    ALLEGRO_FONT* font3d = al_load_ttf_font("3Dtext.ttf", 100, 0); // dodanie stylu czcionki
    ALLEGRO_FONT* font3dmenu = al_load_ttf_font("3Dtext.ttf", 70, 0); // dodanie stylu czcionki
    ALLEGRO_FONT* informacje = al_load_ttf_font("czcionkainfo.ttf", 24, 0); // dodanie stylu czcionki
    ALLEGRO_FONT* informacjekoniec = al_load_ttf_font("czcionkainfo.ttf", 33, 0); // dodanie stylu czcionki o innej wielkosci
    ALLEGRO_BITMAP* button = al_create_bitmap(155, 60);  //tworzenie przycisku i ustalenie jego wymiarow
    ALLEGRO_DISPLAY* display =  statek.getDisplay(); // zmienna tworzaco okno gry 
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // zmienna tworzaca kolejke zdarzen 
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60); // zmienna tworzaca odswierzanie (odswierzanie 60 Hz/s)
    ALLEGRO_BITMAP* tlo = al_load_bitmap("tlomenu.png"); //ladowanie bitmapy tla menu
    ALLEGRO_BITMAP* bitmap = al_load_bitmap("statek.png");  //ladowanie bitmapy statku
    ALLEGRO_BITMAP* bitmap3 = al_load_bitmap("ufolud.png"); //ladowanie bitmapy ufoluda 
    ALLEGRO_BITMAP* boom = al_load_bitmap("boom.png"); // ladowanie bitmapy wybuchu 
    ALLEGRO_BITMAP* bitmapa_pocisku = al_load_bitmap("pocisk.png"); // ladowanie bitmapy pocisku 
    ALLEGRO_SAMPLE* sample = al_load_sample("menu.mp3"); //ladowanie muzyki ktora znajduje sie w menu
    ALLEGRO_SAMPLE* strzal = al_load_sample("pifpaf.mp3"); //ladowanie dzwieku strzalu gracza
    ALLEGRO_SAMPLE* boomA = al_load_sample("boomA.mp3"); //ladowanie dzwieku wybuchu ufoluda
    ALLEGRO_SAMPLE* obrazenia = al_load_sample("obrazenia.mp3");//ladowanie dzwieku obrazen gracza
    al_set_target_bitmap(button); //ustawienie aby okreslone zdarzenia byly przypiasne do przycisku 
    al_set_target_backbuffer(display);  // przekierowanie wszystkich zdarzen aby byly wyswietlone na ekranie 
    al_install_keyboard(); // instalacja klawiatury
    al_install_mouse(); // instalacja myszki
    al_register_event_source(queue, al_get_keyboard_event_source()); // rejestrowanie zdarzen klawiatury do kolejki
    al_register_event_source(queue, al_get_mouse_event_source()); // rejestrowanie zdarzen muszy do kolejki
    al_register_event_source(queue, al_get_display_event_source(display)); // rejestrowanie zdarzen okna do kolejki
    al_register_event_source(queue, al_get_timer_event_source(timer)); // rejestrowanie zdarzen czasu do kolejki
    al_start_timer(timer);// start zmiennej czasu 
    
    int width = al_get_display_width(display); // zapisanie szerokosci okna w zmiennej
    int height = al_get_display_height(display); // zapisanie wysokosci okna w zmiennej
    bool menu = true; //ustawienia wartoci menu na true na samym poczatku kodu zeby odrazu po uruchowmieniu aplikacji wyswietlony byl kod w if(menu)
    bool graj = false; //pozostawienie wartosci graj na false zeby byla mozliwosc przejscia do if(graj) z menu
    bool info = false;//pozostawienie wartosci info na false zeby byla mozliwosc przejscia do if(info) z menu 

    static int punktygracza = 0; // zmienna przechowuj¹ca liczbe punktow gracza
    static int playerLives = 3; // zmienna przechowuj¹ca liczbe zyc gracza
    bool mainWhile = true; //zmienna przechowujaca informacje czy glowana petla mainWhile zostala zakonczona
    bool wszystkieZestrzelone = false; // zmienna przechowujaca wartosci czy wszytkie ufoludy zostaly zestrzelone

    //zmienna przechowujaca pozycje X ufoluda 
    int pozycjaUfoludaX[55] = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550,
               50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550,
               50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550,
               50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550,
               50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550 };
    
    //zmienna przechowujaca pozycje Y ufoluda
    float pozycjaUfoludaY[55] = { 70.0, 70.0, 70.0, 70.0, 70.0, 70.0, 70.0, 70.0, 70.0, 70.0, 70.0,
                  110.0, 110.0, 110.0, 110.0, 110.0, 110.0, 110.0, 110.0, 110.0, 110.0, 110.0,
                  150.0, 150.0, 150.0, 150.0, 150.0, 150.0, 150.0, 150.0, 150.0, 150.0, 150.0,
                  190.0, 190.0, 190.0, 190.0, 190.0, 190.0, 190.0, 190.0, 190.0, 190.0, 190.0,
                  230.0, 230.0, 230.0, 230.0, 230.0, 230.0, 230.0, 230.0, 230.0, 230.0, 230.0 };

    //zmienna przechowujaca wartosc predkosci z jaka poruszaja sie ufoludy 
    float predkoscUfoluda[55] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };


    Pocisk pociski[1000]; //deklaracja tablicy pociski skladajaca sie z 1000 elementow typu Pocisk
    for (int i = 0; i < 1000; i++)  // ten for sluzy do iteracji przez tablice pociski i ustawia wlasciwosci pociskow 
    {
        pociski[i].aktywny = false; // zmienna aktywny okresla czy pocisk jest aktywny 
        pociski[i].PociskGracza = true; // zmienna PociskGracza okreslajaca ze pocisk nalezy do gracza
    }

    while (mainWhile)
    {
        ALLEGRO_EVENT event; // inicjacja eventow
        al_wait_for_event(queue, &event); // czekanie na event i zapisywanie ich w kolejce queue

        initMenu(menu, sample, event, tlo, font3d, width, font, mainWhile, graj, info);//funkcja manu
        initInfo(info, timer, event, font3dmenu, informacje, button, font, mainWhile, menu); //funkcja informacji 
        
        if (graj)
        {
            al_reserve_samples(18); // reserwacja dla pamieci dla 18 wartosici muzyki  
            al_get_keyboard_state(&keyState); // pobieranie statusu klawiatury
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // zamykanie przyciskiem x okna
                mainWhile = false;  

            if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                statek.poruszanieWPrawo(); //funckaj ktora okresla poruszanie sie w prawo

            if (al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                statek.poruszanieWLewo(); // funckaj ktora okresla poruszanie sie w lewo 
                statek.granica(); // funkcja okreslajaca granice poruszania sie statku

            if (event.type == ALLEGRO_EVENT_TIMER)//w celu wyswietlenia ta funkcja jest zwiazana z timerem
            {
                al_clear_to_color(al_map_rgb(0, 0, 0)); // wyczyszczenie ekranu
                al_draw_bitmap(bitmap2, 0, 0, 0); // narysowanie bitmapy tla

                bool zmianaKierunkuPoruszania = false; // zmienna przechowuj¹ca informacjê o zmianie kierunku ruchu ufoludow
                bool dotkniecieKrawedzi = false; // zmienna przechowujaca informacje o dotknieciu krawedzi przez ufoluda
                int czestotliwoscStrzalow = 250; //okreslenie losowosci wystrzeliwanych pociskow przez ufoludow 
               
                ufolud.tworzenieUfoludka(bitmap3, pozycjaUfoludaX, predkoscUfoluda, pozycjaUfoludaY, czestotliwoscStrzalow, pociski, display, dotkniecieKrawedzi, zmianaKierunkuPoruszania);
                
                
                // strzelanie statku 
                static bool spacjanicisnieta = false; //zmienna potrzeba do flagi spacji wykorzystujemy static poniewaz wartosc bedzie przechowywana miedzy kolejnymi wywolaniami
                                                      //i jej wartosc nie bedzie tracona zaraz po zakonczeniu funkcji 
                if (al_key_down(&keyState, ALLEGRO_KEY_SPACE) && !spacjanicisnieta) {
                    spacjanicisnieta = true; // ustawienie flagi ze spacja zostala wcisnieta
                    // znajdz nieaktywny pocisk
                    Pocisk* bullet = NULL;
                    for (int i = 0; i < 1000; i++)
                    {
                        if (!pociski[i].aktywny)
                        {
                            bullet = &pociski[i];
                            break;
                        }
                    }
                    // jesli znaleziono nieaktywny pocisk ustaw jego pozycje i aktywnosc
                    if (bullet) {
                        bullet->x = statek.pobieraniePozycjiStatku() + 20; // pozycja pocisku na osi X (20 to polowa szerokosci statku)
                        bullet->y = height - 50; // pozycja pocisku na osi Y
                        bullet->aktywny = true;
                        bullet->PociskGracza = true;
                        al_play_sample(strzal, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); // odtworzenie dzwieku strzalu
                    }
                }
                // resetowanie flagi po puszczeniu spacji
                if (!al_key_down(&keyState, ALLEGRO_KEY_SPACE)) {
                    spacjanicisnieta = false;
                }

                // ruch pocisków i kolizje
                for (int i = 0; i < 1000; i++)
                {
                    if (pociski[i].aktywny) //sprawdzanie aktywnosci pociskow
                    {
                        kolizjePociskow(i, pociski);//funkcja sprawdzajaca kolizje miedzy pociskami

                        al_draw_bitmap(bitmapa_pocisku, pociski[i].x, pociski[i].y, 0); // rysowanie bitmapy pocisku

                        pociski_gracza_ufoluda(pociski, i); //funkcja od pociskow gracza i ufoluda okreslajaca w ktora strone maja sie poruszac

                        float czasWybuchu = 0.2; // czas wybuchu w sekundach
                        float startTime = al_get_time(); // czas rozpoczêcia wybuchu
                        bool eksplozjaskonczona = false; // flaga informuj¹ca o zakonczeniu wybuchu
                        sprawdzanie_kolizji(pozycjaUfoludaY, pociski, i, pozycjaUfoludaX, boom, punktygracza, eksplozjaskonczona, startTime, czasWybuchu, boomA); //funkcja ktora spawdza kolizje z ufoludami 

                        // kod sprawdzaj¹cy kolizje z graczem
                        // Warunek jest spelniony jesli pocisk nie nalezy do gracza (PociskGracza == false) 
                        // i jesli odleglosc pocisku od srodka statku (x + 24) jest mniejsza ni¿ 24 (polowa szerokosci statku)
                        // oraz odleglosc pocisku od dolnej krawedzi ekranu (SCREEN_H - 24) jest mniejsza niz 20 (polowa wysokosci statku).
                        if (pociski[i].PociskGracza == false && abs(pociski[i].x - (statek.pobieraniePozycjiStatku() + 23)) < 23 && abs(pociski[i].y - (height - 40)) < 20)
                        {
                            pociski[i].aktywny = false; // usun pocisk
                            playerLives -= 1; // zmniejsz liczbe zyc gracza o 1
                            al_play_sample(obrazenia, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

                            if (playerLives <= 0) // jesli gracz stracil wszystkie zycia to gra sie konczy
                            {
                                bool gameOver = true; // zmienna okreslajaca ze gra sie skonczyla

                                gameOverAction(ufolud, gameOver, timer, queue, informacjekoniec, width, punktygracza, mainWhile, playerLives, pozycjaUfoludaX, pozycjaUfoludaY, predkoscUfoluda, pociski, statek, graj, menu);
                            }
                        }
                        if (pozycjaUfoludaY[i] == 400) // jesli ufoludy dotra do wartosci 400 na ekranie gra sie konczy
                        {
                            bool gameOver = true;
                            gameOverAction(ufolud, gameOver, timer, queue, informacjekoniec, width, punktygracza, mainWhile, playerLives, pozycjaUfoludaX, pozycjaUfoludaY, predkoscUfoluda, pociski, statek, graj, menu);
                        }
                    }
                    //petla ktora sprawdza czy wszystkie ufoludy zostaly zastrzelone 
                    wygranieGry(wszystkieZestrzelone, pozycjaUfoludaY, queue, mainWhile, width, informacjekoniec, playerLives, pozycjaUfoludaX, predkoscUfoluda, pociski, czestotliwoscStrzalow, statek, graj, ufolud, punktygracza, menu);
                }

                al_draw_textf(font, al_map_rgb(255, 255, 255), width - 100, 10, ALLEGRO_ALIGN_RIGHT, "Zycie: %d", playerLives); // wyswietlanie ilosci zyc gracz
                al_draw_textf(font, al_map_rgb(255, 255, 255), width - 480, 10, ALLEGRO_ALIGN_CENTER, "Punkty:%d", punktygracza); // wyswietlanie ilosci punktow gracza
                al_draw_bitmap(bitmap, statek.pobieraniePozycjiStatku(), height - al_get_bitmap_height(bitmap), 0);; // rysowanie statku
                al_draw_bitmap(button, 0, 0, 0); // rysowanie przycisku
                al_draw_text(font, al_map_rgb(255, 255, 255), 20, 0, 0, "<- KONIEC GRY");
                al_flip_display();
            }
            // przycisk wyjscia z gry i powrotu do menu
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && event.mouse.x >= 0 && event.mouse.x <= 200 && event.mouse.y >= 0 && event.mouse.y <= 70)
            {
                graj = false;
                startX = 420;
                playerLives = 3; // przywróæ 3 zycia graczowi
                punktygracza = 0;
                ufolud.resetowaniePozycjiUfoludkow(pozycjaUfoludaX, pozycjaUfoludaY, predkoscUfoluda, pociski);
                menu = true;
            }
        }
    }

    niszczenieObiektow(bitmapa_pocisku, queue, button, statek, display, timer, bitmap, bitmap3, bitmap2, tlo, boom, sample, strzal, boomA, obrazenia);
    return 0;
}




void wygranieGry(bool& wszystkieZestrzelone, float  pozycjaUfoludaY[55], ALLEGRO_EVENT_QUEUE* queue, bool& mainWhile, int width, ALLEGRO_FONT* informacjekoniec, int& playerLives, int  pozycjaUfoludaX[55], float  predkoscUfoluda[55], Pocisk  pociski[1000], int& czestotliwoscStrzalow, Statek& statek, bool& graj, Ufolud& ufolud, int& punktygracza, bool& menu)
{
    wszystkieZestrzelone = true; //zmienna ktora przechowuje informacje czy wszytkie ufoludy zostaly zestrzelone
    for (int i = 0; i <= 54; i++) {
        // sprawdzanie czy jakikolwiek ufolud jest jeszcze na mapie 
        if (pozycjaUfoludaY[i] > 0) {
            wszystkieZestrzelone = false; // ustawienie zmiennej na false gdy przynajmniej jeden ufolud jest jeszcze na mapie
            break; //petla jest przerywana
        }
    }
    if (wszystkieZestrzelone) {

        bool wygrana = true;
        while (wygrana)
        {
            ALLEGRO_EVENT event;
            al_wait_for_event(queue, &event); // czekaj na zdarzenie

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // mozliwosc wyjscia przyciskiem x na oknie
                mainWhile = false;

            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) // jesli kliknieto przycisk myszy
            {
                if (event.mouse.x >= (width / 2 + 90) - al_get_text_width(informacjekoniec, "Kolejny poziom") && event.mouse.x <= (width / 2 + 90) &&
                    event.mouse.y >= 320 && event.mouse.y <= 320 + al_get_font_line_height(informacjekoniec)) // jesli kliknieto przycisk rozpocznie sie kolejny poziom
                {
                    for (int i = 0; i < 55; i++)
                    {
                        pozycjaUfoludaX[i] = 50 + i % 11 * 50; // ustawienie pozycji ufoludow na osi Y
                        pozycjaUfoludaY[i] = 70.0 + i / 11 * 40.0; // ustawienie pozycji ufoludow na osi X
                        predkoscUfoluda[i] = 2.0; // ustawienie predkosci ufoludow 
                        pociski[i].aktywny = false; // ustaw aktywnosc pociskow na false
                        czestotliwoscStrzalow = 150; // ustawienie czestotliwosci strzalow na 150 w celu czestrzego losowego strzelania
                    }
                    statek.pobieraniePozycjiStatku(); 
                    graj = true;
                    wygrana = false; // zakoncz petle while
                }
                else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && // wspolrzedne i wymagania wydarzenie aby przycisk "Powrot do menu" przekierowal nas do menu
                    event.mouse.x >= (width / 2 + 150) - al_get_text_width(informacjekoniec, "Powrot do menu") && event.mouse.x <= (width / 2 + 150) &&
                    event.mouse.y >= 400 && event.mouse.y <= 400 + al_get_font_line_height(informacjekoniec)) 
                {
                    // kod wracaj¹cy do menu
                    wygrana = false; // ustaw wygrana na false, zeby wyjsc z petli while
                    ufolud.resetowaniePozycjiUfoludkow(pozycjaUfoludaX, pozycjaUfoludaY, predkoscUfoluda, pociski); // funkcja resetujaca pozycje ufoludow
                    statek.pobieraniePozycjiStatku(); // pobieranie pozycji statku
                    punktygracza = 0; // wyzeruj ilosc zebranych punktow
                    playerLives = 3; // przywroc 3 zycia graczowi
                    graj = false;
                    menu = true;
                }
            }
            al_clear_to_color(al_map_rgb(0, 0, 0)); // wyczysc ekran
            al_draw_text(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 90) - al_get_text_width(informacjekoniec, "Wygrales!"), 180, 0, "Wygrales!"); //wypisz tekst na ekranie 
            al_draw_textf(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 140) - al_get_text_width(informacjekoniec, "Punkty gracza: %d"), 240, 0, "Punkty gracza: %d", punktygracza);
            al_draw_text(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 140) - al_get_text_width(informacjekoniec, "Kolejny poziom"), 320, 0, "Kolejny poziom");
            al_draw_text(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 150) - al_get_text_width(informacjekoniec, "Powrot do menu"), 400, 0, "Powrot do menu");
            al_flip_display(); // wyswietl na ekranie
        }
    }
}




void gameOverAction(Ufolud ufolud, bool& gameOver, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* informacjekoniec, int width, int& punktygracza, bool& mainWhile, int& playerLives, int  pozycjaUfoludaX[55], float  pozycjaUfoludaY[55], float  predkoscUfoluda[55], Pocisk  pociski[1000], Statek& statek, bool& graj, bool& menu)
{
    while (gameOver)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event); // czekaj na zdarzenie
        al_clear_to_color(al_map_rgb(0, 0, 0)); // wyczysc ekran
        al_draw_text(informacjekoniec, al_map_rgb(153, 0, 153), (width / 2 + 110) - al_get_text_width(informacjekoniec, "Game    Over"), 180, 0, "Game   Over"); // wyswietlanie komunikatow tekstowych 
        al_draw_textf(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 150) - al_get_text_width(informacjekoniec, "Punkty gracza: %d"), 240, 0, "Punkty gracza: %d", punktygracza);
        al_draw_text(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 130) - al_get_text_width(informacjekoniec, "Powrot do menu"), 320, 0, "Powrot do menu");
        al_draw_text(informacjekoniec, al_map_rgb(0, 255, 0), (width / 2 + 70) - al_get_text_width(informacjekoniec, "Reset gry"), 400, 0, "Reset gry");
        al_flip_display(); // wyswietlanie na ekranie 

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // mozliwosc wyjscia przyciskiem x na oknie
            mainWhile = false;

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) // jesli kliknieto przycisk myszy
        {
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && // wspolrzedne i wymagania wydarzenie aby przycisk "Powrot do menu" przekierowal nas do menu
                event.mouse.x >= (width / 2 + 130) - al_get_text_width(informacjekoniec, "Powrot do menu") && event.mouse.x <= (width / 2 + 130) &&
                event.mouse.y >= 320 && event.mouse.y <= 320 + al_get_font_line_height(informacjekoniec)) 
            {
                // kod wracaj¹cy do menu
                gameOver = false; // ustaw gameOver na false zeby wyjsc z petli while
                playerLives = 3; // przywroc 3 zycia graczowi
                ufolud.resetowaniePozycjiUfoludkow(pozycjaUfoludaX, pozycjaUfoludaY, predkoscUfoluda, pociski);
                statek.pobieraniePozycjiStatku();
                graj = false;
                menu = true;
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP &&
                event.mouse.x >= (width / 2 + 70) - al_get_text_width(informacjekoniec, "Reset gry") && event.mouse.x <= (width / 2 + 70) &&
                event.mouse.y >= 400 && event.mouse.y <= 400 + al_get_font_line_height(informacjekoniec)) // jesli kliknieto przycisk restartu
            {
                playerLives = 3; // przywroc 3 zycia graczowi
                ufolud.resetowaniePozycjiUfoludkow(pozycjaUfoludaX, pozycjaUfoludaY, predkoscUfoluda, pociski);
                statek.pobieraniePozycjiStatku();
                punktygracza = 0;
                gameOver = false; // ustaw gameOver na false zeby wyjsc z petli while
            }
        }
    }
}

void sprawdzanie_kolizji(float  pozycjaUfoludaY[55], Pocisk  pociski[1000], int i, int  pozycjaUfoludaX[55], ALLEGRO_BITMAP* boom, int& punktygracza, bool& eksplozjaskonczona, float startTime, float czasWybuchu, ALLEGRO_SAMPLE* boomA)
{
    // sprawdz kolizje z ufoludami
    for (int j = 0; j < 55; j++)
    {
        //pozycjaUfoludaY[j] > 0 - ufolud znajduje siê na planszy
        //pociski[i].x >= pozycjaUfoludaX[j] - pocisk znajduje sie na lub po prawej stronie ufoluda (na osi X).
        //pociski[i].x <= pozycjaUfoludaX[j] + 50 - pocisk znajduje sie na lub przed lewa strona ufoluda(na osi X).
        //pociski[i].y >= pozycjaUfoludaY[j] - pocisk znajduje sie na lub ponizej ufoluda (na osi Y).
        //pociski[i].y <= pozycjaUfoludaY[j] + 50 - pocisk znajduje siê na lub powyzej dolnej strony ufoluda(na osi Y).
        //pociski[i].PociskGracza - pocisk jest pociskiem gracza.
        if (pozycjaUfoludaY[j] > 0 && pociski[i].x >= pozycjaUfoludaX[j] && pociski[i].x <= pozycjaUfoludaX[j] + 50 && pociski[i].y >= pozycjaUfoludaY[j] && pociski[i].y <= pozycjaUfoludaY[j] + 50 && pociski[i].PociskGracza)
        {
            pociski[i].aktywny = false; // usun pocisk
            al_draw_bitmap(boom, pozycjaUfoludaX[j], pozycjaUfoludaY[j], 0); // rysuj wybuch
            // wyswietl wybuch while wykonywana przez okreslony czas
            punktygracza += 100;
            while (!eksplozjaskonczona)
            {
                float czasKtoryUplynol = al_get_time() - startTime; // czas ktory uplynal od rozpoczecia wybuchu
                if (czasKtoryUplynol < czasWybuchu)
                {
                    al_draw_bitmap(boom, pozycjaUfoludaX[j], pozycjaUfoludaY[j], 0); // rysuj wybuch
                    al_play_sample(boomA, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); // odtworzenie dzwieku wybuchu 
                    al_flip_display(); // wyswietl wybuch na ekranie
                    eksplozjaskonczona = true;
                }

            }
            pozycjaUfoludaY[j] -= 2000; // usun ufoluda
            break;
        }

    }
}

void pociski_gracza_ufoluda(Pocisk  pociski[1000], int i)
{
    if (pociski[i].PociskGracza) {
        pociski[i].y -= PREDKOSC_POCISKU; // pocisk gracza leci w gore

        // usun pocisk gracza jesli przekroczyl gorna krawedz ekranu
        if (pociski[i].y < 0)
        {
            pociski[i].aktywny = false; // jesli dotarl do wartosci 0 ustawiamy jego aktywnosc na false 
        }
    }
    else {
        pociski[i].y += PREDKOSC_POCISKU; // pocisk ufoluda leci w dol

        // usun pocisk ufoludka jesli przekroczyl dolna krawedz ekranu
        if (pociski[i].y > 620)
        {
            pociski[i].aktywny = false; // jesli dotarl do wartosci 620 ustawiamy jego aktywnosc na false
        }
    }
}

void kolizjePociskow(int i, Pocisk  pociski[1000])
{
    for (int j = i + 1; j < 1000; j++) //sprawdzanie kolizji pociskow
    {
        if (pociski[j].aktywny && pociski[i].PociskGracza != pociski[j].PociskGracza
            && abs(pociski[i].x - pociski[j].x) < 10 && abs(pociski[i].y - pociski[j].y) < 10)
        {
            // jesli pociski natrafia na siebie ustawiamy ich aktywnosc na false
            pociski[i].aktywny = false; 
            pociski[j].aktywny = false;
            break;
        }
    }
}



void niszczenieObiektow(ALLEGRO_BITMAP* bullet_bitmap, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* button, Statek& statek, ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer, ALLEGRO_BITMAP* bitmap, ALLEGRO_BITMAP* bitmap3, ALLEGRO_BITMAP* bitmap2, ALLEGRO_BITMAP* tlo, ALLEGRO_BITMAP* boom, ALLEGRO_SAMPLE* sample, ALLEGRO_SAMPLE* strzal, ALLEGRO_SAMPLE* boomA, ALLEGRO_SAMPLE* obrazenia)
{
    al_destroy_bitmap(bullet_bitmap);
    al_destroy_bitmap(bitmap);
    al_destroy_bitmap(bitmap3);
    al_destroy_bitmap(bitmap2);
    al_destroy_bitmap(tlo);
    al_destroy_bitmap(boom);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(button);
    al_uninstall_mouse();
    al_destroy_display(statek.getDisplay());
    al_uninstall_keyboard();
    al_destroy_sample(sample);
    al_destroy_sample(strzal);
    al_destroy_sample(boomA);
    al_destroy_sample(obrazenia);
    al_uninstall_audio();
    al_destroy_timer(timer);
}


void initAllegro()
{
    al_init(); //inicjacja allegro
    al_init_font_addon(); // inicjacja czcionki
    al_init_ttf_addon(); // inicjacja stylu czcionki
    al_init_image_addon(); // inicjacja dodawania zdjec
    al_install_audio(); // instalowanie audio
    al_init_acodec_addon(); // inicjowanie elementow audio
}

void initMenu(bool& menu, ALLEGRO_SAMPLE* sample, ALLEGRO_EVENT& event, ALLEGRO_BITMAP* tlo, ALLEGRO_FONT* font3d, int width, ALLEGRO_FONT* font, bool& mainWhile, bool& graj, bool& info)
{
    if (menu) {
        al_reserve_samples(1); // rezerwacja pamieci dla muzyki
        al_play_sample(sample, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL); // wczytanie dzwieku ktory ma byc odtwarany w menu LOOP oznacza ze ma leciec w petli

        if (event.type == ALLEGRO_EVENT_TIMER) //w celu wyswietlenia ta funkcja jest zwiazana z timerem
        {
            //wyswietlenie tekstu o podanym kolrze i podajej wielkosci i stylu czcioski 
            al_draw_bitmap(tlo, 0, 0, 0); 
            al_draw_text(font3d, al_map_rgb(0, 255, 0), (width / 2) - al_get_text_width(font3d, " VADERS"), 10, 0, "SPACE INVADERS");
            al_draw_text(font, al_map_rgb(0, 255, 0), (width / 2) - al_get_text_width(font, "GRAJ"), 200, 0, "GRAJ");
            al_draw_text(font, al_map_rgb(0, 255, 0), (width / 2) - al_get_text_width(font, " RMACJE"), 300, 0, "INFORMACJE");
            al_draw_text(font, al_map_rgb(0, 255, 0), (width / 2) - al_get_text_width(font, "YJSCIE"), 400, 0, "WYJSCIE");
            al_flip_display(); //wyswietlenie na ekranie 
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // mozliwosc wyjscia przyciskiem x na oknie
            mainWhile = false; //ustawienie wartosci na false w celu zakonczenia glownej petli gry

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && // wspolrzedne i wymagania wydarzenia aby przycisk "GRAJ" wprowadzal nas do if(graj)
            event.mouse.x >= (width / 2) - al_get_text_width(font, "GRAJ") && event.mouse.x <= (width / 2) &&
            event.mouse.y >= 200 && event.mouse.y <= 200 + al_get_font_line_height(font))
        {

            graj = true; //ustawienie wartosci na true w celu przejscia do if(graj)
            al_stop_samples(); // zatrzymaj odtwarzanie dzwieku z menu
            menu = false; // ustawienia wartosci na false w celu zakonczenia if(menu)
        }

        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && // wspolrzedne i wymagania wydarzenia aby przycisk "WYJSCIE" zamykal aplikacje
            event.mouse.x >= (width / 2) - al_get_text_width(font, "YJSCIE") && event.mouse.x <= (width / 2) + al_get_text_width(font, "IE") &&
            event.mouse.y >= 400 && event.mouse.y <= 400 + al_get_font_line_height(font))
            mainWhile = false; //ustawienie wartosci na false w celu zakonczenia glownej petli gry

        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && // wspolrzedne i wymagania wydarzenia aby przcisk "Informacje" przechodzil do if(info)
            event.mouse.x >= (width / 2) - al_get_text_width(font, " RMACJE") && event.mouse.x <= (width / 2) + al_get_text_width(font, "E") &&
            event.mouse.y >= 300 && event.mouse.y <= 300 + al_get_font_line_height(font))
        {
            info = true; //ustawienie wartosci na true w celu przejscia do if(info)
            menu = false; //ustawienie wartosci na false w celu zakonczenia if(menu)
        }

    }
}
void initInfo(bool& info, ALLEGRO_TIMER* timer, ALLEGRO_EVENT& event, ALLEGRO_FONT* font3dmenu, ALLEGRO_FONT* informacje, ALLEGRO_BITMAP* button, ALLEGRO_FONT* font, bool& mainWhile, bool& menu)
{
    if (info)
    {
        if (event.type == ALLEGRO_EVENT_TIMER) //w celu wyswietlenia ta funkcja jest zwiazana z timerem
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font3dmenu, al_map_rgb(0, 255, 0), 150, 0, 0, "INFORMACJE O GRZE");
            al_draw_text(informacje, al_map_rgb(0, 255, 0), 50, 150, 0, "- Celem gry jest zestrzelenie wszystkich przeciwnikow na ekranie");
            al_draw_text(informacje, al_map_rgb(0, 255, 0), 50, 220, 0, "- Gracz porusza sie na ekranie za pomoca strzalek lewo i prawo");
            al_draw_text(informacje, al_map_rgb(0, 255, 0), 50, 290, 0, "- Gracz ma mozliwosc strzelania do przeciwnikow za pomoca spacji");
            al_draw_text(informacje, al_map_rgb(0, 255, 0), 50, 360, 0, "- Ufolduki sa przeciwnikami ktore ruszaja sie i moga strzelac do gracza");
            al_draw_text(informacje, al_map_rgb(0, 255, 0), 50, 430, 0, "- Gra konczy sie gdy gracz straci wszystkie zycia lub zabije ufoludki");
            al_draw_text(informacje, al_map_rgb(0, 255, 0), 50, 590, 0, "- Tworcy: Tomasz Chojnacki,  Przemyslaw Palka,  Maciej Bernatek");
            al_draw_bitmap(button, 350, 500, 0); // rysowanie przycisku
            al_draw_text(font, al_map_rgb(0, 255, 0), 400, 500, 0, "POWROT");
            al_flip_display();
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // mozliwosc wyjscia przyciskiem x na oknie
            mainWhile = false;
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && //  wspolrzedne i wymagania wydarzenia aby "POWROT" powracal do manu
            event.mouse.x >= 350 && event.mouse.x <= 350 + 200 &&
            event.mouse.y >= 500 && event.mouse.y <= 570)
        {

            info = false; //ustawienie wartosci na false w celu zakonczenia if(info)
            menu = true; //ustawienie wartosci na true w celu przejscia do if(menu)

        }
    }
}
