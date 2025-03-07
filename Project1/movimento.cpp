﻿#pragma once
#include <allegro5/allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include "constantes.h"
#include "desenho.h"
#include "definicao.h"
#include "placar.h"
#include "movimento.h"
#include "structs.h"

//Variáveis
double aceleracao = 1.1;
int velocidadeBase = 10;
bool keys[6] = {
	false,
	false,
	false,
	false,
	false,
	false
};

bool pulo = false;
int velocidadeMaxPulo = 16;
int gravidade = 1;
int velocidadePulo = velocidadeMaxPulo;
bool colidiuAntes = false;


void movementLeft(Player& p) {
	p.x -= 30;
}

void movementRight(Player& p) {
	p.x += 30;
}

void moveBall(Ball& b) {
	al_draw_filled_circle(b.x, b.y, b.raio, al_map_rgb(0, 0, 0));
	b.x += ((velocidadeBase / 4) * aceleracao) * b.xDir;
	b.y += (velocidadeBase * aceleracao) * b.yDir;
	aceleracao += 0.002;

}
/*
	return 0 -> Não colidiu
	return 1 -> Colidiu ou com o player
	return 2 -> Colidiu com o bot
	return 3 -> Colidiu com as extremidades Y do cenário
	return 4 -> Colidiu com o canto esquerdo do cenário
	return 5 -> Colidiu com o canto direito do cenário
*/
int colide(Ball& ball, Player& player, Player& bot, bool& colidiuAntes) {
	
	int movimentoBolaX = (ball.x + (velocidadeBase / 4) * aceleracao);
	int movimentoBolaY = (ball.y + (velocidadeBase * aceleracao));

	//Colisão com o jogador
	if (player.x <= ball.x + ball.width &&
		player.x + player.width >= ball.x &&
		player.y <= ball.y + ball.height &&
		player.y + player.height >= ball.y) {

		if (ball.x - bot.x > (player.x + player.width) - ball.x) {
			ball.xDir = 0.8;
		}
		else if (ball.x - player.x < (player.x + player.width) - ball.x) {
			ball.xDir = -0.8;
		}
		else
			ball.xDir = 0;

		return 1;
	}

	//Colisão com o bot
	
	if (bot.x <= ball.x + ball.width &&
		bot.x + bot.width >= ball.x &&
		bot.y <= ball.y + ball.height &&
		bot.y + bot.height >= ball.y) {

		if (ball.x - bot.x > (bot.x + bot.width) - ball.x) {
			ball.xDir = 0.8;
		}
		else if (ball.x - bot.x < (bot.x + bot.width) - ball.x) {
			ball.xDir = -0.8;
		}
		else
			ball.xDir = 0;
		return 2;
	}

	//Colisão com os cantos

	if (ball.x + ball.width >= (7 * widthTenis) / 8 - 30) {
		return 5;
	}

	if (ball.x + ball.width <= (widthTenis / 8) + 30) {
		return 4;
	}
	if (ball.y <= 0 || ball.y >= height) {
		if (ball.y <= 0) {
			if (++pontosPlayer == 3) {
				zeraPontuacao();
				setsPlayer++;
			}
		}
		else if (ball.y >= height) {
			if (++pontosBot == 3) {
				zeraPontuacao();
				setsBot++;
			}
		}
		aceleracao = 1.1;
		ball.x = widthTenis / 2;
		ball.y = heightTenis / 2;
		return 3;
	}

	return 0;
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

int Collision(Obstaculo obstaculo[], Jogador& jogador) {
	for (int i = 0; i < numeroObstaculos; i++) {
		if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CONE) {
			if (obstaculo[i].x1 < jogador.x1 + 50 &&
				obstaculo[i].x1 + 50 > jogador.x1 &&
				obstaculo[i].y1 < jogador.y2 + 75 &&
				obstaculo[i].y1 + 50 > jogador.y2 &&
				(obstaculo[i].y2 > 350 && obstaculo[i].y1 < 450)) {
				jogador.status = MORTO;
				return 1;
			}
		}
		else if (obstaculo[i].status == VIVO && obstaculo[i].tipo == CORRIMAO) {
			if (obstaculo[i].x1 < jogador.x1 + 50 &&
				obstaculo[i].x1 + 50 > jogador.x1 &&
				obstaculo[i].y1 < jogador.y2 + 75 &&
				obstaculo[i].y1 + 25 > jogador.y2 &&
				(obstaculo[i].y2 > 448 && obstaculo[i].y1 < 450)) {
				jogador.status = MORTO;
				return 1;
			}
		}
	}
	return 0;
}


