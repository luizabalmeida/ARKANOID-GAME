#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "game_structs.h"

#define NOME_JOGO "Projeto Arkanoid em C"

#define LINHAS_BLOCOS 8
#define COLUNAS_BLOCOS 10

#define RAIO_BOLA 25.0f
#define VELOCIDADE_INICIAL_BOLA 10.0f
#define VELOCIDADE_RAQUETE 15.0f

#define MAX_PONTUACOES 5
#define TAM_MAX_NOME 9
#define MAX_SKINS 23

#ifndef POWERUP_CONTROLES_INVERTIDOS
#define POWERUP_CONTROLES_INVERTIDOS 4
#endif
#ifndef POWERUP_RAQUETE_ARMA
#define POWERUP_RAQUETE_ARMA 5
#endif
#ifndef POWERUP_MULTI_BOLA
#define POWERUP_MULTI_BOLA 6
#endif

const float VELOCIDADE_PARTICULA_MIN = 1.0f;
const float VELOCIDADE_PARTICULA_MAX = 3.0f;
const float COMPRIMENTO_PARTICULA_MIN = 5.0f;
const float COMPRIMENTO_PARTICULA_MAX = 15.0f;

int largura_tela = 800;
int altura_tela = 450;

Block blocos_jogo[LINHAS_BLOCOS][COLUNAS_BLOCOS];
Paddle raquete_jogador;
BallNode *cabeca_bola = NULL;
PowerUpNode *cabeca_powerup = NULL;
Bullet *cabeca_projetil = NULL;

int pontuacao_atual = 0;
int contagem_blocos_ativos = 0; 

int estado_jogo = 4;
HighScore melhores_pontuacoes[MAX_PONTUACOES];

char nome_jogador[TAM_MAX_NOME + 1] = "JOGADOR";
int contagem_letras = 7;
bool esta_digitando = false;

Texture2D texturas_fundo[MAX_SKINS];
Texture2D texturas_blocos[MAX_SKINS][3]; 
Texture2D texturas_powerup[7];
SkinManager skins_bola;
FallingParticle particulas[NUM_PARTICLES];

Color PALETAS_CORES[MAX_SKINS][3]; 

const char *NOMES_ARQUIVOS_SKIN[MAX_SKINS] = {
    "gelo", "terra", "trovao", "vidente", "diamante", "xadrez", "camuflada", "mel", 
    "pixelada", "labirinto", "morango", "chocolate", "arco-iris", "donut", "pizza", 
    "hamburguer", "sorvete", "sushi", "panda", "cachorro", "coelho", "raposa", "melancia"
};

const char *NOMES_EXIBICAO_SKIN[MAX_SKINS] = {
    "GELO", "TERRA", "TROVÃO", "VIDENTE", "DIAMANTE", "XADREZ", "CAMUFLADA", "MEL", 
    "PIXELADA", "LABIRINTO", "MORANGO", "CHOCOLATE", "ARCO-ÍRIS", "DONUT", "PIZZA", 
    "HAMBÚRGUER", "SORVETE", "SUSHI", "PANDA", "CACHORRO", "COELHO", "RAPOSA", "MELANCIA"
};

