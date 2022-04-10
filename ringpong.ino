/****************************************************************************
 * RING PONG V1.0
 * 
 * MADE BY AERODYNAMICS
 * 
 * JULY 17, 2017
 * 
 * FEEL FREE TO USE THE CODE
 * 
 * 
 ***************************************************************************/

#include <Adafruit_NeoPixel.h> //used to control the ring

#define RING_PIN 6      //control PIN of the neopixel ring
#define button0_PIN 2   //control PIN for the first button
#define button1_PIN 3   //control PIN for the 2nd button
#define num_LED 24      //number of LED on the ring


Adafruit_NeoPixel ring = Adafruit_NeoPixel(num_LED, RING_PIN, NEO_GRB + NEO_KHZ800);

uint32_t color; 

bool game_ON = 0;          //state if the game is ON or not
bool player = 0;           //select the current player
int point_player0 = 0;     //number of player 0 points
int point_player1 = 0;     //number of player 1 points
bool dir = 0;              //select the direction of the game
int k=4;                   //position of the ball
int game_speed = 100;      //speed of the ball
int normal = 100;          //normal speed 
unsigned long timer;       //timer used to accelerate the game 

/**************************************************************************
 * SETUP
 *************************************************************************/
 
void setup() {
  
  pinMode(button0_PIN, INPUT_PULLUP); //Using the pullup option
  pinMode(button1_PIN, INPUT_PULLUP);
    
  ring.begin(); //switch on the ring

  //Creation of a random color for the game
  randomSeed(analogRead(A5));   
  color = ring.Color(random(20, 100), random(20, 100), random(20, 100));
  
  ring_blink(color, 500, 2);  //starting "animation"
}


/**************************************************************************
 * LOOP
 *************************************************************************/
 
