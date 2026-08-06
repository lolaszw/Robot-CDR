//maquinas de estado!!
enum estados { APAGADO,
               ESPERA,
               INICIO,  
               PISO,
               ATAQUE };
estados estadoActual = APAGADO;

enum motores { ADELANTE,
               ATRAS,
               DERECHA,
               IZQUIERDA,
               DETENIDO };
motores estadoMotores = DETENIDO;
//----------------------------------------------------------------------------------------------------------------------
//botones
#define swInicio 11
#define swEstrategia 12

//LEDs
#define ledVerde 10
#define ledRojo A2

//----------------------------------------------------------------------------------------------------------------------
//ultraSonicos:
#define ECHO_1 A3
#define TRIG_1 A4

#define ECHO_2 8
#define TRIG_2 13

#define ECHO_3 A1
#define TRIG_3 A0

//CNYs
#define cnyDer A5
#define cnyIzq A6

//----------------------------------------------------------------------------------------------------------------------
//motores
#define motorDER_1 4
#define motorDER_2 5
#define motorIZQ_1 6
#define motorIZQ_2 7

#define D2 2
//pwm
#define pwmDer 3
#define pwmIzq 9
//----------------------------------------------------------------------------------------------------------------------
//parametros
const int minDistancia = 150;
const int esperaInicio = 5000;
unsigned long ti = 0;
const int buscaEspera = 3000;
const int umbralCNY = 400;

//variables
int distDer = 0;
int distCen = 0;
int distIzq = 0;

int velocidadPWM_DER = 0;
int velocidadPWM_IZQ = 0;
//----------------------------------------------------------------------------------------------------------------------
//velocidades
void motores(){
switch (estadoMotores) {
  case ADELANTE:
    {
      static int velocidadPWM_DER = 0;
      static int velocidadPWM_IZQ = 0;
      digitalWrite(motorDER_1, HIGH);
      digitalWrite(motorDER_2, LOW);
      digitalWrite(motorIZQ_1, LOW);
      digitalWrite(motorIZQ_2, HIGH);
      velocidadPWM_DER += 20;
      velocidadPWM_DER = constrain(velocidadPWM_DER, 0, 255);
      velocidadPWM_IZQ += 20;
      velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, 0, 255);
      analogWrite(pwmDer, velocidadPWM_DER);
      analogWrite(pwmIzq, velocidadPWM_IZQ);
    }
  case ATRAS:
    {
      static int velocidadPWM_DER = 0;
      static int velocidadPWM_IZQ = 0;
      digitalWrite(motorDER_1, LOW);
      digitalWrite(motorDER_2, HIGH);
      digitalWrite(motorIZQ_1, HIGH);
      digitalWrite(motorIZQ_2, LOW);
      velocidadPWM_DER += 20;
      velocidadPWM_DER = constrain(velocidadPWM_DER, 0, 255);
      velocidadPWM_IZQ += 20;
      velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, 0, 255);
      analogWrite(pwmDer, velocidadPWM_DER);
      analogWrite(pwmIzq, velocidadPWM_IZQ);
    }
  case DERECHA:
    {
      static int velocidadPWM_DER = 0;
      static int velocidadPWM_IZQ = 0;
      Serial.println("Va a la derecha");
      digitalWrite(motorDER_1, HIGH);
      digitalWrite(motorDER_2, LOW);
      digitalWrite(motorIZQ_1, HIGH);
      digitalWrite(motorIZQ_2, LOW);
      velocidadPWM_DER += 30;
      velocidadPWM_DER = constrain(velocidadPWM_DER, 0, 255);
      velocidadPWM_IZQ += 10;
      velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, 0, 255);
      analogWrite(pwmDer, velocidadPWM_DER);
      analogWrite(pwmIzq, velocidadPWM_IZQ);
    }
  case IZQUIERDA:
    {
      static int velocidadPWM_DER = 0;
      static int velocidadPWM_IZQ = 0;
      digitalWrite(motorDER_1, LOW);
      digitalWrite(motorDER_2, HIGH);
      digitalWrite(motorIZQ_1, LOW);
      digitalWrite(motorIZQ_2, HIGH);
      velocidadPWM_DER += 10;
      velocidadPWM_DER = constrain(velocidadPWM_DER, 0, 255);
      velocidadPWM_IZQ += 30;
      velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, 0, 255);
      analogWrite(pwmDer, velocidadPWM_DER);
      analogWrite(pwmIzq, velocidadPWM_IZQ);
    }
  case DETENIDO:
    {
      digitalWrite(motorDER_1, LOW);
      digitalWrite(motorDER_2, LOW);
      digitalWrite(motorIZQ_1, LOW);
      digitalWrite(motorIZQ_2, LOW);
    }
}}

//funciones

void atacarEnemigo() {  //si el enemigo esta cerca, lo ataca
  if (distCen < minDistancia) {
    //irAdelante();
    estadoMotores = ADELANTE;
    motores();
  }
  if (distDer < minDistancia) {
    //irDerecha();
    estadoMotores = DERECHA;
    motores();
  }
  if (distIzq < minDistancia) {
    //irIzquierda();
    estadoMotores = IZQUIERDA;
    motores();
  }
}

int senCNY(int a) {
  int valorSenCNY = analogRead(a);
  return valorSenCNY;
}

