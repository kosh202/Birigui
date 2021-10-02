enum IDS
{
	JOGADOR,
	OBSTACULOS
};

struct Jogador
{
	int ID;
	int x;
	int y;
	int velocidade;
	int boundx;
	int boundy;

	int vidas;
};