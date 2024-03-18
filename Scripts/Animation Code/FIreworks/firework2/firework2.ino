//Added animation of fireworks falling

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

struct particles {
   int x, y, z;
   int speed; // falling speed
};

void explode(int centerX, int centerY, int centerZ) {
   const int particlesCount = 100; // Define the number of fragments produced by the explosion
   Particle particles[particlesCount];

   // Initialize the position of the fragment as the center point and assign the falling speed
   for (int i = 0; i < particlesCount; i++) {
     particles[i].x = centerX;
     particles[i].y = centerY;
     particles[i].z = centerZ;
     particles[i].speed = random(1, 4); // Fragments fall at different speeds
   }

   // Explosion effect: diffuse fragments
   for (int i = 0; i < particlesCount; i++) {
     // Spread outward from the center
     int dx = random(-2, 3);
     int dy = random(-2, 3);
     int dz = random(-2, 3);

     particles[i].x += dx;
     particles[i].y += dy;
     particles[i].z += dz;

     // Check and correct the position outside the boundary
     particles[i].x = constrain(particles[i].x, 0, 7);
     particles[i].y = constrain(particles[i].y, 0, 7);
     particles[i].z = constrain(particles[i].z, 0, 7);

     LED1(particles[i].z, particles[i].x, particles[i].y, Yellow); // Displays yellow when exploding
   }
   delay(500); // explosion duration

   // Simulate the falling debris
   for (int step = 0; step < 15; step++) {
     for (int i = 0; i < particlesCount; i++) {
       //Clear current position
       LED1(particles[i].z, particles[i].x, particles[i].y, Black);

       // update location
       particles[i].z -= particles[i].speed;
       if (particles[i].z < 0) particles[i].z = 0; // The particles reach the bottom

       // show new location
       LED1(particles[i].z, particles[i].x, particles[i].y, Red); // Displays red when falling
     }
     delay(100); //Control the falling speed
   }

   // Clear all fragments
   for (int i = 0; i < particlesCount; i++) {
     LED1(particles[i].z, particles[i].x, particles[i].y, Black);
   }
}