void loop() {

//Initializing game if player 0 is selected
  
  if(!game_ON && player == 0){
    k=4;
 
//Light on the border (green), the ball (blueish) and the points (red)
    ring.setPixelColor(3,0,100,0);
    ring.setPixelColor(ring.numPixels()-4,0,100,0);
    ring.setPixelColor(4,0,50,75);
    for(int i=0;i<point_player0;i++){
      ring.setPixelColor(i,100,0,0);
    }
    for(int i=ring.numPixels()-1;i>ring.numPixels()-point_player1-1;i--){
      ring.setPixelColor(i,100,0,0);
    }
    ring.show();

//Actions if player 0 engage the game
    if(!digitalRead(button0_PIN)){
      game_ON = 1;
      player = !player;
      normal = 100;
      game_speed = normal;
      delay(1000);
      timer = millis();
    }
    
  }
//Initializing game if player 1 is selected
  else if(!game_ON && player == 1){
    k=ring.numPixels()-5;
    
//Light on the border (green), the ball (blueish) and the points (red)
    ring.setPixelColor(3,0,100,0);
    ring.setPixelColor(ring.numPixels()-4,0,100,0);
    ring.setPixelColor(ring.numPixels()-5,0,50,75);
    for(int i=0;i<point_player0;i++){
      ring.setPixelColor(i,100,0,0);
    }
    for(int i=ring.numPixels()-1;i>ring.numPixels()-point_player1-1;i--){
      ring.setPixelColor(i,100,0,0);
    }
    ring.show();

//Actions if player 0 engage the game
    if(!digitalRead(button1_PIN)){
      game_ON = 1;
      player = !player;
      normal = 100;
      game_speed = normal;
      delay(1000);
      timer = millis();
    }
  }

// GAME IS ON  
  else{

//Update the display
    comete(k,0,50,75); 
    
    ring.setPixelColor(3,0,100,0);
    ring.setPixelColor(ring.numPixels()-4,0,100,0);

//IF PLAYER 1 is playing
    if(player == 1){

//When Player 1 is pushing the button
      if(!digitalRead(button1_PIN)){ 
//At the best moment      
        if(k == ring.numPixels()-5){                         
          dir = !dir; //direction inversion
          game_speed = normal*0.5;// super speed
          player = !player; //player switch
        }
        
//At the good moment
        else if(k > ring.numPixels()-8 && k < ring.numPixels()-5){ 
          dir = !dir; //direction inversion
          game_speed = normal;  //normal speed
          player = !player; //player switch
        }
        
//If he pushes at a wrong moment        
        else{ 
          game_ON = 0;               //Game OFF
          point_player0 ++;          // + 1pt for other player
          player = !player;          // player switch
          game_speed = normal*0.75;  //engagement speed
          ring_blink(color,150,2);   //ring blink
          show_none(); //screen off
        }
      }
    }

//IF PLAYER 0 is playing
    if(player == 0){ //Si c est au joureur 1

//When Player 0 is pushing the button
      if(!digitalRead(button0_PIN)){
//At the best moment
        if(k == 4){ 
          dir = !dir; //direction inversion
          game_speed = normal*0.5;// super speed
          player = !player; //player switch
        }

//At the good moment
        else if(k < 8 && k > 4){ 
          dir = !dir; //direction inversion
          game_speed = normal;// Normal speed
          player = !player; //player switch
        }

//If he pushes at a wrong moment      
        else{ 
          game_ON = 0;              //Game OFF
          point_player1 ++;         // + 1pt for other player
          player = !player;         // player switch
          game_speed = normal*0.75; //engagement speed
          ring_blink(color,150,2);  //ring blink
          show_none();              //screen off
        }
      }
    
    }

 //MOVING THE BALL 
    if(!dir){
      k++;
      delay(game_speed);
      
//Game fail for player 1 if he lets the ball going too far
      if(k>ring.numPixels()-5){
        game_ON = 0; 
        point_player0 ++; 
        player = !player; 
        k=4;
        ring_blink(color,150,2);
        show_none();
      }
    }
    else {
      k--;
      delay(game_speed);

//Game fail for player 1 if he lets the ball going too far
      if(k<4){
        game_ON = 0; //fin de la partie
        point_player1 ++; //point en plus pour le joeur 1;
        player = !player;// changement de joueur ;
        k=ring.numPixels()-5;
        ring_blink(color,150,2);
        show_none();
      }
    }
  }

// ENDING THE GAME IF A PLAYER HAS MORE THAN 3 POINTS
  if (point_player0>3 || point_player1>3){
    ring_blink(color,150,5);
    game_ON = 0;
    player = 0;
    point_player0=0;
    point_player1=0;
    game_speed = 100;
    k=4;
    dir = 0;
 }

//ACCELERATING THE GAME 
  if(millis()-timer > 5000){
    normal = normal*0.8;
    timer=millis();
  }
}



/**************************************************************************
 * FUNCTIONS
 *************************************************************************/
void show_all(uint32_t color) {
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, color);
  }
  ring.show();
}

void show_none() {
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, 0, 0, 0);
  }
  ring.show();
}

void ring_blink(uint32_t color, int wait, int num_repet){
  for(int i = 0 ; i<num_repet ; i++){
    show_all(color);
    delay(wait);
    show_none();
    delay(wait);
  }
}

void comete(int pos, byte r, byte g, byte b){
  for(int i=0; i < ring.numPixels(); i++){
    ring.setPixelColor(i,int32bittoRGB(ring.getPixelColor(i),'r')/3,int32bittoRGB(ring.getPixelColor(i),'g')/3,int32bittoRGB(ring.getPixelColor(i),'b')/3);
    ring.setPixelColor(pos,r,g,b);
    }
  ring.show();
}

uint8_t int32bittoRGB ( uint32_t color, char value )
{
  switch ( value ) {
    case 'r': return (uint8_t)(color >> 16);
    case 'g': return (uint8_t)(color >>  8);
    case 'b': return (uint8_t)(color >>  0);
    default:  return 0;
  }
}
