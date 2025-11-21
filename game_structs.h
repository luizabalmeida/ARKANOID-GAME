#include "raylib.h"

#define MAX_SCORE_NAME 20
#define MAX_SKINS 23 

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
}
Ball;

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

typedef struct {
    Texture2D textures[MAX_SKINS];
    int current_skin_index;
} SkinManager;

extern PowerUpNode *powerup_head;
extern SkinManager ball_skins;