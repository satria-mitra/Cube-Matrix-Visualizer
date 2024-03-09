void fireworks() {
   int num = 64;
   int leds = 5;
   int lednum;
   long start;
   int explosionHeight = 7; // Height of fireworks explosion
   int ledx[16];
   int ledy[16];
   int num1 = 0;

   //Initialize the starting position of the fireworks
   for(int x = 2; x < 6; x++) {
     for(int y = 2; y<6; y++) {
       ledx[num1] = x;
       ledy[num1] = y;
       num1++;

     }
    
   }

start = millis();
   while (millis() - start < 200000) { // The duration of the fireworks is 200 seconds
     for (lednum = 0; lednum < leds; lednum++) {
       // Simulate fireworks rising to the top
       for (int z = 0; z <= explosionHeight; z++) {
         LED1(z, ledx[lednum], ledy[lednum], Blue); // Use blue to simulate the rise of fireworks
         delay(100); // Control the rising speed of fireworks
         if (z < explosionHeight) {
           LED1(z, ledx[lednum], ledy[lednum], Black); // Clear the current position before rising to the next height
         }
       }

       // Simulate fireworks exploding on the top floor
       explode(ledx[lednum], ledy[lednum], explosionHeight);

       delay(500); // Wait for a while after the explosion before displaying the next round of fireworks
     }

     // Randomly select a new starting point for the next round of display
     for (int i = 0; i < leds; i++) {
       ledx[i] = random(2, 6);
       ledy[i] = random(2, 6);
     }
   }
}

void explode(int x, int y, int z) {
   int explosionRadius = 1; // The explosion radius is set to 1, which means the explosion will affect adjacent LEDs
   for(int dx = -explosionRadius; dx <= explosionRadius; dx++) {
     for(int dy = -explosionRadius; dy <= explosionRadius; dy++) {
       for(int dz = -explosionRadius; dz <= explosionRadius; dz++) {
         // Check if the new coordinates are within the bounds of the CUBE
         if(x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8 && z + dz >= 0 && z + dz < 8) {
           LED1(z + dz, x + dx, y + dy, Red); // Use red to simulate the explosion effect
         }
       }
     }
   }
   delay(300); // Maintain the explosion effect for a period of time
  
   // Clear explosion effects
   for(int dx = -explosionRadius; dx <= explosionRadius; dx++) {
     for(int dy = -explosionRadius; dy <= explosionRadius; dy++) {
       for(int dz = -explosionRadius; dz <= explosionRadius; dz++) {
         if(x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8 && z + dz >= 0 && z + dz < 8) {
           LED1(z + dz, x + dx, y + dy, Black); // Restore the LED affected by the explosion to black
         }
       }
     }
   }
}