int LAYOUTS_NIVEIS[3][LINHAS_BLOCOS][COLUNAS_BLOCOS] = {
    { 
        {3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3},
        {2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    },
    { 
        {3,0,2,0,1,1,0,2,0,3},
        {3,0,2,0,1,1,0,2,0,3},
        {3,0,2,0,1,1,0,2,0,3},
        {3,0,2,0,1,1,0,2,0,3},
        {3,0,2,0,1,1,0,2,0,3},
        {3,0,2,0,1,1,0,2,0,3},
        {3,3,2,2,1,1,2,2,3,3},
        {0,0,0,0,0,0,0,0,0,0}
    },
    {
        {3,3,3,3,3,3,3,3,3,3},
        {3,0,0,0,0,0,0,0,0,3},
        {3,0,2,2,2,2,2,2,0,3},
        {3,0,2,0,0,0,0,2,0,3},
        {3,0,2,0,1,1,0,2,0,3},
        {3,0,2,2,2,2,2,2,0,3},
        {3,0,0,0,0,0,0,0,0,3},
        {3,3,3,3,3,3,3,3,3,3}
    }
};

void InicializarCoresSkins() {
    for (int i = 0; i < MAX_SKINS; i++) {
        PALETAS_CORES[i][0] = GREEN;
        PALETAS_CORES[i][1] = ORANGE;
        PALETAS_CORES[i][2] = RED;
    }

    PALETAS_CORES[0][0] = RAYWHITE;
    PALETAS_CORES[0][1] = SKYBLUE;
    PALETAS_CORES[0][2] = DARKBLUE;
    
    PALETAS_CORES[1][0] = BEIGE;
    PALETAS_CORES[1][1] = BROWN;
    PALETAS_CORES[1][2] = DARKBROWN;

    PALETAS_CORES[2][0] = LIGHTGRAY;
    PALETAS_CORES[2][1] = YELLOW;
    PALETAS_CORES[2][2] = DARKPURPLE;

    PALETAS_CORES[3][0] = MAGENTA;
    PALETAS_CORES[3][1] = PURPLE;
    PALETAS_CORES[3][2] = DARKPURPLE;

    PALETAS_CORES[4][0] = SKYBLUE;
    PALETAS_CORES[4][1] = BLUE;
    PALETAS_CORES[4][2] = DARKBLUE;

    PALETAS_CORES[5][0] = LIGHTGRAY;
    PALETAS_CORES[5][1] = DARKGRAY;
    PALETAS_CORES[5][2] = BLACK;

    PALETAS_CORES[6][0] = LIME;
    PALETAS_CORES[6][1] = DARKGREEN;
    PALETAS_CORES[6][2] = BROWN;

    PALETAS_CORES[7][0] = YELLOW;
    PALETAS_CORES[7][1] = GOLD;
    PALETAS_CORES[7][2] = ORANGE;

    PALETAS_CORES[8][0] = WHITE;
    PALETAS_CORES[8][1] = LIGHTGRAY;
    PALETAS_CORES[8][2] = GRAY;

    PALETAS_CORES[9][0] = WHITE;
    PALETAS_CORES[9][1] = GREEN;
    PALETAS_CORES[9][2] = BLACK;

    PALETAS_CORES[10][0] = GREEN;
    PALETAS_CORES[10][1] = RED;
    PALETAS_CORES[10][2] = BLACK;

    PALETAS_CORES[11][0] = (Color){ 122, 81, 55, 255 };
    PALETAS_CORES[11][1] = (Color){ 99, 65, 43, 255 };
    PALETAS_CORES[11][2] = (Color){ 64, 40, 24 , 255 };

    PALETAS_CORES[12][0] = (Color){ 212, 188, 32, 255 };
    PALETAS_CORES[12][1] = (Color){110, 212, 32, 255 };
    PALETAS_CORES[12][2] = (Color){227, 56, 14, 255  };

    PALETAS_CORES[13][0] = (Color){183, 201, 44, 255  };
    PALETAS_CORES[13][1] = (Color){201, 44, 123, 255 };
    PALETAS_CORES[13][2] = (Color){158, 105, 74, 255  };

    PALETAS_CORES[14][0] = (Color){245, 250, 147, 255 };
    PALETAS_CORES[14][1] = (Color){238, 247, 57, 255 };
    PALETAS_CORES[14][2] = (Color){227, 58, 11, 255 };

    PALETAS_CORES[15][0] = (Color){214, 178, 99, 255 };
    PALETAS_CORES[15][1] = (Color){242, 39, 39, 255 };
    PALETAS_CORES[15][2] = (Color){89, 71, 29, 255 };

    PALETAS_CORES[16][0] = (Color){230, 207, 126, 255 };
    PALETAS_CORES[16][1] = (Color){247, 143, 190, 255 };
    PALETAS_CORES[16][2] = (Color){82, 65, 35, 255 };

    PALETAS_CORES[17][0] = (Color){252, 244, 230, 255 };
    PALETAS_CORES[17][1] = (Color){252, 162, 83, 255 };
    PALETAS_CORES[17][2] = (Color){3, 36, 7, 255 };

    PALETAS_CORES[18][0] = (Color){242, 247, 243, 255 };
    PALETAS_CORES[18][1] = (Color){10, 10, 10, 255 };
    PALETAS_CORES[18][2] = (Color){105, 153, 96, 255 };

    PALETAS_CORES[19][0] = (Color){153, 151, 96, 255 };
    PALETAS_CORES[19][1] = (Color){64, 55, 36, 255 };
    PALETAS_CORES[19][2] = (Color){8, 8, 8, 255};

    PALETAS_CORES[20][0] = (Color){252, 252, 252, 255 };
    PALETAS_CORES[20][1] = (Color){89, 87, 87, 255 };
    PALETAS_CORES[20][2] = (Color){10, 10, 10, 255};

    PALETAS_CORES[21][0] = (Color){252, 163, 98, 255 };
    PALETAS_CORES[21][1] = (Color){247, 136, 54, 255 };
    PALETAS_CORES[21][2] = (Color){247, 118, 22, 255 };

    PALETAS_CORES[22][0] = (Color){245, 53, 39, 255 };
    PALETAS_CORES[22][1] = (Color){59, 122, 47, 255 };
    PALETAS_CORES[22][2] = (Color){10, 10, 10, 255 };
}

void InicializarParticulas() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particulas[i].position.x = GetRandomValue(0, largura_tela);
        particulas[i].position.y = GetRandomValue(0, altura_tela);
        particulas[i].speed = GetRandomValue(VELOCIDADE_PARTICULA_MIN * 100, VELOCIDADE_PARTICULA_MAX * 100) / 100.0f;
        particulas[i].length = GetRandomValue(COMPRIMENTO_PARTICULA_MIN * 10, COMPRIMENTO_PARTICULA_MAX * 10) / 10.0f;
        particulas[i].color = WHITE;
    }
}

void AtualizarParticulas() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particulas[i].position.y += particulas[i].speed;
        if (particulas[i].position.y > altura_tela) {
            particulas[i].position.y = -particulas[i].length;
            particulas[i].position.x = GetRandomValue(0, largura_tela);
            particulas[i].speed = GetRandomValue(VELOCIDADE_PARTICULA_MIN * 100, VELOCIDADE_PARTICULA_MAX * 100) / 100.0f;
            particulas[i].length = GetRandomValue(COMPRIMENTO_PARTICULA_MIN * 10, COMPRIMENTO_PARTICULA_MAX * 10) / 10.0f;
        }
    }
}

