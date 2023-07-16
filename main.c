#include <gb/gb.h>
#include <gbdk/console.h>

#include <stdio.h>

#define LabelWidth 21 // Width of the label
#define LabelHeight 18 // Height of the label 
#define LabelBank 0 // Bank of the label

// Tile Ball
unsigned char TileLabelBall[] =
{
  0x7E,0x7E,0xFF,0x9F,0xFF,0xBF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7E,0x7E
};

// Tile Palette
unsigned char TileLabelPalette[] =
{
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};


// Ball variables
UINT8 BallX = 72;
UINT8 BallY = 66;
UINT8 BallSpeedX = 2;
UINT8 BallSpeedY = 2;

// Player variables
UINT8 PlayerY = 70;

// Enemy variables
UINT8 EnemyY = 70;

// Global variables
UINT8 MoveSpeed = 2;
UINT8 IsPaused = 0;
UINT16 Score1 = 0;
UINT16 Score2 = 0;

// Declare functions
void Ball();
void Player();
void Enemy();
void TitleScreen();
int GameOver();
void ResetGame();
void PlaySound();

// Main function
void main()
{
    unsigned char prevStartState = 0; // Old state of "Start" button
    PlaySound(); // Start the sound
    SHOW_SPRITES;// Show sprites, background and enable display
    TitleScreen(); // Show title screen
    gotoxy(0, 0); // Print score
    printf("       %d   %d", Score1, Score2); // Print score
    // Loop forever
    while(1)
    {
        while(1)
        {
            // Call Functions
            if (IsPaused == 0)
            {
                // Call functions
                Ball();
                Player();
                Enemy();
                if (GameOver() == 1)
                {
                    break;
                }
            }

            // Pause game
            unsigned char currentStartState = joypad() & J_START; // State of "Start" button
            if (currentStartState && !prevStartState)
            {
                if (IsPaused == 1)
                {
                    IsPaused = 0;
                    gotoxy(0, 6);
                    printf("\n              \n\n                  ");
                }
                else
                {
                    IsPaused = 1;
                    gotoxy(0, 6);
                    printf("\n       Pause \n\n    Press  Start");
                }
            }
            prevStartState = currentStartState; // Update previous state
            
            // Delay
            delay(20);

            // Wait for next iteration
            wait_vbl_done();
        }

        if (Score1 > Score2)
        {
            // Hide sprites, background and disable display
            HIDE_SPRITES;
            // Print game over
            gotoxy(0, 6);
            printf("\n     Game Over\n\n   Player 1 wins!");
            delay(3000);
        }
        else
        {
            // Hide sprites, background and disable display
            HIDE_SPRITES;
            // Print game over
            gotoxy(0, 6);
            printf("\n     Game Over\n\n   Enemy   wins!");
            delay(3000);
        }

        ResetGame();
        TitleScreen();
    }
}


// Ball function
void Ball()
{
    // Load tile
    set_sprite_data(0, 1, TileLabelBall);
    set_sprite_tile(0, 0);
    move_sprite(0, BallX, BallY);

    // Move ball while it is not at the end of the screen
    BallX = BallX + BallSpeedX;
    BallY = BallY + BallSpeedY;
    move_sprite(0, BallX, BallY);

    // Change direction if ball is at the end of the screen
    if (BallY > 144 + 8 || BallY < 0 + 16)
    {
        BallSpeedY = -BallSpeedY;
    }

    // Reset ball if it is at the end of the screen
    if (BallX > 150 + 8 && BallX < 200)
    {
        BallSpeedX *= -1;
        Score1++;
        gotoxy(0, 0); 
        printf("       %d   %d", Score1, Score2);
        BallX = 72;
        BallY = 66;
        delay(1000);
    }
    if (BallX > 200)
    {
        BallSpeedX *= -1;
        Score2++;
        gotoxy(0, 0); 
        printf("       %d   %d", Score1, Score2);
        BallX = 72;
        BallY = 66;
        delay(1000);
    }

    // Change direction if ball hits player or enemy
    if ((BallX <= 23 && BallX > 21) && BallY <= PlayerY + 32 && BallY >= PlayerY)
    {
        BallSpeedX = -BallSpeedX;
        PlaySound();
    }
    if ((BallX >= 145 && BallX < 147)&& BallY <= EnemyY + 32 && BallY >= EnemyY)
    {
        BallSpeedX = -BallSpeedX;
        PlaySound();
    }
}

