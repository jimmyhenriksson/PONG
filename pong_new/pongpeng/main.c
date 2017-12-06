#include <pic32mx.h>
#include <stdint.h>
#include "btnsw.h"
#include "fonts.h"

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


#define MAX_X               127
#define MAX_Y               31
#define PLAYER_HEIGHT       8
#define PLAYER_WIDTH		2
#define BALL_HEIGHT			2
#define BALL_WIDTH			2

int player1_score = 0;
int player2_score = 0;

uint8_t font[128 * 4] = {0};

char textbuffer[4][16];

typedef struct Player {
    int x, y, speedY;
}Player;

typedef struct Ball {
    int x, y, speedX, speedY;
}Ball;


Ball ball;
Player player1,player2;

void movePlayer(){
    if (buttonOne()){
        player2.speedY = -1;
    }
    if (buttonTwo()){
        player2.speedY = 1;
    }
    if (buttonThree()) {
        player1.speedY = 1;
    }
    if (buttonFour()) {
        player1.speedY = -1;
    }
    if(player1.y < 0){
        player1.y = 0;
    }
    if(player1.y > MAX_Y - 8){
        player1.y = MAX_Y - 8;
    }
    if(player2.y < 0){
        player2.y = 0;
    }
    if(player2.y > MAX_Y - 8){
        player2.y = MAX_Y - 8;
    }
    player1.y += player1.speedY;
    player2.y += player2.speedY;
    player1.speedY = 0;
    player2.speedY = 0;
}

void pauseGame(){
    while (switchOne()) {
        display_update_pause();
        display_update();
    }
}

void increaseScore(){
    volatile int* portE = (volatile int*) 0xbf886110;
    if(player1_score == 0 & player2_score == 1){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x01);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 0 & player2_score == 2){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x03);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 0 & player2_score == 3){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x07);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 0 & player2_score >= 4){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x0f);	//turn on the leds for our current (binary) time
    }
    if(player1_score == 1 & player2_score == 0){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x80);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 2 & player2_score == 0){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xc0);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 3 & player2_score == 0){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xe0);	//turn on the leds for our current (binary) time
    }
    else if(player1_score >= 4 & player2_score == 0){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xf0);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 1 & player2_score == 1){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x81);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 2 & player2_score == 1){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xc1);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 3 & player2_score == 1){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xe1);	//turn on the leds for our current (binary) time
    }
    else if(player1_score >= 4 & player2_score == 1){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xf1);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 1 & player2_score == 2){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x83);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 2 & player2_score == 2){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xc3);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 3 & player2_score == 2){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xe3);	//turn on the leds for our current (binary) time
    }
    else if(player1_score >= 4 & player2_score == 2){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xf3);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 1 & player2_score == 3){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x87);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 2 & player2_score == 3){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xc7);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 3 & player2_score == 3){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xe7);	//turn on the leds for our current (binary) time
    }
    else if(player1_score >= 4 & player2_score == 3){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xf7);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 1 & player2_score >= 4){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0x8f);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 2 & player2_score >= 4){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xcf);	//turn on the leds for our current (binary) time
    }
    else if(player1_score == 3 & player2_score >= 4){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xef);	//turn on the leds for our current (binary) time
    }
    else if(player1_score >= 4 & player2_score >= 4){
        *portE = *portE & 0x00;		//turn off any leds previously on
        *portE = (0xff & 0xff);	//turn on the leds for our current (binary) time
    }
}

void winner(){
    int i = 1;
    if (player1_score >= 5){
        display_update_player1();
        display_update();
        while (i) {
            if (switchTwo()){
                display_update_levelBas();
                display_update();
                startGame();
                i = 0;
            }
        }
    }

    if (player2_score >= 5){
        display_update_player2();
        display_update();
        while (i) {
            if (switchTwo()){
                display_update_levelBas();
                display_update();
                startGame();
                i = 0;
            }
        }

    }
}

void changeLevel(){
    while (switchTwo()){
        startGame();
        display_update_levelBas();
        display_update();
    }
    while (switchThree()){
        ball.speedY = 3;
        display_update_levelAd();
        display_update();
    }
}

void tick() {
    ball.x += ball.speedX;
    ball.y += ball.speedY;

    // check if ball is on the bottom of the screen
    if (ball.y <= 0) {
        ball.y = 0;
        ball.speedY *= (-1);
    }

    //top of screen
    else if (ball.y >= MAX_Y) {
        ball.y = MAX_Y;
        ball.speedY *= (-1);
    }

    //if ball is on the far left side of the screen and touches player 1
    if (ball.x <= 2) {
        if ((ball.y >= player1.y) && (ball.y <= (player1.y + 8))){
            ball.x = 2;
            ball.speedX *= (-1);
        } else{
            player2_score++;
            increaseScore();
            scored();
        }
    }
        //if ball is on the right side of the screen and touches player 1
    if (ball.x >= (MAX_X - 2)) {
        if ((ball.y >= player2.y) && (ball.y <= (player2.y + 8))){
            ball.x = MAX_X-2;
            ball.speedX *= (-1);
        } else{
            player1_score++;
            increaseScore();
            scored();
        }
    }
    changeLevel();
    pauseGame();
    winner();
    movePlayer();
}

void scored(){
    ball.x = 64;
    ball.y = 5;

    player1.x = 0;
    player1.y = 14;
    player1.speedY = 0;

    player2.x = 126;
    player2.y = 14;
    player2.speedY = 0;
}