void DesenharParticulas() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        DrawLine(particulas[i].position.x, particulas[i].position.y,
                 particulas[i].position.x, particulas[i].position.y + particulas[i].length,
                 particulas[i].color);
    }
}

void CriarBola(Vector2 posicao, Vector2 velocidade) {
    BallNode *novaBola = (BallNode *)malloc(sizeof(BallNode));
    if (novaBola == NULL) return;
    
    novaBola->position = posicao;
    novaBola->velocity = velocidade;
    novaBola->radius = RAIO_BOLA;

    for (int i = 0; i < TRAIL_LENGTH; i++) {
        novaBola->previous_positions[i] = posicao;
    }
    
    novaBola->next = cabeca_bola;
    cabeca_bola = novaBola;
}

void LimparBolas() {
    BallNode *atual = cabeca_bola;
    while (atual != NULL) {
        BallNode *proximo = atual->next;
        free(atual);
        atual = proximo;
    }
    cabeca_bola = NULL;
}

void InicializarBlocos(int indiceNivel)
{
    float largura_bloco = (float)largura_tela / COLUNAS_BLOCOS;
    float altura_bloco = altura_tela * 0.05f;
    float topo_offset = altura_tela * 0.05f;

    contagem_blocos_ativos = 0; 

    for (int i = 0; i < LINHAS_BLOCOS; i++)
    {
        for (int j = 0; j < COLUNAS_BLOCOS; j++)
        {
            blocos_jogo[i][j].rect = (Rectangle){ j * largura_bloco, topo_offset + i * altura_bloco, largura_bloco, altura_bloco };
            int tipo = LAYOUTS_NIVEIS[indiceNivel][i][j];

            if (tipo > 0) 
            {
                blocos_jogo[i][j].is_active = 1;
                blocos_jogo[i][j].type = 1;
                blocos_jogo[i][j].life = tipo; 
                contagem_blocos_ativos++; 
            }
            else 
            {
                blocos_jogo[i][j].is_active = 0;
            }
        }
    }
}

void Atirar()
{
    if (GetTime() - raquete_jogador.time_last_shot < 0.25f) return;
    
    Bullet *novoProjetil = (Bullet *)malloc(sizeof(Bullet));
    if (novoProjetil == NULL) return;

    novoProjetil->rect = (Rectangle){ 
        raquete_jogador.rect.x + raquete_jogador.rect.width / 2 - 3,
        raquete_jogador.rect.y, 
        6,
        15
    };
    novoProjetil->speed = 12.0f;
    novoProjetil->is_active = true;
    novoProjetil->next = cabeca_projetil;
    cabeca_projetil = novoProjetil;
    
    raquete_jogador.time_last_shot = GetTime();
}

void LimparPowerUps()
{
    PowerUpNode *atual = cabeca_powerup;
    PowerUpNode *proximo;
    while (atual != NULL) {
        proximo = atual->next;
        free(atual);
        atual = proximo;
    }
    cabeca_powerup = NULL;
}

void InicializarJogo()
{
    raquete_jogador.rect.width = largura_tela * 0.125f;
    raquete_jogador.rect.height = altura_tela * 0.035f;
    raquete_jogador.rect.x = (largura_tela / 2) - (raquete_jogador.rect.width / 2);
    raquete_jogador.rect.y = altura_tela - raquete_jogador.rect.height * 2.0f;
    raquete_jogador.speed = VELOCIDADE_RAQUETE;

    raquete_jogador.controls_reversed = false;
    raquete_jogador.time_effect_started = 0.0;
    
    raquete_jogador.has_gun = false;
    raquete_jogador.time_last_shot = 0.0;

    Bullet *atual = cabeca_projetil;
    while (atual != NULL) {
        Bullet *proximo = atual->next;
        free(atual);
        atual = proximo;
    }
    cabeca_projetil = NULL;
    
    LimparPowerUps();
    LimparBolas(); 

    Vector2 posInicial = { largura_tela / 2.0f, raquete_jogador.rect.y - RAIO_BOLA };
    Vector2 velInicial = { VELOCIDADE_INICIAL_BOLA, -VELOCIDADE_INICIAL_BOLA };
    CriarBola(posInicial, velInicial);
    
    pontuacao_atual = 0;
    int nivelAleatorio = GetRandomValue(0, 2);
    InicializarBlocos(nivelAleatorio); 
}

void CarregarPontuacoes() {
    FILE *arquivo = fopen("scores.dat", "rb");
    if (arquivo == NULL) {
        for (int i = 0; i < MAX_PONTUACOES; i++) {
            strcpy(melhores_pontuacoes[i].name, "---");
            melhores_pontuacoes[i].score = 0;
        }
        return;
    }
    fread(melhores_pontuacoes, sizeof(HighScore), MAX_PONTUACOES, arquivo);
    fclose(arquivo);
}

