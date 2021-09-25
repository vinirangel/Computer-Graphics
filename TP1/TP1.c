#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"
#define NUM_PROCESSES  3

static const char *processText[] = {
    "Reta DDA",
    "Menor Bresenham",
    "Cirfunferencia Bresenham",
};

void DDA(int x1, int y1, int x2, int y2)
{
    float dx, dy, passos, k;
    float x_incr, y_incr, x, y;
    dx = x2 - x1;
    dy = y2 - y1;
    if(abs(dx) > abs(dy))
        passos = abs(dx);
    else
        passos = abs(dy);
    x_incr = dx/passos;
    y_incr = dy/passos;
    x = x1;
    y = y1;
    DrawPixel(ceil(x), ceil(y), RED);
    for(k = 1; k < passos; k++)
    {
        x+= x_incr;
        y+= y_incr;
        DrawPixel(ceil(x), ceil(y), RED);
    }
}

void Reta_Bresenham(int x1, int y1, int x2, int y2)
{
    int dx, dy, x, y, const1, const2, p;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    p = 2*dy - dx;
    const1 = 2*dy;
    const2 = 2*(dy-dx);
    x = x1;
    y = y1;
    DrawPixel(x, y, RED);
    while(x < x2)
    {
        x+=1;
        if(p < 0)
        {
            p+= const1;
        }
        else{
            p+= const2;
            y++;
        }
        DrawPixel(x,y, RED);
    }
}

void drawCircle(int xc, int yc, int x, int y)
{
    DrawPixel(xc+x, yc+y, RED);
    DrawPixel(xc-x, yc+y, RED);
    DrawPixel(xc+x, yc-y, RED);
    DrawPixel(xc-x, yc-y, RED);
    DrawPixel(xc+y, yc+x, RED);
    DrawPixel(xc-y, yc+x, RED);
    DrawPixel(xc+y, yc-x, RED);
    DrawPixel(xc-y, yc-x, RED);
}

void circleBres(int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
    while (y >= x)
    {
         
        x++;
        //Atualiza d, x e y baseado no parametro de desisao
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(xc, yc, x, y);
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Algoritmos CG");
    SetTargetFPS(60);
    int startX, startY, endX, endY = -100;
    Rectangle toggleRecs[NUM_PROCESSES] = { 0 };
    int currentProcess = 0;
    int mouseHoverRec = -1;
    for (int i = 0; i < NUM_PROCESSES; i++) 
    toggleRecs[i] = (Rectangle){ 15.0f, (float)(620 + 32*i), 300.0f, 30.0f };
    
    while (!WindowShouldClose())
    {
        //Altera os botoes do menu ao serem selecionados
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), toggleRecs[i]))
            {
                mouseHoverRec = i;

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    currentProcess = i;
                }
                break;
            }
            else mouseHoverRec = -1;
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        DrawLine(1280, 360, 0, 360, BLACK);
        DrawLine(640, 720, 640, 0, BLACK);
        DrawCircle(startX, startY, 5, RED);
        DrawCircle(endX, endY, 5, RED);
        //Desenha os menus
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            DrawRectangleRec(toggleRecs[i], ((i == currentProcess) || (i == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
            DrawRectangleLines((int)toggleRecs[i].x, (int) toggleRecs[i].y, (int) toggleRecs[i].width, (int) toggleRecs[i].height, ((i == currentProcess) || (i == mouseHoverRec)) ? BLUE : GRAY);
            DrawText( processText[i], (int)( toggleRecs[i].x + toggleRecs[i].width/2 - MeasureText(processText[i], 10)/2), (int) toggleRecs[i].y + 11, 10, ((i == currentProcess) || (i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
        }
        //Adquire o ponto final e inicial baseado no click esquerdo e direito do mouse
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            startX = GetMouseX();
            startY = GetMouseY();
        }
        if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            endX = GetMouseX();
            endY = GetMouseY();
        }
        //Executa o algoritmo a ser executado baseado no menu selecionado
        if(startX > 0 && startY > 0 && endX > 0 && endY > 0 && currentProcess == 0)
        {
            DDA(startX, startY, endX, endY);
        }
        if(startX > 0 && startY > 0 && endX > 0 && endY > 0 && currentProcess == 1)
        {
            Reta_Bresenham(startX, startY, endX, endY);
        }
        if(currentProcess == 2)
        {
            circleBres(startX, endY, 50);
        }
        
        ClearBackground(RAYWHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}