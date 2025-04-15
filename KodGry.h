#pragma once

void initAllegro();

void initMenu(bool& menu, ALLEGRO_SAMPLE* sample, ALLEGRO_EVENT& event, ALLEGRO_BITMAP* tlo, ALLEGRO_FONT* font3d, int width, ALLEGRO_FONT* font, bool& mainWhile, bool& graj, bool& info);

void initInfo(bool& info, ALLEGRO_TIMER* timer, ALLEGRO_EVENT& event, ALLEGRO_FONT* font3dmenu, ALLEGRO_FONT* informacje, ALLEGRO_BITMAP* button, ALLEGRO_FONT* font, bool& mainWhile, bool& menu);
