
//----------------------------------------------------------------------------------------------------------------------
//ultra1:
#define ECHO_1 3
#define TRIG_1 2
//ultra2:
#define ECHO_2 5
#define TRIG_2 4
//ultra3:
#define ECHO_3 7
#define TRIG_3 6
//CNY
#define CNY_1 A7
#define CNY_2 A6
//motores 
#define motorDER_1 8
#define motorDER_2 7
#define motorIZQ_1 3
#define motorIZQ_2 2
//----------------------------------------------------------------------------------------------------------------------


void setup() {
 //ultra 1
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_1, OUTPUT);

//ultra 2
  pinMode(ECHO_2, INPUT);
  pinMode(TRIG_2, OUTPUT);

//ultra 3
  pinMode(ECHO_3, INPUT);
  pinMode(TRIG_3, OUTPUT);

//motores 
  pinMode(motorDer_1, OUTPUT);
  pinMode(motorDer_2, OUTPUT);
  pinMode(motorIzq_1, OUTPUT);
  pinMode(motorIzq_2, OUTPUT);

//generales 
pinMode()
  Serial.begin(9600);
}

void loop() {
  /*valorSenCNY = senCNY(senCNY1);
  Serial.println(valorSenCNY);*/
  //medirDistancia(TRIG_1,ECHO_1);

//---------------------------------------------------------------------------------
  //ultrasonico 1
  digitalWrite(TRIG_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_1, LOW);
  int tiempo_1 = pulseIn(ECHO_1, HIGH);
  //delay(100);
  int distancia_1 = tiempo_1 / 59;
  if (distancia_1 > 0) {
    Serial.print("\t\tsensor 1: ");
    Serial.print(distancia_1);
  }
  //---------------------------------------------------------------------------------
  //ultrasonico 2
  digitalWrite(TRIG_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_2, LOW);
  int tiempo_2 = pulseIn(ECHO_2, HIGH);
  //delay(100);
  int distancia_2 = tiempo_2 / 59;
  if (distancia_2 > 0) {
    Serial.print("\t\tsensor 2: ");
    Serial.print(distancia_2);
  }
//------------------------------------------------------------------------------------
  //ultrasonico 3
  digitalWrite(TRIG_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_3, LOW);
  int tiempo_3 = pulseIn(ECHO_3, HIGH);
  //delay(100);
  int distancia_3 = tiempo_3 / 59;
  Serial.print("\t\tsensor 3: ");
  Serial.println(distancia_3);

//-------------------------------------------------------------------------------------
  //cny
  int lecturaCNY_1 = analogRead(CNY_1);
  Serial.println(lecturaCNY_1);
}


/*int senCNY(int a){
  int valorSenCNY = analogRead(a);
  return valorSenCNY;
}*/

/*void medirDistancia(int a, int b){
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  int tiempo = pulseIn(b, HIGH);
  //delay(100);
  int distancia = tiempo / 59;
  Serial.println(distancia);
}*/

void irDerecha(){
  digitalWrite();
}

void irIzquierda(){
}

void irAdelante(){
}

void irAtras (){
}
 
void detenerMotores(){
}
