#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "objetos.h"



const int width = 640;
const int height = 480;

void InitJogador(Jogador& jogador);
void DrawJogador(Jogador& jogador);

int main(void)
{
	
	bool done = false;

	Jogador jogador;

	ALLEGRO_DISPLAY* display = NULL;

	if (!al_init())										
		return -1;

	display = al_create_display(width, height);			

	if (!display)										
		return -1;

	al_init_primitives_addon();
	InitJogador(jogador);
	
	while (!done)
	{
		
		DrawJogador(jogador);

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));

	}		

	return 0;
}

void InitJogador(Jogador& jogador) {
	jogador.x = width / 2;
	jogador.y = 450;
	jogador.ID = JOGADOR;
	jogador.vidas = 1;

	//testar velocidades variadas depois
	jogador.velocidade = 7;

	//necessario para sistema de hitbox
	jogador.boundx = 6;
	jogador.boundy = 7;
}

void DrawJogador(Jogador& jogador) {
	al_draw_rectangle(jogador.x - 25, jogador.y - 75, jogador.x + 25, jogador.y, al_map_rgb(255, 0, 0), 2);

}