void irDerecha() {  //gira a la derecha
  Serial.println("Va a la derecha");
  digitalWrite(motorDER_1, HIGH);
  digitalWrite(motorDER_2, LOW);
  digitalWrite(motorIZQ_1, HIGH);
  digitalWrite(motorIZQ_2, LOW);
  for (int velocidadPWM = 0; velocidadPWM <= 230; velocidadPWM += 5) {
    analogWrite(pwmDer, velocidadPWM);
    analogWrite(pwmIzq, velocidadPWM);
  }
}

void irIzquierda() {  //gira a la izquierda
  digitalWrite(motorDER_1, LOW);
  digitalWrite(motorDER_2, HIGH);
  digitalWrite(motorIZQ_1, LOW);
  digitalWrite(motorIZQ_2, HIGH);
  for (int velocidadPWM = 0; velocidadPWM <= 230; velocidadPWM += 5) {
    analogWrite(pwmDer, velocidadPWM);
    analogWrite(pwmIzq, velocidadPWM);
  }
}

void irAdelante() {  //va adelante
  digitalWrite(motorDER_1, HIGH);
  digitalWrite(motorDER_2, LOW);
  digitalWrite(motorIZQ_1, LOW);
  digitalWrite(motorIZQ_2, HIGH);
  for (int velocidadPWM = 0; velocidadPWM <= 230; velocidadPWM += 5) {
    analogWrite(pwmDer, velocidadPWM);
    analogWrite(pwmIzq, velocidadPWM);
  }
}

void irAtras() {  //va atras
  static int velocidadPWM = 0;
  digitalWrite(motorDER_1, LOW);
  digitalWrite(motorDER_2, HIGH);
  digitalWrite(motorIZQ_1, HIGH);
  digitalWrite(motorIZQ_2, LOW);
  velocidadPWM += 20;
  velocidadPWM = constrain(velocidadPWM, 0, 255);
  analogWrite(pwmDer, velocidadPWM);
  analogWrite(pwmIzq, velocidadPWM);
}
}

void detenerMotores() {  //motores detenidos
  digitalWrite(motorDER_1, LOW);
  digitalWrite(motorDER_2, LOW);
  digitalWrite(motorIZQ_1, LOW);
  digitalWrite(motorIZQ_2, LOW);
}

float medirDistancia(int a, int b) {  //para medir la distancia
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  int tiempo = pulseIn(b, HIGH, 30000);
  if (tiempo == 0) {
    return 999;
  }
  //delay(100);
  return tiempo / 59;
}

bool intervalo(long tiempoi, int t) {  //para ver si x intervalo se cumplio o no

  if (millis() >= tiempoi + t) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  //ultraSonicos
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_1, OUTPUT);

  pinMode(ECHO_2, INPUT);
  pinMode(TRIG_2, OUTPUT);

  pinMode(ECHO_3, INPUT);
  pinMode(TRIG_3, OUTPUT);

  //motores
  pinMode(motorDER_1, OUTPUT);
  pinMode(motorDER_2, OUTPUT);
  pinMode(motorIZQ_1, OUTPUT);
  pinMode(motorIZQ_2, OUTPUT);

  pinMode(D2, OUTPUT);

  //botones
  pinMode(swInicio, INPUT_PULLUP);
  pinMode(swEstrategia, INPUT_PULLUP);

  //LEDs
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  switch (estadoActual) {
    case APAGADO:
      {
        digitalWrite(ledVerde, LOW);
        //detenerMotores();
        estadoMotores = DETENIDO;
        motores();

        if (digitalRead(swInicio) == LOW) {  //si se presiona el boton empieza a esperar
          estadoActual = ESPERA;
          ti = millis();
        }
        break;
      }
    case ESPERA:
      {
        estadoMotores = DETENIDO;
        if (intervalo(ti, esperaInicio)) {  // se fija si pasaron 5 seg
          estadoActual = INICIO;
          motores();
          ti = millis();
        }
        break;
      }
    case INICIO:
      {
        digitalWrite(D2, HIGH);
        digitalWrite(ledVerde, HIGH);
        //irDerecha();  //gira por dos segundos antes de empezar a leer el piso (estrategia de inicio)
        estadoMotores = DERECHA;
        motores();
        if (intervalo(ti, 2000)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }
    case PISO:
      {
        if (senCNY(cnyDer) > umbralCNY && senCNY(cnyIzq) > umbralCNY) {  //si el piso lee negro pasa a ver si hay enemigos
          estadoActual = ATAQUE;
        } else if (senCNY(cnyDer) > umbralCNY && senCNY(cnyIzq) < umbralCNY) {  //si el lado izquierdo ve blanco, gira
          //irAtras();
          estadoMotores = ATRAS;
          motores();
          if (intervalo(ti, 2000)) {
            //irIzquierda();
            estadoMotores = IZQUIERDA;
            motores();
          }
        } else if (senCNY(cnyDer) < umbralCNY && senCNY(cnyIzq) > umbralCNY) {  //si el lado derecho ve blanco, gira
          //irAtras();
          estadoMotores = ATRAS;
          motores();
          if (intervalo(ti, 2000)) {
            //irDerecha();
            estadoMotores = DERECHA;
            motores();
          }
        } else {  //si lee blanco completamente
          //irAtras();
          estadoMotores = ATRAS;
          motores();
        }
      }
      break;
    case ATAQUE:
      {
        distDer = medirDistancia(ECHO_1, TRIG_1);
        distCen = medirDistancia(ECHO_2, TRIG_2);
        distIzq = medirDistancia(ECHO_3, TRIG_3);
        //irDerecha();
        estadoMotores = DERECHA;
        motores();
        atacarEnemigo();
        if (senCNY(cnyDer) < umbralCNY || senCNY(cnyIzq) < umbralCNY) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }
  }
}
