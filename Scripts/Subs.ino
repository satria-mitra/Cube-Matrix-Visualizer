// these next 3 subroutines were used by

void redON(int x, int y, int z) {
  LED(x,y,z,Red);
}

void greenON(int x, int y, int z) {
  LED(x,y,z,Green);
}

void blueON(int x, int y, int z) {
  LED(x,y,z,Blue);
}

//  This routine generates 42 different colors around the color wheel using our 3 bit BAM
uint32_t GetColor(int thecolor) {
  myred = 36*pgm_read_byte_near(&color[thecolor][0]);
  mygreen = 36* pgm_read_byte_near(&color[thecolor][1]);
  myblue = 36*pgm_read_byte_near(&color[thecolor][2]);
  return cube.Color(myred,mygreen, myblue);
}

// Sets intensity of all three colors of one LED to generate a specified color
void LED(int x, int y, int z, unsigned int mycolor){
  if (z%2) cube.drawPixel((z+1)*5-x-1, 4-y, mycolor);
  else cube.drawPixel(x+(z*5),y, mycolor);
}


void clearCube(){
cube.fill(Black);
}

// show cube for a multiples of 10 ms before clearing it.
void showCube(int mytime) {
  delay(20 * mytime);
  clearCube();
}

void pause() {  // pause between animations
  clearCube();
  delay(500);
}

// get a random color from color definitions
uint32_t randomColor() {
  uint32_t mycolor;
  switch (1 + random(7)) {
    case 1:
      mycolor = Red;
      return mycolor;
      break;
    case 2:
      mycolor = Green;
      return mycolor;
      break;
    case 3:
      mycolor = Blue;
      return mycolor;
      break;
    case 4:
      mycolor = Yellow;
      return mycolor;
      break;
    case 5:
      mycolor = Aqua;
      return mycolor;
      break;
    case 6:
      mycolor = Purple;
      return mycolor;
      break;
    case 7:
      mycolor = White;
      return mycolor;
      break;
  }
}

// *******************************  Start of Refresh Timer Interrupt ************************
ISR(TIMER1_COMPA_vect) {
  cube.show();
}
