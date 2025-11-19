#include "raylib.h"

#define MAX_SCORE_NAME 20

typedef struct {
    Rectangle rect;
    int type;
    Color color;
    int is_active;
} Block;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float radius;
} Ball;

typedef struct {
    Rectangle rect;
    float speed;
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

extern PowerUpNode *powerup_head;