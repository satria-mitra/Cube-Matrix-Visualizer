void num0(int mycolor){
  int z,y,x;
  for(x=7;x>-1;x--){
    for(y=2;y<6;y++){
      LED1(0, y, x, mycolor);
      LED1(7, y, x, mycolor);
    }
    for(z=1;z<7;z++){
      LED1(z, 1, x, mycolor);
      LED1(z, 6, x, mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    //cube.show();
    
  }
  
  delay(numstay);
}
//////////////////////////////
void num1(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=1;y1<7;y1++){
      LED1(0, y1, x, mycolor);
    }
    for(int z1=1;z1<8;z1++){
      LED1(z1, 3, x, mycolor);
      LED1(z1, 4, x, mycolor);
    }
    LED1(5, 1, x, mycolor);
    LED1(6, 2, x, mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////////

void num2(int mycolor){
  int z2[4]={1,2,3,4};
  int y2[4]={2,3,4,5};
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(0, y1, x, mycolor);
    }
    for(int i=0;i<4;i++){
      LED1(z2[i],y2[i],x,mycolor);
    }
    for(int z3=5;z3<7;z3++){
      LED1(z3,2,x,mycolor);
      LED1(z3,6,x,mycolor);
    }
    for(int y2=3;y2<6;y2++){
      LED1(7,y2,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}

////////////////////////////

void num3(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<3;z1++){
      LED1(z1,6,x,mycolor);
    }
    for(int z2=5;z2<7;z2++){
      LED1(z2,6,x,mycolor);
    }
    for(int z3=3;z3<5;z3++){
      LED1(z3,5,x,mycolor);
    }
    LED1(1,2,x,mycolor);
    LED1(6,2,x,mycolor);
    LED1(3,4,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  
  }
  delay(numstay);
}

//////////////////////////////////////////////

void num4(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,4,x,mycolor);
    }
    for(int z2=3;z2<8;z2++){
      LED1(z2,1,x,mycolor);
    }
    for(int y1=1;y1<7;y1++){
      LED1(3,y1,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}

///////////////////////////////////

void num5(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(7,y1,x,mycolor);
    }
    for(int y2=2;y2<6;y2++){
      LED1(4,y2,x,mycolor);
      LED1(0,y2,x,mycolor);
    }
    for(int z1=5;z1<7;z1++){
      LED1(z1,2,x,mycolor);
    }
    for(int z2=1;z2<4;z2++){
      LED1(z2,6,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);

}
////////////////////

void num6(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(0,y1,x,mycolor);
      LED1(4,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<4;z1++){
      LED1(z1,2,x,mycolor);
    }
    for(int z2=1;z2<7;z2++){
      LED1(z2,2,x,mycolor);
    }
    for(int z2=0;z2<5;z2++){
      LED1(z2,6,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    //cube.show();
  }
  delay(numstay);
}

//////////////
void num7(int mycolor){
  int z1[7]={0,0,0,0,2,4,6};
  int z2[7]={0,0,0,2,4,6,8};
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(7,y1,x,mycolor);
    }
    for(int y2=3;y2<7;y2++){
      for(int z=z1[y2];z<z2[y2];z++){
        LED1(z,y2,x,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    //cube.show();
  }
  delay(numstay);
}
/////////////////
void num8(int mycolor){
  int z[6]={6,6,5,5,3,3};
  int y[6]={6,1,5,2,5,2,};
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<3;z1++){
      LED1(z1,1,x,mycolor);
      LED1(z1,6,x,mycolor);
    }
    for(int y2=3;y2<5;y2++){
      LED1(4,y2,x,mycolor);
    }
    for(int i=0;i<6;i++){
      LED1(z[i],y[i],x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  //cube.show();
  delay(numstay);
}
/////////////
void num9(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(3,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<7;z1++){
      LED1(z1,6,x,mycolor);
    }
    for(int z2=4;z2<7;z2++){
      LED1(z2,2,x,mycolor);
    }
    LED1(1,2,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  //cube.show();
  delay(numstay);
}
/////////////
void T(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=1;y1<8;y1++){
      LED1(7,y1,x,mycolor);
    }
    for(int z1=0;z1<7;z1++){
      LED1(z1,4,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////
void U(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<6;y1++){
      LED1(0,y1,x,mycolor);
    }
    for(int z1=1;z1<8;z1++){
      LED1(z1,1,x,mycolor);
      LED1(z1,6,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////
void M(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,1,x,mycolor);
      LED1(z1,7,x,mycolor);
    }
    for(int z2=3;z2<5;z2++){
      LED1(z2,4,x,Red);
    }
    LED1(6,2,x,mycolor);
    LED1(6,6,x,mycolor);
    LED1(5,3,x,mycolor);
    LED1(5,5,x,mycolor);

    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////////////////
void P(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,1,x,mycolor);
    }
    for(int y1=1;y1<5;y1++){
      LED1(2,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    LED1(6,5,x,mycolor);
    LED1(3,5,x,mycolor);
    LED1(4,6,x,mycolor);
    LED1(5,6,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////////
void H(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,2,x,mycolor);
      LED1(z1,6,x,mycolor);
    }
    for(int y1=2;y1<7;y1++){
      LED1(4,y1,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  
  }
  delay(numstay);
}
/////////////////////////
void C(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int x1=2;x1<6;x1++){
      LED1(x1,1,x,mycolor);
    }
    LED1(1,2,x,mycolor);
    LED1(1,6,x,mycolor);
    LED1(6,2,x,mycolor);
    LED1(6,6,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////////
void colon(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=5;z1<7;z1++){
      for(int y1=3;y1<5;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=1;z1<3;z1++){
      for(int y1=3;y1<5;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
   
  }
  delay(numstay);
}
//////////////////////////////
void percentage(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=5;z1<7;z1++){
      for(int y1=1;y1<3;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=1;z1<3;z1++){
      for(int y1=5;y1<7;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=2;z1<4;z1++){
      LED1(z1,3,x,mycolor);
    }
    for(int z1=4;z1<6;z1++){
      LED1(z1,4,x,mycolor);
    }
    LED1(0,1,x,mycolor);
    LED1(1,2,x,mycolor);
    LED1(6,5,x,mycolor);
    LED1(7,6,x,mycolor);

    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  
  }
  delay(numstay);
}
////////////////////
void exclamation(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=3;z1<8;z1++){
      LED1(z1,3,x,mycolor);
      LED1(z1,4,x,mycolor);
    }
    for(int z1=0;z1<2;z1++){
      for(int y1=3;y1<5;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}
/////////////////
void degree(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=6;z1<8;z1++){
      for(int y1=0;y1<2;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=1;z1<6;z1++){
      LED1(z1,2,x,mycolor);
    }
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(6,y1,x,mycolor);
    }
    LED1(1,6,x,mycolor);
    LED1(5,6,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}
///////////////////