void SalvarPontuacoes(int nova_pontuacao) {
    if (nova_pontuacao <= melhores_pontuacoes[MAX_PONTUACOES - 1].score) return;
    
    HighScore nova_entrada;
    nova_entrada.score = nova_pontuacao;
    strcpy(nova_entrada.name, nome_jogador); 

    for (int i = 0; i < MAX_PONTUACOES; i++) {
        if (nova_pontuacao > melhores_pontuacoes[i].score) {
            for (int j = MAX_PONTUACOES - 1; j > i; j--) {
                melhores_pontuacoes[j] = melhores_pontuacoes[j - 1];
            }
            melhores_pontuacoes[i] = nova_entrada;
            break;
        }
    }

    FILE *arquivo = fopen("scores.dat", "wb");
    if (arquivo != NULL) {
        fwrite(melhores_pontuacoes, sizeof(HighScore), MAX_PONTUACOES, arquivo);
        fclose(arquivo);
    }
}

void AdicionarPowerUp(float x, float y, int tipo)
{
    PowerUpNode *novoNo = (PowerUpNode *)malloc(sizeof(PowerUpNode));
    if (novoNo == NULL) return;
    
    novoNo->rect = (Rectangle){ x + 10, y + 5, largura_tela * 0.04f, largura_tela * 0.04f };
    novoNo->type = tipo;
    novoNo->speed = 2.0f;
    novoNo->next = cabeca_powerup;
    cabeca_powerup = novoNo;
}

void AplicarPowerUp(int tipo)
{
    switch (tipo) {
        case 1: 
            raquete_jogador.rect.width = largura_tela * 0.20f;
            break;
        case 2: 
            raquete_jogador.rect.width = largura_tela * 0.08f;
            break;
        case 3:
        {
            BallNode *bola = cabeca_bola;
            while (bola != NULL) {
                float vel_atual_x = (bola->velocity.x > 0) ? 1.0f : -1.0f;
                float vel_atual_y = (bola->velocity.y > 0) ? 1.0f : -1.0f;
                bola->velocity.x = (VELOCIDADE_INICIAL_BOLA * 0.6f) * vel_atual_x;
                bola->velocity.y = (VELOCIDADE_INICIAL_BOLA * 0.6f) * vel_atual_y;
                bola = bola->next;
            }
            break;
        }
        case POWERUP_CONTROLES_INVERTIDOS:
            raquete_jogador.controls_reversed = true;
            raquete_jogador.time_effect_started = GetTime(); 
            break;
        case POWERUP_RAQUETE_ARMA:
            raquete_jogador.has_gun = true;
            break;
        case POWERUP_MULTI_BOLA:
        {
            if (cabeca_bola != NULL) {
                Vector2 posBase = cabeca_bola->position;
                Vector2 velBase = cabeca_bola->velocity;
                CriarBola(posBase, (Vector2){ -velBase.x, velBase.y });
            }
            break;
        }

        default: break;
    }
}

void VerificarColisaoBolaBloco(BallNode *bola) 
{
    for (int i = 0; i < LINHAS_BLOCOS; i++)
    {
        for (int j = 0; j < COLUNAS_BLOCOS; j++)
        {
            if (blocos_jogo[i][j].is_active) 
            {
                if (CheckCollisionCircleRec(bola->position, bola->radius, blocos_jogo[i][j].rect))
                {
                    bola->position.x -= bola->velocity.x;
                    bola->position.y -= bola->velocity.y;
                    
                    bola->velocity.y *= -1.0f;
                    
                    blocos_jogo[i][j].life--;

                    if (blocos_jogo[i][j].life > 0) 
                    {
                        pontuacao_atual += 5; 
                    }
                    else 
                    {
                        blocos_jogo[i][j].is_active = 0; 
                        contagem_blocos_ativos--; 
                        pontuacao_atual += 10;
                        
                        if (contagem_blocos_ativos <= 0) 
                        {
                            int nivelAleatorio = GetRandomValue(0, 2); 
                            InicializarBlocos(nivelAleatorio); 
                            
                            LimparBolas();
                            Vector2 posInicial = { largura_tela / 2.0f, raquete_jogador.rect.y - RAIO_BOLA };
                            Vector2 velInicial = { VELOCIDADE_INICIAL_BOLA, -VELOCIDADE_INICIAL_BOLA };
                            CriarBola(posInicial, velInicial);
                            
                            LimparPowerUps();
                        }

                        if (GetRandomValue(1, 10) <= 5) { 
                            int tipo_power = GetRandomValue(1, POWERUP_MULTI_BOLA); 
                            AdicionarPowerUp(blocos_jogo[i][j].rect.x, blocos_jogo[i][j].rect.y, tipo_power);
                        }
                    }
                    return; 
                }
            }
        }
    }
}

