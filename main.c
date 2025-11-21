#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game_structs.h"

#define GAME_NAME "Arkanoid C Project"

#define BLOCK_ROWS 8
#define BLOCK_COLS 10

#define BALL_RADIUS 15.0f
#define BALL_INITIAL_SPEED 10.0f
#define PADDLE_SPEED 15.0f

#define MAX_HIGH_SCORES 5
#define MAX_NAME_LENGTH 9
#define MAX_SKINS 23

int current_screen_width = 800;
int current_screen_height = 450;

Block game_blocks[BLOCK_ROWS][BLOCK_COLS];
Paddle player_paddle;
Ball game_ball;
PowerUpNode *powerup_head = NULL;

int current_score = 0;
int active_blocks_count = 0; 

int game_state = 4; 
HighScore top_scores[MAX_HIGH_SCORES];

char player_name[MAX_NAME_LENGTH + 1] = "PLAYER";
int letter_count = 6;
bool is_typing = false;

const char *SKIN_FILE_NAMES[MAX_SKINS] = {
    "gelo", "terra", "trovao", "vidente", "diamante", "xadrez", "camuflada", "mel", 
    "pixelada", "labirinto", "morango", "chocolate", "arco-iris", "donut", "pizza", 
    "hamburguer", "sorvete", "sushi", "panda", "cachorro", "coelho", "raposa", "melancia"
};

const char *SKIN_DISPLAY_NAMES[MAX_SKINS] = {
    "GELO", "TERRA", "TROVÃO", "VIDENTE", "DIAMANTE", "XADREZ", "CAMUFLADA", "MEL", 
    "PIXELADA", "LABIRINTO", "MORANGO", "CHOCOLATE", "ARCO-ÍRIS", "DONUT", "PIZZA", 
    "HAMBÚRGUER", "SORVETE", "SUSHI", "PANDA", "CACHORRO", "COELHO", "RAPOSA", "MELANCIA"
};

SkinManager ball_skins;

void InitializeBlocks()
{
    float block_width = (float)current_screen_width / BLOCK_COLS;
    float block_height = current_screen_height * 0.05f;
    float top_offset = current_screen_height * 0.05f;

    active_blocks_count = 0; 

    for (int i = 0; i < BLOCK_ROWS; i++)
    {
        for (int j = 0; j < BLOCK_COLS; j++)
        {
            game_blocks[i][j].rect = (Rectangle){ j * block_width, top_offset + i * block_height, block_width, block_height };
            
            if (i < 2) {
                game_blocks[i][j].color = RED;
                game_blocks[i][j].life = 3;
            }
            else if (i < 4) {
                game_blocks[i][j].color = ORANGE;
                game_blocks[i][j].life = 2;
            }
            else {
                game_blocks[i][j].color = GREEN;
                game_blocks[i][j].life = 1;
            }

            game_blocks[i][j].is_active = 1;
            game_blocks[i][j].type = 1;
            
            active_blocks_count++; 
        }
    }
}

void InitializeGame()
{
    player_paddle.rect.width = current_screen_width * 0.125f;
    player_paddle.rect.height = current_screen_height * 0.035f;
    
    player_paddle.rect.x = (current_screen_width / 2) - (player_paddle.rect.width / 2);
    player_paddle.rect.y = current_screen_height - player_paddle.rect.height * 2.0f;
    player_paddle.speed = PADDLE_SPEED;

    game_ball.position = (Vector2){ current_screen_width / 2.0f, player_paddle.rect.y - BALL_RADIUS };
    game_ball.radius = BALL_RADIUS;
    game_ball.velocity = (Vector2){ BALL_INITIAL_SPEED, -BALL_INITIAL_SPEED };
    
    current_score = 0;
    InitializeBlocks();
}

void LoadScores() {
    FILE *file = fopen("scores.dat", "rb");
    if (file == NULL) {
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            strcpy(top_scores[i].name, "---");
            top_scores[i].score = 0;
        }
        return;
    }
    fread(top_scores, sizeof(HighScore), MAX_HIGH_SCORES, file);
    fclose(file);
}

