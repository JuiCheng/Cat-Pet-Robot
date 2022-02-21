const int motorA_1 = 2;
const int motorA_2 = 3;
const int motorB_1 = 4;
const int motorB_2 = 5;
int tail_motor_1=6;
int tail_motor_2=7;
int tail_status=0;
int red_sensor[4][4]={{24,26,28,30},{25,27,29,31},{32,34,36,38},{33,35,37,39}};
int red_en[4]={40,41,42,43};
char msg=' ';

void setup()
{
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(motorA_1,OUTPUT);
  pinMode(motorA_2,OUTPUT);
  
  pinMode(motorB_1,OUTPUT);
  pinMode(motorB_2,OUTPUT);

  pinMode(tail_motor_1,OUTPUT);
  pinMode(tail_motor_2,OUTPUT);
  int i,k;
  for(i=0;i<4;i++){/////紅外線感測器//////
     for(k=0;k<4;k++){
       pinMode(red_sensor[i][k], INPUT);
      } 
    }
  for(i=0;i<4;i++){/////紅外線感測器_智能//////
     pinMode( red_en[i],OUTPUT);
     digitalWrite(red_en[i],HIGH);
    }
}

void loop()
{
   if(Serial1.available()){
    msg= Serial1.read();
    delay(10);  
  }
   Action();
   Tail();
}
void Action(){//執行動作
  switch(msg){
  case 'f' ://前進
    Forward();
  break;
  case'b'://後退
    Back();
  break;
  case 's'://停止
    Stop();
  break;
  case 'r'://右轉
    Right();
  break;
  case 'l'://左轉
    Left();
  break;
  case 'd'://自動
    Avoidance();
  break;
  case 't'://尾巴
   if(tail_status==0){
    tail_status=1;
   }else if(tail_status!=0){
    tail_status=0;  
   }  
   msg=' ';
  break;
  }   
}
void Tail(){//尾巴動作
  if(tail_status==1){
   analogWrite(tail_motor_1,255);
   digitalWrite(tail_motor_2,LOW);
  }else if(tail_status==2){
   analogWrite(tail_motor_2,150);
   digitalWrite(tail_motor_1,LOW);
  }else if(tail_status==0){
   digitalWrite(tail_motor_2,LOW);
   digitalWrite(tail_motor_1,LOW);
  }
  int Time=millis();
  if((Time%3000)==0){
    if(tail_status==1){
      tail_status=2;
    }else if(tail_status==2){
      tail_status=1;
    }  
  }
}
int Detect(int red_ID){//紅外線偵測
  int i;
  int red_Value[4];
  for(i=0;i<4;i++){
    red_Value[i] = digitalRead(red_sensor[red_ID][i]);
  }
//  for(i=0;i<4;i++){
//    Serial.print(" ");
//    Serial.print( red_Value[i]);
//    Serial.print(",");
//    }
//    Serial.println();
//    delay(500);
   if(red_Value[0]||red_Value[1]||red_Value[2]||red_Value[3]){
          return 1;    
    }
}
void Avoidance(){//避障
  int Barrier[4],i;
  int Situation=0;
  for(i=0;i<4;i++){
    Barrier[i]=Detect(i);
    Situation=Situation+Barrier[i];
  }
  switch(Situation){
    case 0:
      Forward();
    break;
    case 1:
     if(Barrier[0]==1){
        Back();
     }else if(Barrier[1]==1){
        Right();
     }else if(Barrier[2]==1){
        Left();
     }else if(Barrier[3]==1){
        Forward();
     }
    break;
    case 2:
    case 3:
      if(Barrier[0]==1){
        Back();
     }else if(Barrier[0]==1 && Barrier[1]==1|| Barrier[0]==1 && Barrier[3]==1 || Barrier[1]==1 && Barrier[3]==1){
        Right();
     }else if(Barrier[0]==1 && Barrier[2]==1||Barrier[2]==1 && Barrier[3]==1){
        Left();
     }
    break;
    case 4:
      Stop();
    break;   
    }
  
}
void Red_Situation(int Situation){
  
  }
int Locate();///藍芽定位

void Forward()//前進
{
   motor_moveA(1,255);
   motor_moveB(1,255);
  
}
void Back()//後退
{
   motor_moveA(2,255);
   motor_moveB(2,255);
}
void Left()//左轉
{
   motor_moveA(2,0);
   motor_moveB(1,255);
}
void Right()//右轉
{
   motor_moveA(1,255);
   motor_moveB(2,0);
}
void Stop(){//暫停
  motor_moveA(0,0);
  motor_moveB(0,0);
}
void motor_moveA(int motor_dir,int motor_speed){
    if(motor_dir==0){
      digitalWrite(motorA_1,LOW);
      digitalWrite(motorA_2,LOW);
    }
    if(motor_dir==1){
       analogWrite(motorA_1,motor_speed);
      digitalWrite(motorA_2,LOW);
    }
    if(motor_dir==2){
      digitalWrite(motorA_1,LOW);
      analogWrite(motorA_2,motor_speed);
    }
}
void motor_moveB(int motor_dir,int motor_speed){
    if(motor_dir==0){
      digitalWrite(motorB_1,LOW);
      digitalWrite(motorB_2,LOW);
    }
    if(motor_dir==1){
      analogWrite(motorB_1,motor_speed);
      digitalWrite(motorB_2,LOW);
    }
    if(motor_dir==2){
      digitalWrite(motorB_1,LOW);
      analogWrite(motorB_2,motor_speed);
    }
}