void AtualizarJogo()
{
    if (raquete_jogador.controls_reversed)
    {
        const float DURACAO_EFEITO = 5.0f; 
        if (GetTime() - raquete_jogador.time_effect_started >= DURACAO_EFEITO)
        {
            raquete_jogador.controls_reversed = false;
        }
    }

    if (raquete_jogador.controls_reversed)
    {
        if (IsKeyDown(KEY_LEFT)) raquete_jogador.rect.x += raquete_jogador.speed; 
        if (IsKeyDown(KEY_RIGHT)) raquete_jogador.rect.x -= raquete_jogador.speed; 
    }
    else 
    {
        if (IsKeyDown(KEY_LEFT)) raquete_jogador.rect.x -= raquete_jogador.speed;
        if (IsKeyDown(KEY_RIGHT)) raquete_jogador.rect.x += raquete_jogador.speed;
    }

    if (raquete_jogador.rect.x <= 0) raquete_jogador.rect.x = 0;
    if (raquete_jogador.rect.x + raquete_jogador.rect.width >= largura_tela) 
        raquete_jogador.rect.x = largura_tela - raquete_jogador.rect.width;

    if (raquete_jogador.has_gun && IsKeyDown(KEY_SPACE))
    {
        Atirar();
    }

    BallNode *bola_atual = cabeca_bola;

    while (bola_atual != NULL)
    {
        for (int i = TRAIL_LENGTH - 1; i > 0; i--) {
            bola_atual->previous_positions[i] = bola_atual->previous_positions[i - 1];
        }
        bola_atual->previous_positions[0] = bola_atual->position;

        bola_atual->position.x += bola_atual->velocity.x;
        bola_atual->position.y += bola_atual->velocity.y;

        if ((bola_atual->position.x + bola_atual->radius >= largura_tela) || (bola_atual->position.x - bola_atual->radius <= 0))
            bola_atual->velocity.x *= -1.0f;
        if (bola_atual->position.y - bola_atual->radius <= 0)
            bola_atual->velocity.y *= -1.0f;
        
        if (bola_atual->position.y + bola_atual->radius >= altura_tela)
        {
            estado_jogo = 2;
            SalvarPontuacoes(pontuacao_atual);
            return; 
        }
        
        if (CheckCollisionCircleRec(bola_atual->position, bola_atual->radius, raquete_jogador.rect) && bola_atual->velocity.y > 0)
        {
            bola_atual->velocity.y *= -1.0f;
            float ponto_impacto = bola_atual->position.x - raquete_jogador.rect.x;
            float impacto_relativo = ponto_impacto / raquete_jogador.rect.width;
            float direcao = (impacto_relativo * 2.0f - 1.0f);
            float velocidade = (fabsf(bola_atual->velocity.x) > fabsf(bola_atual->velocity.y)) ? fabsf(bola_atual->velocity.x) : fabsf(bola_atual->velocity.y);
            
            bola_atual->velocity.x = direcao * velocidade;
            bola_atual->velocity.y = -velocidade;
        }

        VerificarColisaoBolaBloco(bola_atual);

        bola_atual = bola_atual->next;
    }

    Bullet *projetil_atual = cabeca_projetil;
    Bullet *projetil_anterior = NULL;

    while (projetil_atual != NULL)
    {
        projetil_atual->rect.y -= projetil_atual->speed;
        bool destruido = false;

        for (int i = 0; i < LINHAS_BLOCOS; i++) {
            for (int j = 0; j < COLUNAS_BLOCOS; j++) {
                if (blocos_jogo[i][j].is_active && CheckCollisionRecs(projetil_atual->rect, blocos_jogo[i][j].rect)) {
                    blocos_jogo[i][j].life--;
                    if (blocos_jogo[i][j].life <= 0) {
                        blocos_jogo[i][j].is_active = 0;
                        contagem_blocos_ativos--;
                        pontuacao_atual += 10;
                        if (contagem_blocos_ativos <= 0) {
                             int nivelAleatorio = GetRandomValue(0, 2); 
                             InicializarBlocos(nivelAleatorio); 
                             
                             LimparBolas();
                             Vector2 posInicial = { largura_tela / 2.0f, raquete_jogador.rect.y - RAIO_BOLA };
                             Vector2 velInicial = { VELOCIDADE_INICIAL_BOLA, -VELOCIDADE_INICIAL_BOLA };
                             CriarBola(posInicial, velInicial);

                             LimparPowerUps();
                        }
                    }
                    pontuacao_atual += 5;
                    destruido = true;
                    break; 
                }
            }
            if (destruido) break;
        }

        if (destruido || projetil_atual->rect.y < 0) {
            if (projetil_anterior == NULL) cabeca_projetil = projetil_atual->next;
            else projetil_anterior->next = projetil_atual->next;
            Bullet *temp = projetil_atual;
            projetil_atual = projetil_atual->next;
            free(temp);
            continue;
        }
        projetil_anterior = projetil_atual;
        projetil_atual = projetil_atual->next;
    }

    PowerUpNode *atual = cabeca_powerup;
    PowerUpNode *anterior = NULL;

    while (atual != NULL)
    {
        atual->rect.y += atual->speed;
        
        if (CheckCollisionRecs(atual->rect, raquete_jogador.rect))
        {
            AplicarPowerUp(atual->type);
            
            if (anterior == NULL) cabeca_powerup = atual->next;
            else anterior->next = atual->next;
            PowerUpNode *temp = atual;
            atual = atual->next;
            free(temp);
            continue;
        }
        
        if (atual->rect.y > altura_tela)
        {
            if (anterior == NULL) cabeca_powerup = atual->next;
            else anterior->next = atual->next;
            PowerUpNode *temp = atual;
            atual = atual->next;
            free(temp);
            continue;
        }

        anterior = atual;
        atual = atual->next;
    }
}

