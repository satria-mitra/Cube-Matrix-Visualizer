
void loop() {
 int z=myLayer;
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        LED(x,y,z,Red);  // you could try other colors if you are worried about the LEDs themselves
      }                  // but any color will work if you are just testing the wiring
    }
    delay(2000);
    clearCube();
}