void SaveScores(int new_score) {
    if (new_score <= top_scores[MAX_HIGH_SCORES - 1].score) return;
    
    HighScore new_entry;
    new_entry.score = new_score;
    strcpy(new_entry.name, player_name); 

    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (new_score > top_scores[i].score) {
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
                top_scores[j] = top_scores[j - 1];
            }
            top_scores[i] = new_entry;
            break;
        }
    }

    FILE *file = fopen("scores.dat", "wb");
    if (file != NULL) {
        fwrite(top_scores, sizeof(HighScore), MAX_HIGH_SCORES, file);
        fclose(file);
    }
}

void AddPowerUp(float x, float y, int type)
{
    PowerUpNode *newNode = (PowerUpNode *)malloc(sizeof(PowerUpNode));
    if (newNode == NULL) return;
    
    newNode->rect = (Rectangle){ x + 10, y + 5, current_screen_width * 0.025f, current_screen_height * 0.02f };
    newNode->type = type;
    newNode->speed = 2.0f;
    newNode->next = powerup_head;
    powerup_head = newNode;
}

void ClearPowerUps()
{
    PowerUpNode *current = powerup_head;
    PowerUpNode *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    powerup_head = NULL;
}

void ApplyPowerUp(int type)
{
    switch (type) {
        case 1: 
            player_paddle.rect.width = current_screen_width * 0.20f;
            break;
        case 2: 
            player_paddle.rect.width = current_screen_width * 0.08f;
            break;
        case 3: 
        {
            float current_speed_x = (game_ball.velocity.x > 0) ? 1.0f : -1.0f;
            float current_speed_y = (game_ball.velocity.y > 0) ? 1.0f : -1.0f;
            game_ball.velocity.x = (BALL_INITIAL_SPEED * 0.6f) * current_speed_x;
            game_ball.velocity.y = (BALL_INITIAL_SPEED * 0.6f) * current_speed_y;
            break;
        }
        default: break;
    }
}

void CheckBallBlockCollision(Ball *ball) 
{
    for (int i = 0; i < BLOCK_ROWS; i++)
    {
        for (int j = 0; j < BLOCK_COLS; j++)
        {
            if (game_blocks[i][j].is_active) 
            {
                if (CheckCollisionCircleRec(ball->position, ball->radius, game_blocks[i][j].rect))
                {
                    ball->position.x -= ball->velocity.x;
                    ball->position.y -= ball->velocity.y;
                    
                    ball->velocity.y *= -1.0f;
                    
                    game_blocks[i][j].life--;

                    if (game_blocks[i][j].life > 0) 
                    {
                        if (game_blocks[i][j].life == 2) game_blocks[i][j].color = ORANGE;
                        else if (game_blocks[i][j].life == 1) game_blocks[i][j].color = GREEN;
                        current_score += 5; 
                    }
                    else 
                    {
                        game_blocks[i][j].is_active = 0; 
                        active_blocks_count--; 
                        current_score += 10;
                        
                        if (active_blocks_count <= 0) 
                        {
                            InitializeBlocks(); 
                            ball->position = (Vector2){ current_screen_width / 2.0f, player_paddle.rect.y - BALL_RADIUS };
                            
                            float speedMultiplier = 1.1f;
                            ball->velocity.y = (ball->velocity.y > 0) ? -BALL_INITIAL_SPEED * speedMultiplier : BALL_INITIAL_SPEED * speedMultiplier;
                            ball->velocity.x = BALL_INITIAL_SPEED * speedMultiplier;
                            
                            ClearPowerUps();
                        }

                        if (GetRandomValue(1, 10) <= 3) {
                            int power_type = GetRandomValue(1, 3); 
                            AddPowerUp(game_blocks[i][j].rect.x, game_blocks[i][j].rect.y, power_type);
                        }
                    }
                    return; 
                }
            }
        }
    }
}

