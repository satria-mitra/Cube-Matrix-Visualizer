void Humdata(float hum){
  String humdata=String(hum,0);
  int numlength= humdata.length();
  Serial.println(humdata);
  Serial.println(numlength);
  H(Red);
  U(Red);
  M(Red);
  for(int i=0;i<humdata.length();i++){
    char Digit= humdata.charAt(i);
    Serial.println(Digit);
    switch(Digit){
      case '0':
      num0(Red);
      break;
      case '1':
      num1(Red);
      break;
      case '2':
      num2(Red);
      break;
      case '3':
      num3(Red);
      break;
      case '4':
      num4(Red);
      break;
      case '5':
      num5(Red);
      break;
      case '6':
      num6(Red);
      break;
      case '7':
      num7(Red);
      break;
      case '8':
      num8(Red);
      break;
      case '9':
      num9(Red);
      break;

    }
    delay(450);
  }
  delay(500);
  clearCube();
}



void rain(float hum){
  int Humlevel;
  String humdata=String(hum,0);
  int numlength= humdata.length();
  char Digit= humdata.charAt(0);
  switch(Digit){
    case '1':
      Humlevel=0;
      break;
    case '2':
      Humlevel=1;
      break;
    case '3':
      Humlevel=2;
      break;
    case '4':
      Humlevel=3;
      break;
    case '5':
      Humlevel=4;
      break;
    case '6':
      Humlevel=5;
      break;
    case '7':
      Humlevel=6;
      break;
    case '8':
      Humlevel=7;
      break;
    case '9':
      Humlevel=8;
      break;
  }
  int num=64;
  int num2=0;

  int x[num], y[num], z[num], zlevel[num], leds = 10,lednum;
  int xold[num], yold[num], zold[num];
  int ranled[leds];

    for(int x1=0;x1<8;x1++){
      for(int y1=0;y1<8;y1++){
        x[num2] = x1;
        y[num2] = y1;
        z[num2] = random(7,26);
        zlevel[num2] = 0;
        num2++;
      }
    }
  for(int i = 0;i<leds;i++){
    ranled[i]=random(0,63);
  }
    int numnnnn=0;
  start = millis();
  while (millis() - start < 10000){
    for(lednum = 0;lednum<leds;lednum++){
      // if(z[ranled[lednum]]>0){
        
      // }
      LED1(z[ranled[lednum]], x[ranled[lednum]], y[ranled[lednum]], Blue);
    }
    cube.show();
    for (lednum = 0; lednum < leds; lednum++){
      xold[ranled[lednum]] = x[ranled[lednum]];
      yold[ranled[lednum]] = y[ranled[lednum]];
      zold[ranled[lednum]] = z[ranled[lednum]];
    }
    
    delay(120);
    for (lednum = 0; lednum < leds; lednum++) {
      if(z[ranled[lednum]]!=zlevel[ranled[lednum]]){
        z[ranled[lednum]] = z[ranled[lednum]] -1;
        LED1(zold[ranled[lednum]], xold[ranled[lednum]], yold[ranled[lednum]], Black);
        //Serial.println(z[ranled[lednum]]);
      }
      if (z[ranled[lednum]] == zlevel[ranled[lednum]]) {
        z[ranled[lednum]] = random(7,26);
        LED1(zlevel[ranled[lednum]], x[ranled[lednum]], y[ranled[lednum]], Blue);
        if(zlevel[ranled[lednum]] != Humlevel){
          zlevel[ranled[lednum]]++;
          }
        
        //Serial.println(zlevel[ranled[lednum]]);
        ranled[lednum]=random(0,63);
      }
      
      
    }
    cube.show();
  }
}