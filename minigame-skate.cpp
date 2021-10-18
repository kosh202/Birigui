#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "objetos.h"

const int width = 640;
const int height = 480;

const int numeroObstaculos = 10;

bool pulo = false;

int velocidadeMaxPulo = 15;
int gravidade = 1;
int velocidadePulo = velocidadeMaxPulo;

enum KEYS {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

bool keys[4] = {
	false,
	false,
	false,
	false
};

void InitJogador(Jogador& jogador);
void DrawJogador(Jogador& jogador);
void MoveJogadorLeft(Jogador& jogador);
void MoveJogadorRight(Jogador& jogador);
void MakeJogadorJump(Jogador& jogador);

void InitObstaculo(Obstaculo obstaculo[], int size);
void DrawObstaculo(Obstaculo obstaculo[], int size);
void CreateObstaculo(Obstaculo obstaculo[], int size);
void UpdateObstaculo(Obstaculo obstaculo[], int size);
void DrawPassedObstaculo(Obstaculo obstaculo[], int size);

int Collision(Obstaculo obstaculo[], Jogador& jogador);

int main(void) {
	bool done = false;
	bool redraw = true;
	const int FPS = 60;

	Jogador jogador;
	Obstaculo obstaculos[10];

	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* gameTimer = NULL;
	ALLEGRO_TIMER* obstaculoTimer = NULL;
	ALLEGRO_TIMER* updateObstaculoTimer = NULL;
	ALLEGRO_TIMER* gameTime = NULL;

	if (!al_init())
		return -1;

	display = al_create_display(width, height);

	if (!display)
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();

	event_queue = al_create_event_queue();
	gameTimer = al_create_timer(1.0 / FPS);
	obstaculoTimer = al_create_timer(0.6);
	updateObstaculoTimer = al_create_timer(0.035);
	gameTime = al_create_timer(1.0);

	srand(time(NULL));

	InitJogador(jogador);
	InitObstaculo(obstaculos, numeroObstaculos);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(gameTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(obstaculoTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(updateObstaculoTimer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	ALLEGRO_FONT* font18 = al_load_font("Roboto-Regular.ttf", 18, 0);

	al_start_timer(gameTimer);
	al_start_timer(obstaculoTimer);
	al_start_timer(updateObstaculoTimer);
	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);



		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;

			if (keys[UP]) {
				if (!pulo) {
					pulo = true;
				}
			}

			if (keys[LEFT]) {
				MoveJogadorLeft(jogador);
				keys[LEFT] = false;
			}

			if (keys[RIGHT]) {
				MoveJogadorRight(jogador);
				keys[RIGHT] = false;
			}

			if (pulo == true) {
				MakeJogadorJump(jogador);
			}
		} if (ev.timer.source == obstaculoTimer && rand() % 5 < 4) {
			CreateObstaculo(obstaculos, numeroObstaculos);

			if (rand() % 10 == 0) {
				CreateObstaculo(obstaculos, numeroObstaculos);
			}

			CreateObstaculo(obstaculos, numeroObstaculos);
		} if (ev.timer.source == updateObstaculoTimer) {
			UpdateObstaculo(obstaculos, numeroObstaculos);
			if (Collision(obstaculos, jogador)) {
				al_flush_event_queue(event_queue);
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_flip_display();
				al_rest(3);
				done = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_W:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_W:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			DrawObstaculo(obstaculos, numeroObstaculos);
			DrawJogador(jogador);
			DrawPassedObstaculo(obstaculos, numeroObstaculos);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}

	}


	al_destroy_display(display);

	return 0;
}

void InitJogador(Jogador& jogador) {
	jogador.ID = JOGADOR;

	jogador.posicao = MEIO;

	jogador.x1 = 295; //25
	jogador.x2 = 345; //25
	jogador.y1 = 450; //parte inferior da tela "chão"
	jogador.y2 = 375; //chão + 75

	jogador.status = VIVO;
}

void DrawJogador(Jogador& jogador) {
	al_draw_filled_rectangle(jogador.x1, jogador.y1, jogador.x2, jogador.y2, al_map_rgb(255, 0, 0));
}

void MoveJogadorLeft(Jogador& jogador) {
	if (jogador.posicao != ESQUERDA) {
		jogador.posicao -= 1;
		jogador.x1 -= 90;
		jogador.x2 -= 90;
	}
}

void MoveJogadorRight(Jogador& jogador) {
	if (jogador.posicao != DIREITA) {
		jogador.posicao += 1;
		jogador.x1 += 90;
		jogador.x2 += 90;
	}
}

void MakeJogadorJump(Jogador& jogador) {
	jogador.y1 -= velocidadePulo;
	jogador.y2 -= velocidadePulo;

	velocidadePulo -= gravidade;

	if (velocidadePulo < -velocidadeMaxPulo) {
		velocidadePulo = velocidadeMaxPulo;
		pulo = false;
	}
}

void InitObstaculo(Obstaculo obstaculo[], int size) {
	for (int i = 0; i < size; i++) {
		obstaculo[i].ID = OBSTACULOS;
		obstaculo[i].velocidade = 1.0;
		obstaculo[i].status = MORTO;
	}
}

void CreateObstaculo(Obstaculo obstaculo[], int size) {
	for (int i = 0; i < size; i++) {
		if (obstaculo[i].status == MORTO) {
			obstaculo[i].tipo = rand() % 2;
			obstaculo[i].posicao = rand() % 3;
			obstaculo[i].edgeRate = 1;
			obstaculo[i].edgeRate2 = 1;
			switch (obstaculo[i].tipo) {
			case 0:
				switch (obstaculo[i].posicao) {
				case 0:
					obstaculo[i].x1 = 280 + 25;
					obstaculo[i].y1 = 145;
					obstaculo[i].x2 = 280 + 30;
					obstaculo[i].y2 = 150;
					obstaculo[i].status = VIVO;
					break;
				case 1:
					obstaculo[i].x1 = 280 + 40;
					obstaculo[i].y1 = 145;
					obstaculo[i].x2 = 280 + 45;
					obstaculo[i].y2 = 150;
					obstaculo[i].status = VIVO;
					break;
				case 2:
					obstaculo[i].x1 = 280 + 55;
					obstaculo[i].y1 = 145;
					obstaculo[i].x2 = 280 + 70;
					obstaculo[i].y2 = 150;
					obstaculo[i].status = VIVO;
					break;
				case 3:
					obstaculo[i].status = MORTO;
				}
				break;
			case 1:
				switch (obstaculo[i].posicao) {
				case 0:
					obstaculo[i].x1 = 280 + 25;
					obstaculo[i].y1 = 145;
					obstaculo[i].x2 = 280 + 30;
					obstaculo[i].y2 = 150;
					obstaculo[i].status = VIVO;
					break;
				case 1:
					obstaculo[i].x1 = 280 + 40;
					obstaculo[i].y1 = 145;
					obstaculo[i].x2 = 280 + 45;
					obstaculo[i].y2 = 150;
					obstaculo[i].status = VIVO;
					break;
				case 2:
					obstaculo[i].x1 = 280 + 55;
					obstaculo[i].y1 = 145;
					obstaculo[i].x2 = 280 + 70;
					obstaculo[i].y2 = 150;
					obstaculo[i].status = VIVO;
					break;
				case 3:
					obstaculo[i].status = MORTO;
				}
				break;
			case 2:
				break;
			}
			return;
		}
	}
}

void UpdateObstaculo(Obstaculo obstaculo[], int size) {
	for (int i = 0; i < size; i++) {
		if (obstaculo[i].status == VIVO) {
			obstaculo[i].y1 += obstaculo[i].velocidade;
			obstaculo[i].y2 += obstaculo[i].velocidade + obstaculo[i].edgeRate2;
			obstaculo[i].edgeRate2 += 0.15;
			if (obstaculo[i].posicao == 0) {
				obstaculo[i].x1 -= obstaculo[i].edgeRate;
				obstaculo[i].x2 -= obstaculo[i].edgeRate / 2;
				obstaculo[i].edgeRate += 0.29;
			}
			else if (obstaculo[i].posicao == 1) {
				obstaculo[i].x1 -= obstaculo[i].edgeRate / 2;
				obstaculo[i].x2 += obstaculo[i].edgeRate / 2;
				obstaculo[i].edgeRate += 0.15;
			}
			else if (obstaculo[i].posicao == 2) {
				obstaculo[i].x1 += obstaculo[i].edgeRate / 2;
				obstaculo[i].x2 += obstaculo[i].edgeRate;
				obstaculo[i].edgeRate += 0.29;
			}
			obstaculo[i].velocidade += 0.5;
			if (obstaculo[i].y1 < 150 && obstaculo[i].y1 < 250) {
				obstaculo[i].velocidade -= 0.01;
			}
			else if (obstaculo[i].y1 >= 150 && obstaculo[i].y1 < 315) {
				obstaculo[i].velocidade += 0.15;
			}
			else if (obstaculo[i].y1 >= 315 && obstaculo[i].y1 <= 500) {
				obstaculo[i].velocidade += 0.01;
			} if (obstaculo[i].y1 > height) {
				obstaculo[i].status = MORTO;
				obstaculo[i].velocidade = 1;
				obstaculo[i].edgeRate = 1;
				obstaculo[i].edgeRate2 = 1;
			}
		}
	}
}

void DrawObstaculo(Obstaculo obstaculo[], int size) {
	for (int i = 0; i < size; i++) {
		if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CONE && obstaculo[i].y2 <= 450) {
			al_draw_filled_rectangle(obstaculo[i].x1, obstaculo[i].y1, obstaculo[i].x2, obstaculo[i].y2, al_map_rgb(0, 0, 255));
		} if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CORRIMAO && obstaculo[i].y2 <= 450) {
			al_draw_filled_rectangle(obstaculo[i].x1, obstaculo[i].y1, obstaculo[i].x2, obstaculo[i].y2, al_map_rgb(0, 255, 0));
		}
	}
}

void DrawPassedObstaculo(Obstaculo obstaculo[], int size) {
	for (int i = 0; i < size; i++) {
		if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CONE && obstaculo[i].y2 >= 450) {
			al_draw_filled_rectangle(obstaculo[i].x1, obstaculo[i].y1, obstaculo[i].x2, obstaculo[i].y2, al_map_rgb(0, 0, 255));
		} if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CORRIMAO && obstaculo[i].y2 >= 450) {
			al_draw_filled_rectangle(obstaculo[i].x1, obstaculo[i].y1, obstaculo[i].x2, obstaculo[i].y2, al_map_rgb(0, 255, 0));
		}
	}
}

int Collision(Obstaculo obstaculo[], Jogador& jogador) {
	for (int i = 0; i < numeroObstaculos; i++) {
		if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CONE) {
			if () {
				jogador.status = MORTO;
				return 1;
			}
		}
	}
	return 0;
}