const char* combat(int acaoP, int acaoB, int& PL, int& BL) {
	if (acaoP == 1 && acaoB == 1) {
		return "os golpes se encontram, mas não machucam nenhum dos lutadores!\n";
	}
	else if (acaoP == 1 && acaoB == 2) {
		PL = PL - 1;
		return "o jogador 1 levou um combo em cheio!\n";
	}
	else if (acaoP == 1 && acaoB == 3) {
		BL = BL - 1;
		return "o jogador 2 levou um jeb bem no nariz! Deve ter machucado\n";
	}
	else if (acaoP == 2 && acaoB == 1) {
		BL = BL - 1;
		return "o jogador 2 levou um combo em cheio!\n";
	}
	else if (acaoP == 2 && acaoB == 2) {
		return "os golpes se encontram, mas não machucam nenhum dos lutadores!\n";
	}
	else if (acaoP == 2 && acaoB == 3) {
		PL = PL - 1;
		return "o jogador 2 da um chute no estomago do jogador 1\n";
	}
	else if (acaoP == 3 && acaoB == 1) {
		PL = PL - 1;
		return "o jogador 1 levou um jeb bem no nariz! Deve ter machucado\n";
	}
	else if (acaoP == 3 && acaoB == 2) {
		BL = BL - 1;
		return "o jogador 1 da um chute no estomago do jogador 2\n";
	}
	else if (acaoP == 3 && acaoB == 3) {
		return "os golpes se encontram, mas não machucam nenhum dos lutadores!\n";
	}
	return "error";
};

int game(int PL, int BL) {
	printf("\n \n-- BOXE BIRIGUI --  \n Vida P1: %d \n Vida P2: %d", PL, BL);
	printf("\nFaça uma ação\n 1 - Jeb\n 2 - gancho\n 3 - chute\n ação: ");
	scanf_s("%d", &acaoP);
	acaoB = rand() % 3 + 1;
	printf("o jogador 2 usou a acao %d\n \n", acaoB);
	printf("%s", combat(acaoP, acaoB, PL, BL));
	return 0;
}

void personagemMoveCima(Personagem& personagem) {
	personagem.curFrame = 1;

	personagem.posY -= personagem.velocidade;

	if (personagem.posY < heightSurf / 3)
		personagem.posY = heightSurf / 3;
}
void personagemMoveBaixo(Personagem& personagem) {
	personagem.curFrame = 8;

	personagem.posY += personagem.velocidade;

	if (personagem.posY + 30 > heightSurf)
		personagem.posY = heightSurf - 30;
}
void personagemMoveEsquerda(Personagem& personagem) {
	personagem.curFrame = 1;

	personagem.posX -= personagem.velocidade;

	if (personagem.posX < 0)
		personagem.posX = 0;
}
void personagemMoveDireira(Personagem& personagem) {
	personagem.curFrame = 0;

	personagem.posX += personagem.velocidade;

	if (personagem.posX + 30 > widthSurf)
		personagem.posX = widthSurf - 30;
}

void moveObstaculos(Obstaculos obstaculos[], int quantidade) {
	for (int i = 0; i < quantidade; i++) {
		if (obstaculos[i].ativo) {
			obstaculos[i].posX -= obstaculos[i].velocidade;

			if (obstaculos[i].posX < 0)
				obstaculos[i].ativo = false;
		}
	}
}

// Colis�o
void colidirObstaculos(Obstaculos obstaculos[], int quantidade, Personagem& personagem, ALLEGRO_SAMPLE_INSTANCE* insMedalhas) {
	for (int i = 0; i < quantidade; i++) {
		if (obstaculos[i].ativo) {
			if (personagem.posX + personagem.bordaX > obstaculos[i].posX - obstaculos[i].bordaX &&
				personagem.posY + personagem.bordaY > obstaculos[i].posY - obstaculos[i].bordaY &&
				personagem.posX < obstaculos[i].posX + obstaculos[i].bordaX &&
				personagem.posY < obstaculos[i].posY + obstaculos[i].bordaY) {

				al_play_sample_instance(insMedalhas);
				obstaculos[i].ativo = false;
				personagem.vida--;
			}
		}
	}
}