void UpdateGame()
{
    if (IsKeyDown(KEY_LEFT)) player_paddle.rect.x -= player_paddle.speed;
    if (IsKeyDown(KEY_RIGHT)) player_paddle.rect.x += player_paddle.speed;

    if (player_paddle.rect.x <= 0) player_paddle.rect.x = 0;
    if (player_paddle.rect.x + player_paddle.rect.width >= current_screen_width) 
        player_paddle.rect.x = current_screen_width - player_paddle.rect.width;

    game_ball.position.x += game_ball.velocity.x;
    game_ball.position.y += game_ball.velocity.y;

    if ((game_ball.position.x + game_ball.radius >= current_screen_width) || (game_ball.position.x - game_ball.radius <= 0))
        game_ball.velocity.x *= -1.0f;
    if (game_ball.position.y - game_ball.radius <= 0)
        game_ball.velocity.y *= -1.0f;
    
    if (game_ball.position.y + game_ball.radius >= current_screen_height)
    {
        game_state = 2;
        SaveScores(current_score);
        ClearPowerUps();
    }
    
    if (CheckCollisionCircleRec(game_ball.position, game_ball.radius, player_paddle.rect) && game_ball.velocity.y > 0)
    {
        game_ball.velocity.y *= -1.0f;
        float hit_point = game_ball.position.x - player_paddle.rect.x;
        float relative_hit = hit_point / player_paddle.rect.width;
        float direction = (relative_hit * 2.0f - 1.0f);
        float speed = BALL_INITIAL_SPEED * 1.05f; 
        
        game_ball.velocity.x = direction * speed;
        game_ball.velocity.y = -speed;
    }

    CheckBallBlockCollision(&game_ball);

    PowerUpNode *current = powerup_head;
    PowerUpNode *prev = NULL;

    while (current != NULL)
    {
        current->rect.y += current->speed;
        
        if (CheckCollisionRecs(current->rect, player_paddle.rect))
        {
            ApplyPowerUp(current->type);
            if (prev == NULL) powerup_head = current->next;
            else prev->next = current->next;
            PowerUpNode *temp = current;
            current = current->next;
            free(temp);
            continue;
        }
        
        if (current->rect.y > current_screen_height)
        {
            if (prev == NULL) powerup_head = current->next;
            else prev->next = current->next;
            PowerUpNode *temp = current;
            current = current->next;
            free(temp);
            continue;
        }
        prev = current;
        current = current->next;
    }
}

