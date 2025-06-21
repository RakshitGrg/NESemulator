#include <stdlib.h>

#include "LIB/nesdoug.h"
#include "LIB/neslib.h"
#include "backNT.h"
#include "metasprites.h"

#define totalPoss 15
#define MAX_HEARTS 5

const unsigned char paletteSpr[16] = {0x3c, 0x0f, 0x16, 0x2a, 0x3c, 0x0f,
                                      0x13, 0x2a, 0x3c, 0x0f, 0x18, 0x28,
                                      0x3c, 0x0f, 0x27, 0x2a};

const unsigned char paletteBG[16] = {0x1a, 0x37, 0x3c, 0x30, 0x1a, 0x01,
                                     0x21, 0x31, 0x1a, 0x06, 0x16, 0x26,
                                     0x1a, 0x09, 0x19, 0x29};

typedef struct sprites {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} sprites;

sprites basket = {128, 185, 31, 31};
unsigned char controller;

sprites fruits[totalPoss];
sprites heart[MAX_HEARTS];
unsigned char visible[totalPoss];
unsigned char fruitType[totalPoss];
unsigned char heart_color[MAX_HEARTS];  // to depict the color of each heart
unsigned char scoreText[5];
unsigned int score = 0;
unsigned char speed = 50;
unsigned char counter = 0;
unsigned char i;
unsigned char basketSpeed = 2;
unsigned char gameState = 0;  // 0 - Not Started 1 = Game Running 2 = Game Over
unsigned char playText[5] = {'P', 'L', 'A', 'Y', '!'};
unsigned char gameOverText[9] = {'G', 'A', 'M', 'E','O', 'V', 'E', 'R', '!'};
unsigned char scoreConstText[5] = {'S', 'C', 'O', 'R', 'E'};
// Fruit Type:
// 0 = Bomb
// 1 = Banana
// 2 = Apple
// 3 = Orange
// 4 = Watermelon
// 5 = Grapes

void moveBasket() {
    if (controller & PAD_LEFT) {
        basket.x -= basketSpeed;
    } else if (controller & PAD_RIGHT){
        basket.x += basketSpeed;
    }
}

unsigned char checkCollision(sprites fruit, sprites basket) {
    if ((fruit.x > basket.x - 28 && fruit.x - 16 < basket.x - 4) &&
        (fruit.y > basket.y - 24 && fruit.y - 16 < basket.y)) {
        return 1;
    }
    return 0;
}

void convertScoreToChar(unsigned int num) {
    scoreText[4] = num % 10;
    num /= 10;
    scoreText[3] = num % 10;
    num /= 10;
    scoreText[2] = num % 10;
    num /= 10;
    scoreText[1] = num % 10;
    num /= 10;
    scoreText[0] = num % 10;
    num /= 10;
}

void drawHearts() {
    for (i = 0; i < MAX_HEARTS; ++i) {
        if (heart_color[i] == 1)
            oam_spr(210 + i * 8, 10, 8, 0);
        else
            oam_spr(210 + i * 8, 10, 9, 0);
    }
}

void updateHeart() {
    signed char j = MAX_HEARTS;
    for (j = MAX_HEARTS - 1; j >= 0; --j) {
        if (heart_color[j] == 1) {
            heart_color[j] = 0;
            break;
        }
    }
}

void initializeNewGame() {
    score = 0;
    speed = 50;
    basketSpeed = 2;
    for (i = 0; i < 5; ++i) {
        scoreText[i] = 0;
    }

    for (i = 0; i < totalPoss; ++i) {
        visible[i] = 0;
        fruits[i].height = 15;
        fruits[i].width = 15;
        fruits[i].x = 0;
        fruits[i].y = 0;
        fruitType[i] = 0;
    }
    for (i = 0; i < MAX_HEARTS; ++i) {
        heart_color[i] = 1;
    }
}

void main() {
    ppu_off();
    initializeNewGame();
    pal_spr((const char*)paletteSpr);
    pal_bg((const char*)paletteBG);
    bank_bg(1);
    vram_adr(NAMETABLE_A);
    vram_unrle(backNT);

    ppu_on_all();

    while (1) {
        ppu_wait_nmi();
        oam_clear();
        controller = pad_poll(0);

        // Start screen
        if (gameState == 0) {
            oam_clear();
            for (i = 0; i < 5; ++i) {
                oam_spr(120 + i * 8, 104, playText[i] + 96, 3);
            }
            if (controller & PAD_START) {
                gameState = 1;
            }
        }

        // Game ending
        if (gameState == 2) {
            oam_clear();
            for (i = 0; i < 4; ++i) {
                oam_spr(100 + i * 8, 80, gameOverText[i] + 96, 0);
            }
            for (i = 4; i < 9; ++i) {
                oam_spr(100 + (i - 4) * 8, 90, gameOverText[i] + 96, 0);
            }

            for (i = 0; i < 5; ++i) {
                oam_spr(100 + i * 8, 105, scoreConstText[i] + 96, 2);
            }

            for (i = 0; i < 5; ++i) {
                oam_spr(100 + i * 8, 115, scoreText[i] + 144, 2);
            }

            if (controller & PAD_START) {
                initializeNewGame();
                gameState = 1;
            }
        }

        if (gameState == 1) {
            moveBasket();
            oam_meta_spr(basket.x, basket.y, basketMS);
            ++counter;
            // Create a new Fruit/Bomb
            if (counter >= speed) {
                counter = 0;
                for (i = 0; i < totalPoss; i++) {
                    if (visible[i] == 0) {
                        visible[i] = 1;
                        fruitType[i] = rand8() % 6;
                        fruits[i].x = rand8() % 220 + 30;
                        fruits[i].y = 0;
                        break;
                    }
                }
            }
        }

        // To make the fruits fallen/caught in basket disappear
        for (i = 0; i < totalPoss; ++i) {
            if (visible[i] == 1) {
                if (fruits[i].y > 190) {
                    visible[i] = 0;
                    if (fruitType[i] != 0) updateHeart();  // Change color of a heart
                } else if (checkCollision(fruits[i], basket)) {
                    if (fruitType[i] == 0) {
                        updateHeart();
                    } else {
                        ++score;
                        convertScoreToChar(score);
                    }
                    visible[i] = 0;
                }
            }
        }

        if (heart_color[0] == 0) {
            gameState = 2;
            continue;
        }

        // Draw the visible fruits
        for (i = 0; i < totalPoss; ++i) {
            if (visible[i] == 1) {
                if (fruitType[i] == 0) {
                    oam_meta_spr(fruits[i].x, fruits[i].y, bombMS);
                } else if (fruitType[i] == 1) {
                    oam_meta_spr(fruits[i].x, fruits[i].y, bananaMS);
                } else if (fruitType[i] == 2) {
                    oam_meta_spr(fruits[i].x, fruits[i].y, appleMS);
                } else if (fruitType[i] == 3) {
                    oam_meta_spr(fruits[i].x, fruits[i].y, orangeMS);
                } else if (fruitType[i] == 4) {
                    oam_meta_spr(fruits[i].x, fruits[i].y, watermelonMS);
                } else if (fruitType[i] == 5) {
                    oam_meta_spr(fruits[i].x, fruits[i].y, grapesMS);
                }
                ++fruits[i].y;
            }
        }

        // Draw the score
        for (i = 0; i < 5; ++i) {
            oam_spr(210 + i * 7, 20, scoreText[i] + 144, 2);
        }

        drawHearts();

        if (speed > 10) {
            speed = 50 - 3*(score/10);
            basketSpeed = (score/40) + 2;
        }
    }
}
