enum estados { apagado,
               espera,
               piso,
               ataque };
estados estadoActual = apagado;
//----------------------------------------------------------------------------------------------------------------------
//botones
#define swInicio 11
#define swEstrategia 12

//LEDs
#define ledVerde 10
#define ledRojo A2

//----------------------------------------------------------------------------------------------------------------------
//ultraSonicos:
#define ECHO_1 3
#define TRIG_1 2

#define ECHO_2 4
#define TRIG_2 13

#define ECHO_3 A1
#define TRIG_3 A0

//CNYs
#define cnyDer A5
#define cnyIzq A6

//----------------------------------------------------------------------------------------------------------------------
//motores
#define motorDER_1 2
#define motorDER_2 5
#define motorIZQ_1 6
#define motorIZQ_2 7

//pwm
#define pwmDer 3
#define pwmIzq 9
//----------------------------------------------------------------------------------------------------------------------
//parametros
const int minDistancia = 150;
const int esperaInicio = 5000;
unsigned long ti = 0;
const int buscaEspera = 3000;
//variables
int distDer = 0;
int distCen = 0;
int distIzq = 0;


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

  //botones
  pinMode(swInicio, INPUT_PULLUP);
  pinMode(swEstrategia, INPUT_PULLUP);

  //LEDs
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
}

void loop() {
  switch (estadoActual) {
    case apagado:
      {
        detenerMotores();

        if (digitalRead(swInicio) == LOW) {  //si se presiona el boton empieza a esperar
          estadoActual = espera;
          ti = millis();
        }
        break;
      }
    case espera:
      {
        detenerMotores();
        if (intervalo(ti, esperaInicio)) {  // se fija si pasaron 5 seg
          estadoActual = piso;
        }
        break;
      }
    case piso:
      {
        digitalWrite(ledVerde, HIGH);
        ti = millis();
        irDerecha();
        if (intervalo(ti, 2000)) {
          if (analogRead(cnyDer) > 0 && analogRead(cnyIzq) > 0) {  //si el piso lee negro pasa a ver si hay enemigos
            estadoActual = ataque;
          } else if (analogRead(cnyDer) > 0 && analogRead(cnyIzq) < 1) {  //si el lado izquierdo ve blanco, gira
            irAtras();
            irIzquierda();
          } else if (analogRead(cnyDer) < 1 && analogRead(cnyIzq) > 0) {  //si el lado derecho ve blanco, gira
            irAtras();
            irDerecha();
          } else {  //si lee blanco completamente
            irAtras();
          }
        }
        break;
      }
    case ataque:
      {
        digitalWrite(ledVerde, HIGH);
        distDer = medirDistancia(ECHO_1, TRIG_1);
        distCen = medirDistancia(ECHO_2, TRIG_2);
        distIzq = medirDistancia(ECHO_3, TRIG_3);
        ti = millis();
        irDerecha();
        intervalo(ti, buscaEspera);
        atacarEnemigo();
        if (senCNY(cnyDer) < 1 || senCNY(cnyIzq) < 1) {
          estadoActual = piso;
        }
        break;
      }
  }
}



void atacarEnemigo() {  //
  if (distCen < minDistancia) {
    irAdelante();
  }
  if (distDer < minDistancia) {
    irDerecha();
  }
  if (distIzq < minDistancia) {
    irIzquierda();
  }
}

int senCNY(int a) {
  bool valorSenCNY = map(analogRead(a), 0, 1023, 0, 1);
  return valorSenCNY;
}

void irDerecha() {  //gira a la derecha
  digitalWrite(motorDER_1, HIGH);
  digitalWrite(motorDER_2, LOW);
  digitalWrite(motorIZQ_1, HIGH);
  digitalWrite(motorIZQ_2, LOW);
}

void irIzquierda() {  //gira a la izquierda
  digitalWrite(motorDER_1, LOW);
  digitalWrite(motorDER_2, HIGH);
  digitalWrite(motorIZQ_1, LOW);
  digitalWrite(motorIZQ_2, HIGH);
}

void irAdelante() {  //va adelante
  digitalWrite(motorDER_1, HIGH);
  digitalWrite(motorDER_2, LOW);
  digitalWrite(motorIZQ_1, LOW);
  digitalWrite(motorIZQ_2, HIGH);
}

void irAtras() {  //va atras
  digitalWrite(motorDER_1, LOW);
  digitalWrite(motorDER_2, HIGH);
  digitalWrite(motorIZQ_1, HIGH);
  digitalWrite(motorIZQ_2, LOW);
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