void DesenharSeletorSkin()
{
    float tam_fonte_titulo = altura_tela * 0.08f;
    float tam_fonte_texto = altura_tela * 0.04f;
    
    DrawRectangle(0, 0, largura_tela, altura_tela, Fade(BLACK, 0.9f));
    
    DrawText("SELECIONE SUA BOLA", largura_tela/2 - MeasureText("SELECIONE SUA BOLA", tam_fonte_titulo)/2, altura_tela/6, tam_fonte_titulo, YELLOW);
             
    const char *nome_skin = NOMES_EXIBICAO_SKIN[skins_bola.current_skin_index];
    DrawText(nome_skin, largura_tela/2 - MeasureText(nome_skin, tam_fonte_texto * 1.5f)/2, altura_tela * 0.28f, tam_fonte_texto * 1.5f, WHITE);
             
    Texture2D textura_atual = skins_bola.textures[skins_bola.current_skin_index];
    
    float tam_preview = altura_tela * 0.20f;
    Rectangle rectDestino = { largura_tela/2 - tam_preview/2, altura_tela/2 - tam_preview/2, tam_preview, tam_preview };
    Rectangle rectOrigem = { 0.0f, 0.0f, (float)textura_atual.width, (float)textura_atual.height };

    DrawTexturePro(textura_atual, rectOrigem, rectDestino, (Vector2){0,0}, 0.0f, WHITE);
    
    DrawText("<", largura_tela/2 - tam_preview/2 - 50, altura_tela/2 - 20, tam_fonte_titulo, LIME);
    DrawText(">", largura_tela/2 + tam_preview/2 + 20, altura_tela/2 - 20, tam_fonte_titulo, LIME);
    
    DrawText("Use SETAS ESQUERDA/DIREITA | ENTER para Continuar", largura_tela/2 - MeasureText("Use SETAS ESQUERDA/DIREITA | ENTER para Continuar", tam_fonte_texto)/2, altura_tela * 0.85f, tam_fonte_texto, RAYWHITE);

    if (IsKeyPressed(KEY_RIGHT)) skins_bola.current_skin_index = (skins_bola.current_skin_index + 1) % MAX_SKINS;
    if (IsKeyPressed(KEY_LEFT)) skins_bola.current_skin_index = (skins_bola.current_skin_index - 1 + MAX_SKINS) % MAX_SKINS;
    if (IsKeyPressed(KEY_ENTER)) estado_jogo = 3; 
}


