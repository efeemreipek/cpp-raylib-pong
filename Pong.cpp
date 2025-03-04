#include <iostream>
#include <raylib.h>

int playerScore = 0;
int cpuScore = 0;

Color green = Color{38, 185, 154, 255};
Color darkGreen = Color{20, 160, 133, 255};
Color lightGreen = Color{129, 204, 184, 255};
Color yellow = Color{243, 213, 91, 255};

Sound hitSound, pointSound;

class Ball
{
public:
    float x, y;
    int speedX, speedY;
    int radius;
    void Draw()
    {
        DrawCircle(x, y, radius, yellow);
    }
    void Update()
    {
        x += speedX;
        y += speedY;

        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speedY *= -1;
            PlaySound(hitSound);
        }
        if(x + radius >= GetScreenWidth())
        {
            cpuScore++;
            PlaySound(pointSound);
            ResetBall();
        }
        if(x - radius <= 0)
        {
            playerScore++;
            PlaySound(pointSound);
            ResetBall();
        }
    }
    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speedChoices[2] = {-1, 1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if(y <= 0)
        {
            y = 0;
        }
        if(y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }
    void Update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }
        
        LimitMovement();
    }
};

class CPUPaddle : public Paddle
{
public:
    void Update(int ballY)
    {
        if(y + height / 2 > ballY)
        {
            y -= speed;
        }
        if(y + height / 2 <= ballY)
        {
            y += speed;
        }

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main()
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 800;

    // Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Game");
    InitAudioDevice();
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.speedX = 7;
    ball.speedY = 7;

    player.width = 25;
    player.height = 120;
    player.x = SCREEN_WIDTH - player.width - 10;
    player.y = SCREEN_HEIGHT / 2 - player.height / 2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x =10;
    cpu.y = SCREEN_HEIGHT / 2 - cpu.height / 2;
    cpu.speed = 6;

    hitSound = LoadSound("resources/hit.wav");
    pointSound = LoadSound("resources/point.wav");

    while(!WindowShouldClose())
    {
        BeginDrawing();

        // Update
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Check collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speedX *= -1;
            PlaySound(hitSound);
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speedX *= -1;
            PlaySound(hitSound);
        }

        // Drawing
        ClearBackground(darkGreen);
        DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, green);
        DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, lightGreen);
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpuScore), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    // Deinit
    CloseAudioDevice();
    CloseWindow();

    return 0;
}