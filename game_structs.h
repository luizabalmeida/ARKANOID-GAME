#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include "raylib.h"

//CONFIGURAÇÕES GERAIS 
#define MAX_SCORE_NAME 20      
#define MAX_SKINS 23          
#define NUM_PARTICLES 100       
#define PARTICLE_LENGTH_MAX 15 
#define TRAIL_LENGTH 10       

//CÓDIGOS DOS POWER-UPS (PODERES) 
#define POWERUP_REVERSE_CONTROLS 4 
#define POWERUP_GUN_PADDLE 5      
#define POWERUP_MULTI_BALL 6      

// Estrutura do Bloquinho (Tijolinho)
typedef struct {
    Rectangle rect; 
    int type;       
    Color color;    
    int is_active;  
    int life;       
} Block;

// Estrutura da Bola 
typedef struct BallNode {
    Vector2 position; 
    Vector2 velocity; 
    float radius;    
    Vector2 previous_positions[TRAIL_LENGTH];
    struct BallNode *next; 
} BallNode;

// Estrutura da Raquete
typedef struct {
    Rectangle rect; 
    float speed;   
    
    // Estados de efeitos (Power-ups ativos)
    bool controls_reversed; 
    double time_effect_started; 

    bool has_gun;
    double time_gun_started;
    double time_last_shot; 

    bool size_changed;        
    double time_size_changed; 
} Paddle;

// Estrutura do PowerUp (Item que cai)
typedef struct PowerUp {
    Rectangle rect; 
    int type;       
    float speed;    
    struct PowerUp *next; 
} PowerUpNode;

// Estrutura para Placar de Líderes
typedef struct {
    char name[MAX_SCORE_NAME]; 
    int score;                
} HighScore;

// Estrutura para Partículas do Fundo 
typedef struct {
    Vector2 position; 
    float speed;    
    float length;    
    Color color;     
} FallingParticle;

// Gerenciador de Skins (as imagens)
typedef struct {
    Texture2D textures[MAX_SKINS]; 
    int current_skin_index;      
} SkinManager;

// Estrutura do Tiro (Bala)
typedef struct Bullet {
    Rectangle rect; 
    float speed;    
    bool is_active; 
    struct Bullet *next; 
} Bullet;

// Declaração das variáveis globais que estão tbm no arquivo main.c
extern PowerUpNode *powerup_head;
extern SkinManager ball_skins;
extern FallingParticle particles[NUM_PARTICLES];
extern Bullet *bullet_head;
extern BallNode *ball_head;

#endif