void DesenharJogo()
{
    if (estado_jogo != 1) AtualizarParticulas();
    
    BeginDrawing();
    
        ClearBackground(BLACK);
        if (estado_jogo != 1) DesenharParticulas();
        
        Texture2D bg = texturas_fundo[skins_bola.current_skin_index];
        if (bg.id > 0 && estado_jogo == 1) {
            DrawTexturePro(bg, (Rectangle){0, 0, (float)bg.width, (float)bg.height}, (Rectangle){0, 0, (float)largura_tela, (float)altura_tela}, (Vector2){0,0}, 0.0f, Fade(WHITE, 0.4f));
        } else if (estado_jogo == 1) {
             DrawRectangleGradientV(0, 0, largura_tela, altura_tela, DARKBLUE, BLACK);
             DrawRectangle(0, 0, largura_tela, altura_tela, Fade(BLACK, 0.3f));
        }
        
        for (int i = 0; i < LINHAS_BLOCOS; i++)
        {
            for (int j = 0; j < COLUNAS_BLOCOS; j++)
            {
                if (blocos_jogo[i][j].is_active)
                {
                    Rectangle rect = blocos_jogo[i][j].rect;
                    int vida = blocos_jogo[i][j].life;

                    Texture2D texBloco = texturas_blocos[skins_bola.current_skin_index][vida - 1];
                    
                    if (texBloco.id > 0) {
                         DrawTexturePro(texBloco, (Rectangle){0, 0, (float)texBloco.width, (float)texBloco.height}, rect, (Vector2){0,0}, 0.0f, WHITE);
                    } else {
                        Color corBloco = PALETAS_CORES[skins_bola.current_skin_index][vida - 1];
                        
                        DrawRectangleRec(rect, corBloco);
                        DrawRectangle(rect.x, rect.y, rect.width, 4, Fade(WHITE, 0.5f));
                        DrawRectangle(rect.x, rect.y, 4, rect.height, Fade(WHITE, 0.5f));
                        DrawRectangle(rect.x, rect.y + rect.height - 4, rect.width, 4, Fade(BLACK, 0.4f));
                        DrawRectangle(rect.x + rect.width - 4, rect.y, 4, rect.height, Fade(BLACK, 0.4f));
                        DrawRectangleLinesEx(rect, 1, BLACK); 
                    }
                }
            }
        }

        Color cor_raquete = BLUE;
        if (raquete_jogador.controls_reversed) { cor_raquete = PURPLE; }
        if (raquete_jogador.has_gun) { cor_raquete = RED; }
        
        DrawRectangleRec(raquete_jogador.rect, cor_raquete);
        DrawRectangleLinesEx(raquete_jogador.rect, 2, SKYBLUE);
        
        if (raquete_jogador.has_gun) {
            const char *textoAtirar = "PRESSIONE ESPACO PARA ATIRAR!";
            int largTexto = MeasureText(textoAtirar, 20);
            DrawText(textoAtirar, raquete_jogador.rect.x + raquete_jogador.rect.width/2 - largTexto/2, raquete_jogador.rect.y + 30, 20, YELLOW);
        }

        if (estado_jogo != 4) 
        {
            Texture2D textura_atual = skins_bola.textures[skins_bola.current_skin_index];
            BallNode *bola = cabeca_bola;
            while (bola != NULL) {
                
                for (int i = 0; i < TRAIL_LENGTH; i++) {
                    float fator = 1.0f - ((float)i / TRAIL_LENGTH); 
                    float raioRastro = bola->radius * (fator * 0.8f); 
                    Color corRastro = Fade(WHITE, fator * 0.4f); 

                    Rectangle rectDestinoRastro = { 
                        bola->previous_positions[i].x - raioRastro, 
                        bola->previous_positions[i].y - raioRastro, 
                        raioRastro * 2, 
                        raioRastro * 2 
                    };
                    
                    Rectangle rectOrigemRastro = { 0.0f, 0.0f, (float)textura_atual.width, (float)textura_atual.height };
                    DrawTexturePro(textura_atual, rectOrigemRastro, rectDestinoRastro, (Vector2){0,0}, 0.0f, corRastro);
                }

                Rectangle rectDestino = { bola->position.x - bola->radius, bola->position.y - bola->radius, bola->radius * 2, bola->radius * 2 };
                Rectangle rectOrigem = { 0.0f, 0.0f, (float)textura_atual.width, (float)textura_atual.height };
                DrawTexturePro(textura_atual, rectOrigem, rectDestino, (Vector2){0,0}, 0.0f, WHITE);
                bola = bola->next;
            }
        }

        Bullet *b = cabeca_projetil;
        while(b != NULL) {
            DrawRectangleRec(b->rect, ORANGE);
            b = b->next;
        }

        PowerUpNode *atual = cabeca_powerup;
        while (atual != NULL)
        {
            if (texturas_powerup[atual->type].id > 0) {
                 Texture2D tex = texturas_powerup[atual->type];
                 DrawTexturePro(tex, (Rectangle){0,0,(float)tex.width,(float)tex.height}, atual->rect, (Vector2){0,0}, 0.0f, WHITE);
            } else {
                 Color pColor = YELLOW;
                 if (atual->type == 1) pColor = MAGENTA;
                 else if (atual->type == 2) pColor = LIME;
                 else if (atual->type == 3) pColor = YELLOW;
                 else if (atual->type == 4) pColor = PURPLE;
                 else if (atual->type == 5) pColor = RED;
                 else if (atual->type == 6) pColor = SKYBLUE;
                 DrawRectangleRec(atual->rect, pColor);
            }
            atual = atual->next;
        }
        
        DrawText(TextFormat("PONTUACAO: %d", pontuacao_atual), 10, 10, altura_tela * 0.04f, WHITE); 

        if (estado_jogo == 2)
        {
            float tam_fonte_grande = altura_tela * 0.08f;
            float tam_fonte_pequena = altura_tela * 0.04f;
            float tam_fonte_opcoes = altura_tela * 0.035f;

            DrawRectangle(0, 0, largura_tela, altura_tela, Fade(BLACK, 0.8f));
            DrawText("GAME OVER!", largura_tela/2 - MeasureText("GAME OVER!", tam_fonte_grande)/2, altura_tela/4, tam_fonte_grande, RED);
            
            DrawText("CONTINUAR (ENTER)", largura_tela/2 - MeasureText("CONTINUAR (ENTER)", tam_fonte_opcoes)/2, altura_tela/2 - tam_fonte_opcoes, tam_fonte_opcoes, WHITE);
            DrawText("VOLTAR AO MENU (ESPACO)", largura_tela/2 - MeasureText("VOLTAR AO MENU (ESPACO)", tam_fonte_opcoes)/2, altura_tela/2 + tam_fonte_opcoes, tam_fonte_opcoes, WHITE);
            
            float ranking_y_inicio = altura_tela/2 + tam_fonte_grande + tam_fonte_opcoes;
            DrawText("TOP 5 PONTUACOES:", largura_tela/2 - MeasureText("TOP 5 PONTUACOES:", tam_fonte_pequena)/2, ranking_y_inicio, tam_fonte_pequena, YELLOW);
            
            for (int i = 0; i < MAX_PONTUACOES; i++) {
                char buffer_texto[50];
                sprintf(buffer_texto, "%d. %s: %d", i + 1, melhores_pontuacoes[i].name, melhores_pontuacoes[i].score);
                DrawText(buffer_texto, largura_tela/2 - MeasureText(buffer_texto, tam_fonte_pequena)/2, ranking_y_inicio + tam_fonte_pequena + i * (tam_fonte_pequena * 1.2f), tam_fonte_pequena, WHITE);
            }
            
            if (IsKeyPressed(KEY_ENTER)) { 
                InicializarJogo(); 
                estado_jogo = 1; 
            }
            else if (IsKeyPressed(KEY_SPACE)) { 
                LimparPowerUps(); 
                estado_jogo = 4; 
            }
        }
        
        else if (estado_jogo == 3) 
        {
            float tam_fonte_titulo = altura_tela * 0.08f;
            float tam_fonte_pequena = altura_tela * 0.04f;
            char texto_instrucao[100];
            
            DrawRectangle(0, 0, largura_tela, altura_tela, Fade(BLACK, 0.9f));
            
            if (!esta_digitando) {
                sprintf(texto_instrucao, "JOGAR COMO: %s", nome_jogador);
                DrawText(texto_instrucao, largura_tela/2 - MeasureText(texto_instrucao, tam_fonte_titulo)/2, altura_tela/3, tam_fonte_titulo, GREEN);
                DrawText("ENTER para Manter | ESPACO para Mudar Nome", largura_tela/2 - MeasureText("ENTER para Manter | ESPACO para Mudar Nome", tam_fonte_pequena)/2, altura_tela/2, tam_fonte_pequena, WHITE);

                if (IsKeyPressed(KEY_ENTER)) { InicializarJogo(); estado_jogo = 1; } 
                else if (IsKeyPressed(KEY_SPACE)) { esta_digitando = true; contagem_letras = 0; nome_jogador[0] = '\0'; }

            } else {
                DrawText("INSIRA SEU NOME (MAX 9 LETRAS):", largura_tela/2 - MeasureText("INSIRA SEU NOME (MAX 9 LETRAS):", tam_fonte_pequena)/2, altura_tela/3, tam_fonte_pequena, WHITE);
                char nome_exibicao[TAM_MAX_NOME + 2];
                strcpy(nome_exibicao, nome_jogador);
                if ((GetTime() / 0.5f) < (int)(GetTime() / 0.5f)) nome_exibicao[contagem_letras] = '_';
                nome_exibicao[contagem_letras + 1] = '\0';
                DrawText(nome_exibicao, largura_tela/2 - MeasureText(nome_exibicao, tam_fonte_titulo)/2, altura_tela/2, tam_fonte_titulo, RAYWHITE);
                DrawText("ENTER para Confirmar", largura_tela/2 - MeasureText("ENTER para Confirmar", tam_fonte_pequena)/2, altura_tela/2 + tam_fonte_titulo, tam_fonte_pequena, YELLOW);

                int tecla = GetCharPressed();
                while (tecla > 0) {
                    if ((tecla >= 32) && (tecla <= 125) && (contagem_letras < TAM_MAX_NOME)) {
                        nome_jogador[contagem_letras] = (char)tecla; contagem_letras++; nome_jogador[contagem_letras] = '\0';
                    }
                    tecla = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    contagem_letras--; if (contagem_letras < 0) contagem_letras = 0; nome_jogador[contagem_letras] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    if (contagem_letras == 0) strcpy(nome_jogador, "JOGADOR"); 
                    esta_digitando = false; InicializarJogo(); estado_jogo = 1;
                }
            }
        }
        else if (estado_jogo == 4) 
        {
            DesenharSeletorSkin(); 
        }

    EndDrawing();
}