void startGame(){
    volatile int* portE = (volatile int*) 0xbf886110;
    *portE = *portE & 0x00;		//turn off any leds previously on
    player1_score = 0;
    player2_score = 0;

    ball.x = 64;
    ball.y = 5;
    ball.speedX = 2;
    ball.speedY = 1;

    player1.x = 0;
    player1.y = 14;
    player1.speedY = 0;

    player2.x = 126;
    player2.y = 14;
    player2.speedY = 0;
}

//we update a pixel into SPI format, so it can light up.
//We have 4 rows (0,1,2,3), that consist of columns of 8 bits (8*4 = 32 = screen height)
//we have 128 of these columns
void updatePixel(int x, int y){
    int row = 0;
    if(y>0) {
        row = y / 8;
    }
    // the OR-masking gives us access to the bit in our column of size 8, that we want to turn on.
    font[row * 128 + x] |= 1 << (y - row * 8);
}

//draw out the player
void drawPlayer(Player p) {
    int i, j;
    //For the width
    for (i = 0; i < PLAYER_WIDTH; i++){
        //for the height
        for (j = 0; j < PLAYER_HEIGHT; j++){
            updatePixel(p.x + i, p.y + j);
        }
    }
}


void display_print(char *s, int x, int y)
{
    int cur_char, cur_slice, cur_col, cur_row;
    int c_printed; /* number of characters printed */
    char textbuffer[16];

    /* Check if valid coordinates */
    if(x < 0 || x > (127 - 8) || y < 0 || y > (31 - 8))
        return;

    /* Check if empty string */
    if(!s)
        return;

    /* Copy string contents to text buffer */
    for(cur_char = 0; cur_char < 16; cur_char++)
    {
        if(*s)
        {
            textbuffer[cur_char] = *s;
            s++;
        }
        else
        {
            textbuffer[cur_char] = ' ';
        }
    }

    /* Copy string to screen buffer */
    for(c_printed = 0; c_printed < 16; c_printed++)
    {
        /* Get next ascii character to print */
        cur_char = textbuffer[c_printed];

        /* Copy bitmap data to screen buffer */
        for(cur_col = 0; cur_col < 8; cur_col++)
        {
            /* Get 8 pixel high slice from character bitmap */
            cur_slice = font[cur_char*8 + cur_col];

            /* Copy current 8 pixel slice to screen buffer */
            for(cur_row = 0; cur_row < 8; cur_row++)
            {
                /* If pixel in slice is set, then set pixel in buffer */
                if( (cur_slice >> cur_row) & 0x1 )
                    updatePixel(x+8*c_printed+cur_col, y+cur_row);
            }
        }
    }
}

void display_string(int line, char *s) {
    int i;
    if(line < 0 || line >= 4){
        return;
    }
    if(!s){
        return;
    }

    for(i = 0; i < 16; i++){
        if(*s) {
            textbuffer[line][i] = *s;
            s++;
        } else{
            textbuffer[line][i] = ' ';}
    }
}

void drawBall(Ball b) {
    int i, j;
    for (i = 0; i < BALL_WIDTH; i++){
        for (j = 0; j < BALL_HEIGHT; j++){
            updatePixel(b.x + i, b.y + j);
        }
    }
}


void resetScreen(){
    int i;
    for(i = 0; i< (128*4); i++){
        font[i] = 0;
    }
}

void quicksleep(int cyc) {
    int i;
    for(i = cyc; i > 0; i--);
};

uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 0x01));
    return SPI2BUF;
};

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    quicksleep(10);
    DISPLAY_ACTIVATE_VDD;
    quicksleep(1000000);

    spi_send_recv(0xAE);
    DISPLAY_ACTIVATE_RESET;
    quicksleep(10);
    DISPLAY_DO_NOT_RESET;
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_ACTIVATE_VBAT;
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}

void display_update(void) {
    int i, j;
    for(i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for(j = 0; j < 128; j++){
            spi_send_recv(font[i*128 + j]);
        }
    }
}

void display_update_player1(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontplayer1[j];
    }
}

void display_update_player2(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontplayer2[j];
    }
}

void display_update_pause(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontpaused[j];
    }
}

void display_update_levelBas(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontlevelBas[j];
    }
}

void display_update_levelAd(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontlevelAd[j];
    }
}

void display_update_open(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontopen[j];
    }
}

void display_update_pong(void){
    int j;
    for(j = 0; j < 512; j++){
        font[j] |= fontpong[j];
    }
}

void drawToScreen(){
    resetScreen();
    drawPlayer(player1);
    drawPlayer(player2);
    drawBall(ball);
    display_update();
}

void spi_init(){
    /* Set up peripheral bus clock */
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;

    /* Clear SPIROV*/
    SPI2STATCLR &= ~0x40;
    /* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60;

    /* Turn on SPI */
    SPI2CONSET = 0x8000;
}

int main(void) {
    spi_init();
    display_init();
    int i = 1;
    int j = 1;
    while(j < 1600){
        display_update_open();
        display_update();
        j++;
    }

    resetScreen();

    while(i){
        display_update_pong();
        display_update();
        if (getbtns() > 0) {
            i = 0;
        }
    };

    startGame();
    while(1){
        int i;
        for(i = 0; i<100000; i++){

        }
        tick();

        drawToScreen();
    }

    for(;;) ;
    return 0;
}
