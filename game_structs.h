#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include "raylib.h"

#define MAX_SCORE_NAME 20
#define MAX_SKINS 23 
#define NUM_PARTICLES 100 
#define PARTICLE_LENGTH_MAX 15

#define POWERUP_REVERSE_CONTROLS 4 
#define POWERUP_GUN_PADDLE 5
#define POWERUP_MULTI_BALL 6

typedef struct {
    Rectangle rect;
    int type;
    Color color;
    int is_active;
    int life; 
} Block;

typedef struct BallNode {
    Vector2 position;
    Vector2 velocity;
    float radius;
    struct BallNode *next;
} BallNode;

typedef struct {
    Rectangle rect;
    float speed;
    
    bool controls_reversed; 
    double time_effect_started; 

    bool has_gun;
    double time_gun_started;
    double time_last_shot;

    bool size_changed;        
    double time_size_changed; 
} Paddle;

typedef struct PowerUp {
    Rectangle rect;
    int type;
    float speed;
    struct PowerUp *next;
} PowerUpNode;

typedef struct {
    char name[MAX_SCORE_NAME];
    int score;
} HighScore;

typedef struct {
    Vector2 position;
    float speed;
    float length;
    Color color;
} FallingParticle;

typedef struct {
    Texture2D textures[MAX_SKINS];
    int current_skin_index;
} SkinManager;

typedef struct Bullet {
    Rectangle rect;
    float speed;
    bool is_active;
    struct Bullet *next;
} Bullet;

extern PowerUpNode *powerup_head;
extern SkinManager ball_skins;
extern FallingParticle particles[NUM_PARTICLES];
extern Bullet *bullet_head;
extern BallNode *ball_head;

#endif