void Player()
{
    // Load tiles
    set_sprite_data(1, 1, TileLabelPalette);  // Cargar el tile de la raqueta en la posición 1
    set_sprite_tile(1, 1);  // Usar el tile de la raqueta en el sprite 1
    set_sprite_tile(2, 1);  // Usar el mismo tile de la raqueta en el sprite 2
    set_sprite_tile(3, 1);  // Usar el mismo tile de la raqueta en el sprite 3
    set_sprite_tile(4, 1);  // Usar el mismo tile de la raqueta en el sprite 4
    move_sprite(1, 18, PlayerY);  // Mover el sprite 1 a la posición inicial del jugador
    move_sprite(2, 18, PlayerY + 8);  // Mover el sprite 2 debajo del sprite 1
    move_sprite(3, 18, PlayerY + 16);  // Mover el sprite 3 debajo del sprite 2
    move_sprite(4, 18, PlayerY + 24);  // Mover el sprite 4 debajo del sprite 3

    // Move player while it is not at the end of the screen
    if (joypad() & J_UP && PlayerY > 0 + 16)
    {
        PlayerY = PlayerY - MoveSpeed;
    }
    else if (joypad() & J_DOWN && PlayerY < 144 - 16)
    {
        PlayerY = PlayerY + MoveSpeed;
    }
}

void Enemy(){
    // Load tiles
    set_sprite_data(1, 1, TileLabelPalette);  // Cargar el tile de la raqueta en la posición 1
    set_sprite_tile(5, 1);  // Usar el tile de la raqueta en el sprite 1
    set_sprite_tile(6, 1);  // Usar el mismo tile de la raqueta en el sprite 2
    set_sprite_tile(7, 1);  // Usar el mismo tile de la raqueta en el sprite 3
    set_sprite_tile(8, 1);  // Usar el mismo tile de la raqueta en el sprite 4
    move_sprite(5, 150, EnemyY);  // Mover el sprite 1 a la posición inicial del jugador
    move_sprite(6, 150, EnemyY + 8);  // Mover el sprite 2 debajo del sprite 1
    move_sprite(7, 150, EnemyY + 16);  // Mover el sprite 3 debajo del sprite 2
    move_sprite(8, 150, EnemyY + 24);  // Mover el sprite 4 debajo del sprite 3

    // Follow ball
    if (BallX > 80 && BallSpeedX > 0) {
        if (BallY > EnemyY + 16 && EnemyY < 144 - 16) {
            EnemyY += MoveSpeed;
        } else if (BallY < EnemyY + 16 && EnemyY > 0 + 16) {
            EnemyY -= MoveSpeed;
        }
    }
}

int GameOver()
{
    if (Score1 == 8 || Score2 == 8)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ResetGame()
{
    Score1 = 0;
    Score2= 0;
    IsPaused = 0;
    PlayerY = 70;
    EnemyY = 70;
    BallX = 72;
    BallY = 66;
    BallSpeedX = 2;
    BallSpeedY = 2;
    SHOW_SPRITES; 
    gotoxy(0, 6);
    printf("\n              \n\n                  ");
    gotoxy(0, 0);
    printf("       %d   %d", Score1, Score2);
}

void TitleScreen()
{
    // Hide sprites, background and disable display
    HIDE_SPRITES;
    // Print title screen
    gotoxy(0, 4);
    printf("\n     Pong Game\n\n\n\n\n\n    Press Start");
    gotoxy(0, 17);
    printf("            Sticman");
    // Wait for start button
    while(1)
    {
        if (joypad() & J_START)
        {
            break;
        }
    }
    delay(1000);
    gotoxy(0, 4);
    printf("\n              \n\n\n\n\n\n               ");
    gotoxy(0, 17);
    printf("                   ");

    SHOW_SPRITES;
    
}

void PlaySound()
{
    NR10_REG = 0x16;
    NR11_REG = 0x40;
    NR12_REG = 0xFF;
    NR13_REG = 0x00;
    NR14_REG = 0xC3;
    NR51_REG |= 0x11; 
    NR52_REG |= 0x80;  
}