void DrawSkinSelector()
{
    float font_size_title = current_screen_height * 0.08f;
    float font_size_text = current_screen_height * 0.04f;
    
    DrawRectangle(0, 0, current_screen_width, current_screen_height, Fade(BLACK, 0.9f));
    
    DrawText("SELECIONE SUA BOLA", 
             current_screen_width/2 - MeasureText("SELECIONE SUA BOLA", font_size_title)/2, 
             current_screen_height/6, 
             font_size_title, YELLOW);
             
    const char *skin_name = SKIN_DISPLAY_NAMES[ball_skins.current_skin_index];
    DrawText(skin_name, 
             current_screen_width/2 - MeasureText(skin_name, font_size_text * 1.5f)/2, 
             current_screen_height * 0.28f, 
             font_size_text * 1.5f, WHITE);
             
    Texture2D current_texture = ball_skins.textures[ball_skins.current_skin_index];
    
    float preview_size = current_screen_height * 0.20f;
    Rectangle destRect = { 
        current_screen_width/2 - preview_size/2, 
        current_screen_height/2 - preview_size/2, 
        preview_size, 
        preview_size 
    };
    Rectangle sourceRect = { 0.0f, 0.0f, (float)current_texture.width, (float)current_texture.height };

    DrawTexturePro(current_texture, sourceRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
    
    DrawText("<", current_screen_width/2 - preview_size/2 - 50, current_screen_height/2 - 20, font_size_title, LIME);
    DrawText(">", current_screen_width/2 + preview_size/2 + 20, current_screen_height/2 - 20, font_size_title, LIME);
    
    DrawText("Use SETAS ESQUERDA/DIREITA | ENTER para Continuar", 
             current_screen_width/2 - MeasureText("Use SETAS ESQUERDA/DIREITA | ENTER para Continuar", font_size_text)/2, 
             current_screen_height * 0.85f, 
             font_size_text, RAYWHITE);

    if (IsKeyPressed(KEY_RIGHT))
    {
        ball_skins.current_skin_index = (ball_skins.current_skin_index + 1) % MAX_SKINS;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        ball_skins.current_skin_index = (ball_skins.current_skin_index - 1 + MAX_SKINS) % MAX_SKINS;
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        game_state = 3; 
    }
}


void DrawGame()
{
    BeginDrawing();
    
        ClearBackground(DARKGRAY);
        
        for (int i = 0; i < BLOCK_ROWS; i++)
        {
            for (int j = 0; j < BLOCK_COLS; j++)
            {
                if (game_blocks[i][j].is_active)
                {
                    DrawRectangleRec(game_blocks[i][j].rect, game_blocks[i][j].color);
                    DrawRectangleLinesEx(game_blocks[i][j].rect, 1, BLACK); 
                }
            }
        }

        DrawRectangleRec(player_paddle.rect, BLUE);
        
        if (game_state != 4) 
        {
            Texture2D current_texture = ball_skins.textures[ball_skins.current_skin_index];
            Rectangle destRect = { 
                game_ball.position.x - game_ball.radius, 
                game_ball.position.y - game_ball.radius, 
                game_ball.radius * 2, 
                game_ball.radius * 2 
            };
            Rectangle sourceRect = { 0.0f, 0.0f, (float)current_texture.width, (float)current_texture.height };
            DrawTexturePro(current_texture, sourceRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        }

        PowerUpNode *current = powerup_head;
        while (current != NULL)
        {
            DrawRectangleRec(current->rect, (current->type == 1) ? MAGENTA : ((current->type == 2) ? LIME : YELLOW));
            current = current->next;
        }
        
        DrawText(TextFormat("SCORE: %d", current_score), 10, 10, current_screen_height * 0.04f, WHITE); 

        if (game_state == 2)
        {
            float font_size_large = current_screen_height * 0.08f;
            float font_size_small = current_screen_height * 0.04f;
            float font_size_options = current_screen_height * 0.035f;

            DrawRectangle(0, 0, current_screen_width, current_screen_height, Fade(BLACK, 0.8f));
            
            DrawText("GAME OVER!", 
                     current_screen_width/2 - MeasureText("GAME OVER!", font_size_large)/2, 
                     current_screen_height/4, 
                     font_size_large, RED);
            
            DrawText("CONTINUAR (ENTER)", 
                     current_screen_width/2 - MeasureText("CONTINUAR (ENTER)", font_size_options)/2, 
                     current_screen_height/2 - font_size_options, 
                     font_size_options, WHITE);
                     
            DrawText("VOLTAR AO MENU (ESPACO)", 
                     current_screen_width/2 - MeasureText("VOLTAR AO MENU (ESPACO)", font_size_options)/2, 
                     current_screen_height/2 + font_size_options, 
                     font_size_options, WHITE);
            
            float ranking_y_start = current_screen_height/2 + font_size_large + font_size_options;
            DrawText("TOP 5 SCORES:", 
                     current_screen_width/2 - MeasureText("TOP 5 SCORES:", font_size_small)/2, 
                     ranking_y_start, 
                     font_size_small, YELLOW);
                     
            for (int i = 0; i < MAX_HIGH_SCORES; i++) {
                char text_buffer[50];
                sprintf(text_buffer, "%d. %s: %d", i + 1, top_scores[i].name, top_scores[i].score);
                
                DrawText(text_buffer, 
                         current_screen_width/2 - MeasureText(text_buffer, font_size_small)/2, 
                         ranking_y_start + font_size_small + i * (font_size_small * 1.2f), 
                         font_size_small, WHITE);
            }
            
            if (IsKeyPressed(KEY_ENTER))
            {
                InitializeGame();
                game_state = 1;
            }
            else if (IsKeyPressed(KEY_SPACE))
            {
                ClearPowerUps(); 
                game_state = 4;
            }
        }
        
        else if (game_state == 3) 
        {
            float font_size_title = current_screen_height * 0.08f;
            float font_size_text = current_screen_height * 0.04f;
            char instruction_text[100];
            
            DrawRectangle(0, 0, current_screen_width, current_screen_height, Fade(BLACK, 0.9f));
            
            if (!is_typing) {
                sprintf(instruction_text, "JOGAR COMO: %s", player_name);
                DrawText(instruction_text, 
                         current_screen_width/2 - MeasureText(instruction_text, font_size_title)/2, 
                         current_screen_height/3, 
                         font_size_title, GREEN);
                         
                DrawText("ENTER para Manter | ESPACO para Mudar Nome", 
                         current_screen_width/2 - MeasureText("ENTER para Manter | ESPACO para Mudar Nome", font_size_text)/2, 
                         current_screen_height/2, 
                         font_size_text, WHITE);

                if (IsKeyPressed(KEY_ENTER)) {
                    InitializeGame();
                    game_state = 1;
                } else if (IsKeyPressed(KEY_SPACE)) {
                    is_typing = true;
                    letter_count = 0;
                    player_name[0] = '\0';
                }

            } else {
                DrawText("INSIRA SEU NOME (MAX 9 LETRAS):", 
                         current_screen_width/2 - MeasureText("INSIRA SEU NOME (MAX 9 LETRAS):", font_size_text)/2, 
                         current_screen_height/3, 
                         font_size_text, WHITE);
                         
                char display_name[MAX_NAME_LENGTH + 2];
                strcpy(display_name, player_name);
                if ((GetTime() / 0.5f) < (int)(GetTime() / 0.5f)) display_name[letter_count] = '_';
                display_name[letter_count + 1] = '\0';
                
                DrawText(display_name, 
                         current_screen_width/2 - MeasureText(display_name, font_size_title)/2, 
                         current_screen_height/2, 
                         font_size_title, RAYWHITE);
                         
                DrawText("ENTER para Confirmar", 
                         current_screen_width/2 - MeasureText("ENTER para Confirmar", font_size_text)/2, 
                         current_screen_height/2 + font_size_title, 
                         font_size_text, YELLOW);

                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 32) && (key <= 125) && (letter_count < MAX_NAME_LENGTH))
                    {
                        player_name[letter_count] = (char)key;
                        letter_count++;
                        player_name[letter_count] = '\0';
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letter_count--;
                    if (letter_count < 0) letter_count = 0;
                    player_name[letter_count] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && letter_count > 0)
                {
                    is_typing = false;
                    InitializeGame();
                    game_state = 1;
                } else if (IsKeyPressed(KEY_ENTER) && letter_count == 0) {
                    strcpy(player_name, "PLAYER"); 
                    is_typing = false;
                    InitializeGame();
                    game_state = 1;
                }
            }
        }
        
        else if (game_state == 4) 
        {
            DrawSkinSelector();
        }

    EndDrawing();
}

int main(void)
{
    int monitorWidth = GetMonitorWidth(0);
    int monitorHeight = GetMonitorHeight(0);
    
    InitWindow(monitorWidth, monitorHeight, GAME_NAME);
    
    ToggleFullscreen(); 
    
    current_screen_width = GetScreenWidth();
    current_screen_height = GetScreenHeight();

    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    LoadScores();
    InitializeGame();

    for (int i = 0; i < MAX_SKINS; i++)
    {
        char path[100];
        sprintf(path, "skins/%s.png", SKIN_FILE_NAMES[i]); 
        ball_skins.textures[i] = LoadTexture(path);
    }
    ball_skins.current_skin_index = 0;
    
    while (!WindowShouldClose())
    {
        if (game_state == 1)
        {
            UpdateGame();
        }
        else if (game_state == 2)
        {
            
        }
        else if (game_state == 3)
        {
            
        }
        else if (game_state == 4)
        {
            
        }
        
        DrawGame();
    }

    for (int i = 0; i < MAX_SKINS; i++)
    {
        UnloadTexture(ball_skins.textures[i]);
    }
    
    CloseWindow();
    return 0;
}