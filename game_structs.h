#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include "raylib.h"

// --- CONFIGURAÇÕES GERAIS ---
// Defines são constantes usadas para facilitar a manutenção do código.
#define MAX_SCORE_NAME 20       // Tamanho máximo do nome no placar
#define MAX_SKINS 23            // Quantidade total de skins disponíveis
#define NUM_PARTICLES 100       // Quantidade de partículas no fundo (efeito visual)
#define PARTICLE_LENGTH_MAX 15  // Tamanho máximo do rastro da partícula
#define TRAIL_LENGTH 10         // Tamanho do rastro visual da bola

// --- CÓDIGOS DOS POWER-UPS ---
// Identificadores para o 'switch case' na lógica de powerups.
#define POWERUP_REVERSE_CONTROLS 4
#define POWERUP_GUN_PADDLE 5
#define POWERUP_MULTI_BALL 6

// --- ESTRUTURAS (STRUCTS) ---

// Representa um tijolo na tela.
typedef struct {
    Rectangle rect; // Posição (x, y) e tamanho (width, height)
    int type;       // Tipo lógico (não usado muito aqui, mas útil para expansão)
    Color color;    // Cor do bloco (muda conforme a vida)
    int is_active;  // 1 = existe, 0 = quebrado (não desenha nem colide)
    int life;       // Vida do bloco (3 = vermelho, 2 = laranja, 1 = verde)
} Block;

// Representa a Bola.
// ATENÇÃO: Isso é um NÓ DE LISTA ENCADEADA.
// Usamos lista para permitir o power-up "Multi-Bola" (várias bolas simultâneas).
typedef struct BallNode {
    Vector2 position; // Posição atual (x, y)
    Vector2 velocity; // Velocidade vetorial (direção e rapidez)
    float radius;     // Raio para colisão circular
    Vector2 previous_positions[TRAIL_LENGTH]; // Vetor para desenhar o rastro
    struct BallNode *next; // PONTEIRO para a próxima bola da lista (ou NULL)
} BallNode;

// Representa a Raquete do jogador.
typedef struct {
    Rectangle rect; // Posição e tamanho da raquete
    float speed;    // Velocidade de movimento lateral
    
    // Estados temporários (Power-ups)
    bool controls_reversed;      // Se verdadeiro, esquerda vira direita
    double time_effect_started;  // Marca o tempo que o efeito começou

    bool has_gun;                // Se verdadeiro, pode atirar
    double time_gun_started;     // Tempo que pegou a arma
    double time_last_shot;       // Controle de cadência de tiro (cooldown)

    bool size_changed;           // Se verdadeiro, tamanho está alterado (grande ou pequeno)
    double time_size_changed;    // Tempo que mudou de tamanho
} Paddle;

// Representa um item de PowerUp caindo.
// TAMBÉM É UMA LISTA ENCADEADA (pode ter vários caindo ao mesmo tempo).
typedef struct PowerUp {
    Rectangle rect;          // Posição e tamanho do item caindo
    int type;                // Qual poder ele dá (1 a 6)
    float speed;             // Velocidade de queda
    struct PowerUp *next;    // Ponteiro para o próximo powerup caindo
} PowerUpNode;

// Estrutura simples para salvar no arquivo binário.
typedef struct {
    char name[MAX_SCORE_NAME]; // Nome do jogador
    int score;                 // Pontuação
} HighScore;

// Estrutura visual para o efeito de chuva no fundo.
typedef struct {
    Vector2 position;
    float speed;
    float length;
    Color color;
} FallingParticle;

// Gerencia as texturas carregadas para não carregar a mesma imagem mil vezes.
typedef struct {
    Texture2D textures[MAX_SKINS]; // Vetor com todas as imagens carregadas
    int current_skin_index;        // Qual skin está selecionada agora
} SkinManager;

// Representa um tiro da raquete.
// LISTA ENCADEADA: O jogador pode atirar quantas vezes quiser.
typedef struct Bullet {
    Rectangle rect;
    float speed;
    bool is_active;
    struct Bullet *next; // Ponteiro para o próximo tiro
} Bullet;

// --- VARIÁVEIS GLOBAIS (EXTERN) ---
// Permite que outros arquivos (como o main.c) acessem essas variáveis.
extern PowerUpNode *powerup_head;
extern SkinManager ball_skins;
extern FallingParticle particles[NUM_PARTICLES];
extern Bullet *bullet_head;
extern BallNode *ball_head;

#endif