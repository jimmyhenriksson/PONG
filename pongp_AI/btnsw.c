#include <stdint.h>
#include <pic32mx.h>
#include "btnsw.h"

/*
 Button 1-4
 */
int getbtns( void ){
  int btn = PORTD >> 5;
  btn &= 0x7;
  return btn;
}


int buttonOne(){
  int btn = PORTF;

  if(btn & 0x2){
    return 1;
  }
  else return 0;
}

int buttonTwo(){
  int btns = getbtns();

  if((btns & 0x1)){
    return 1;
  }
  else return 0;
}

int buttonThree(){
  int btns = getbtns();

  if((btns & 0x2) == 2){
    return 1;
  }
  else return 0;
}

int buttonFour(){
  int btns = getbtns();

  if((btns & 0x4) == 4){
    return 1;
  }
  else return 0;
}
/*
 Switch 1-4
 */
int getsw( void ){
    int sw14 = PORTD & 0xf00;
    sw14 = sw14 >> 8;
    return sw14;
}

int switchOne(){
    int sw = getsw();
    if(sw & 0x1){
        return 1;
    }
    else return 0;
}

int switchTwo(){
    int sw = getsw();
    if(sw & 0x2){
        return 1;
    }
    else return 0;
}

int switchThree(){
    int sw = getsw();
    if(sw & 0x4){
        return 1;
    }
    else return 0;
}

int switchFour(){
    int sw = getsw();
    if(sw & 0x8){
        return 1;
    }
    else return 0;
}
