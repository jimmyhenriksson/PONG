#include <pic32mx.h>
#include <stdint.h>
#include "btnsw.h"
#include "fonts.h"

/**
 *
 *
 PONG
 
 Spela mot datorn eller mot en vän.
 Utmana dig själv genom att spela på "Advanced mode"!
 
 Skapat av:
 Jimmy Henriksson & Navid Haghshenas
 *
 */

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
#define BALL_HEIGHT			2
#define BALL_WIDTH			2
#define PLAYER_HEIGHT       8
#define PLAYER_WIDTH		2

int player1_score = 0;
int player2_score = 0;
int rand_number;

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

/**
 Ändrar spelarnas (brädornas) positioner mha knapparna
 */
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

/**
 Ändrar den vänstra brädans (player 1) rörelse, aktiverad endast under bot-mode
 */
void movePlayer_One(){
    if(ball.speedX > 0){
      if(ball.x > 70 && (rand_number % 3) != 0){
        player2.y = ball.y - 4;
      }
      else if(ball.x > 70 && ball.x < 115 && (rand_number % 3) == 0){
        player2.y = ball.y - 4;
      }
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

/**
 Genererar ett slumpmässigt tal baserat på bollens y-position (mellan 1-4)
 */
int generate_random_number(){
  int random = ball.y/3;
  random++;
  return random;
}

/**
 Pausar spelet när switch 1 är uppe
 */
void pauseGame(){
    while (switchOne()) {
        display_update_pause();
        display_update();
    }
}

/**
 Ökar spelarnas poäng genom att tända lamporna (4 per spelare som max)
 inkluderar alla fall som kan uppstå (5x5-1)
 */
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

/**
 meddelar om vem som har vunnit samt startar om spelet mha switch 2
 */
void winner(){
    int i = 1;
    if (player1_score >= 5){
        display_update_player1();
        display_update();
        ball.y++;
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
/**
 ändrar till normal/svår nivå mha switch 2 och switch 3
 */
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

/**
 Får bollen att röra på sig / studsa när den har kommit till övre/nedre sidan av skärmen,
 eller när den har studsat på brädorna.
 Ökar ena spelarens poäng om den andra spelaren missar bollen.
 */
void tick() {
    ball.x += ball.speedX;
    ball.y += ball.speedY;

    if (ball.y <= 0) {
        ball.y = 0;
        ball.speedY *= (-1);
    }

    else if (ball.y >= MAX_Y) {
        ball.y = MAX_Y;
        ball.speedY *= (-1);
    }

    if (ball.x <= 0) {
        if ((ball.y >= (player1.y-2)) && (ball.y <= (player1.y + 8))){
            ball.x = 2;
            ball.speedX *= (-1);
            rand_number = generate_random_number();
        } else{
            rand_number = generate_random_number();
            player2_score++;
            increaseScore();
            scored();
        }
    }

    if (ball.x >= (MAX_X)) {
        if ((ball.y >= (player2.y-2)) && (ball.y <= (player2.y + 8))){
            ball.x = MAX_X-2;
            ball.speedX *= (-1);
            rand_number = generate_random_number();
        } else{
            rand_number = generate_random_number();
            player1_score++;
            increaseScore();
            scored();
        }
    }
    changeLevel();
    pauseGame();
    winner();
    if(switchFour() == 0){
        movePlayer();
    }
    else{
      movePlayer_One();
    }

}

/**
 Uppdaterar bollen och spelarnas positioner varje någon gör ett mål
 */
void scored(){
    ball.x = 64;
    ball.y -= (-2);
    
    player1.x = 0;
    player1.y = 14;
    player1.speedY = 0;

    player2.x = 126;
    player2.y = 14;
    player2.speedY = 0;
}

/**
 Startar om spelet
 */
void startGame(){
    volatile int* portE = (volatile int*) 0xbf886110;
    *portE = *portE & 0x00;		//turn off any leds previously on
    player1_score = 0;
    player2_score = 0;

    ball.x = 64;
    ball.y = 5;
    ball.speedX = 2;
    ball.speedY = 2;

    player1.x = 0;
    player1.y = 14;
    player1.speedY = 0;

    player2.x = 126;
    player2.y = 14;
    player2.speedY = 0;
}

/**
 Används för att rita bollen och spelarna
 */
void update_pixel(int x, int y){
    int row = 0;
    if(y>0) {
        row = y / 8;
    }
    font[row * 128 + x] |= 1 << (y - row * 8);
}

/**
 Ritar spelarna pixel för pixel
 */
void drawPlayer(Player p) {
    int i, j;
    for (i = 0; i < PLAYER_WIDTH; i++){
        for (j = 0; j < PLAYER_HEIGHT; j++){
            update_pixel(p.x + i, p.y + j);
        }
    }
}

/**
 Ritar bollen pixel för pixel
 @param b är bollen (definerad högst upp på klassen)
 */
void drawBall(Ball b) {
    int i, j;
    for (i = 0; i < BALL_WIDTH; i++){
        for (j = 0; j < BALL_HEIGHT; j++){
            update_pixel(b.x + i, b.y + j);
        }
    }
}

/**
 rensar skärmen
 */
void screen_reset(){
    int i;
    for(i = 0; i< (128*4); i++){
        font[i] = 0;
    }
}

/**
 Dröjer mellan olika "calls"
 @param int cyc är hur länge (hur många loops) vi ska dröja mellan två "calls"
 */
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



/**
 *
 FÖLJANDE METODER ÖPPNAR/ÄNDRAR DISPLAYEN
 *
 */
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

/**
 Resettar och visar alla delar av spelet på skärmen
 */
void drawGame(){
    screen_reset();
    drawBall(ball);
    drawPlayer(player1);
    drawPlayer(player2);
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

/**
 Main metod - spelet körs
 */
int main(void) {
    rand_number = generate_random_number();
    spi_init();
    display_init();
    int i = 1;
    int j = 1;
    while(j < 1600){
        display_update_open();
        display_update();
        j++;
    }

    screen_reset();
    while(i){
        display_update_pong();
        display_update();
        if (((getbtns() || buttonOne()) > 0) && (getsw() == 0 || getsw() == 0x8)) {
            i = 0;
        }
    };

    startGame();
    while(1){
        int i;
        for(i = 0; i<100000; i++){

        }
        tick();
        drawGame();
    }
    return 0;
}