int main(void)
{
    int larguraMonitor = GetMonitorWidth(0);
    int alturaMonitor = GetMonitorHeight(0);
    
    InitWindow(larguraMonitor, alturaMonitor, NOME_JOGO);
    ToggleFullscreen(); 
    
    largura_tela = GetScreenWidth();
    altura_tela = GetScreenHeight();

    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    InicializarCoresSkins();
    CarregarPontuacoes();
    InicializarJogo();
    InicializarParticulas();

    for (int i = 0; i < MAX_SKINS; i++)
    {
        char caminho[100];
        sprintf(caminho, "skins/%s.png", NOMES_ARQUIVOS_SKIN[i]); 
        skins_bola.textures[i] = LoadTexture(caminho);

        char caminhoFundo[100];
        sprintf(caminhoFundo, "backgrounds/%s.png", NOMES_ARQUIVOS_SKIN[i]);
        texturas_fundo[i] = LoadTexture(caminhoFundo);

        for (int j = 0; j < 3; j++) {
            char caminhoBloco[100];
            sprintf(caminhoBloco, "blocks/%s_%d.png", NOMES_ARQUIVOS_SKIN[i], j + 1);
            texturas_blocos[i][j] = LoadTexture(caminhoBloco);
            if (texturas_blocos[i][j].id <= 0) {
                 sprintf(caminhoBloco, "blocks/%s_%d.jpeg", NOMES_ARQUIVOS_SKIN[i], j + 1);
                 texturas_blocos[i][j] = LoadTexture(caminhoBloco);
            }
        }
    }
    skins_bola.current_skin_index = 0;

    texturas_powerup[1] = LoadTexture("img/powerup_aumentar.png");
    texturas_powerup[2] = LoadTexture("img/powerup_diminuir.png");
    texturas_powerup[3] = LoadTexture("img/powerup_lento.png");
    texturas_powerup[4] = LoadTexture("img/powerup_inverter.png");
    texturas_powerup[5] = LoadTexture("img/powerup_arma.png");
    texturas_powerup[6] = LoadTexture("img/powerup_multp.png");
    
    while (!WindowShouldClose())
    {
        if (estado_jogo == 1) AtualizarJogo();
        else if (estado_jogo == 2 || estado_jogo == 3 || estado_jogo == 4) {} 
        
        DesenharJogo();
    }

    for (int i = 0; i < MAX_SKINS; i++) {
        UnloadTexture(skins_bola.textures[i]);
        UnloadTexture(texturas_fundo[i]);
        for (int j = 0; j < 3; j++) {
            UnloadTexture(texturas_blocos[i][j]);
        }
    }
    UnloadTexture(texturas_powerup[1]);
    UnloadTexture(texturas_powerup[2]);
    UnloadTexture(texturas_powerup[3]);
    UnloadTexture(texturas_powerup[4]);
    UnloadTexture(texturas_powerup[5]);
    UnloadTexture(texturas_powerup[6]);
    
    CloseWindow();
    return 0;
}