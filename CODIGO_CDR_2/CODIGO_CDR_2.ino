//maquinas de estado!!
enum estados { APAGADO,
               ANALIZAR,
               ESPERA,
               INICIO,
               PISO,
               ATAQUE };
estados estadoActual = APAGADO;

enum movMotores { ADELANTE,
                  ATRAS,
                  DERECHA,
                  IZQUIERDA,
                  ARCO_IZQ,
                  ARCO_DER,
                  DETENIDO };
movMotores estadoMotores = DETENIDO;

enum estrategias { INIT = 0b00000000,         //init
                   G_IZQ = 0b11000000,        //giro izq
                   G_IZQ2 = 0b11010000,       //giro izq
                   G_DER = 0b10010000,        //giro der
                   G_DER2 = 0b10000000,       //giro der
                   A_IZQ = 0b00110000,        //arco izq
                   A_IZQ2 = 0b01110000,       //arco izq
                   A_DER = 0b00100000,        //arco der
                   A_DER2 = 0b01100000,       //arco der
                   G_IZQ_A_IZQ = 0b11110000,  //giro izq, arco izq
                   G_IZQ_A_DER = 0b11100000,  //giro izq, arco der
                   G_DER_A_IZQ = 0b10110000,  //giro der, arco izq
                   G_DER_A_DER = 0b10100000,  //giro der, arco der
};
estrategias estrategiaActual = 0b00000000;
//----------------------------------------------------------------------------------------------------------------------
//botones
#define swInicio 11
#define DIP A2

//----------------------------------------------------------------------------------------------------------------------
//ultraSonicos:
#define ECHO_1 A3
#define TRIG_1 A4

#define ECHO_2 8
#define TRIG_2 13

#define ECHO_3 A1
#define TRIG_3 A0

#define ECHO_4 A1
#define TRIG_4 A0

#define ECHO_5 A1
#define TRIG_5 A0

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
//--------------------------------------------MAQUINAS DE ESTADO--------------------------------------------------------------------------
//velocidades
void motores() {
  static int velocidadPWM_DER = 0;
  static int velocidadPWM_IZQ = 0;
  switch (estadoMotores) {
    case ADELANTE:
      {
        velocidadPWM_DER += 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -255, 255);
        velocidadPWM_IZQ += 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -255, 255);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case ATRAS:
      {
        velocidadPWM_DER -= 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -255, 255);
        velocidadPWM_IZQ -= 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -255, 255);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case DERECHA:
      {
        velocidadPWM_DER -= 30;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -255, 255);
        velocidadPWM_IZQ += 10;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -255, 255);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case IZQUIERDA:
      {
        velocidadPWM_DER += 10;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -255, 255);
        velocidadPWM_IZQ -= 30;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -255, 255);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case ARCO_IZQ:
      {
        velocidadPWM_DER += 8;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -180, 180);
        velocidadPWM_IZQ -= 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -255, 255);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case ARCO_DER:
      {
        velocidadPWM_DER -= 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -255, 255);
        velocidadPWM_IZQ += 8;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -180, 180);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case DETENIDO:
      {
        digitalWrite(motorDER_1, LOW);
        digitalWrite(motorDER_2, LOW);
        digitalWrite(motorIZQ_1, LOW);
        digitalWrite(motorIZQ_2, LOW);
        break;
      }
  }
}

void estrategiaAnalizada() {
  
  estrategiaActual = dipAnalizado;
  switch (estrategiaActual) {
    case 0b00000000:
      {
      }
    case 0b11000000:
      {
        estadoMotores = IZQUIERDA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  //g izq
    case 0b11010000:
      {
        estadoMotores = IZQUIERDA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  // g izq
    case 0b10010000:
      {
        estadoMotores = DERECHA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  // g der
    case 0b10000000:
      {
        estadoMotores = DERECHA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  //g der
    case 0b00110000:
      {
        estadoMotores = ARCO_IZQ;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  //a izq
    case 0b01110000:
      {
        estadoMotores = ARCO_IZQ;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  //a izq
    case 0b00100000:
      {
        estadoMotores = ARCO_DER;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  //a der
    case 0b01100000:
      {
        estadoMotores = ARCO_DER;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }  //a der
    case 0b11110000:
      {
        estadoMotores = IZQUIERDA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          estadoMotores = ARCO_IZQ;
          ti = millis();
        }
        break;
      }  //giro izq, arco izq
    case 0b11100000:
      {
        estadoMotores = IZQUIERDA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          estadoMotores = ARCO_DER;
          ti = millis();
        }
        break;
      }  //giro izq, arco der
    case 0b10110000:
      {
        estadoMotores = DERECHA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          estadoMotores = ARCO_IZQ;
          ti = millis();
        }
        break;
      }  //giro der, arco izq
    case 0b10100000:
      {
        estadoMotores = DERECHA;
        if (intervalo(ti, 170)) {
          estadoActual = PISO;
          estadoMotores = ARCO_DER;
          ti = millis();
        }
        break;
      }  //giro der, arco der
  }
}
//---------------------------------------------FUNCIONES----------------------------------------------------------------------

void pasarPWMAMotores(int valorPWMIzq, int valorPWMDer) {
  if (valorPWMIzq > 0) {
    digitalWrite(motorIZQ_1, LOW);
    digitalWrite(motorIZQ_2, HIGH);
    analogWrite(pwmIzq, valorPWMIzq);
  } else {
    digitalWrite(motorIZQ_1, HIGH);
    digitalWrite(motorIZQ_2, LOW);
    analogWrite(pwmIzq, -valorPWMIzq);
  }
  if (valorPWMDer > 0) {
    digitalWrite(motorDER_1, LOW);
    digitalWrite(motorDER_2, HIGH);
    analogWrite(pwmDer, valorPWMDer);
  } else {
    digitalWrite(motorDER_1, HIGH);
    digitalWrite(motorDER_2, LOW);
    analogWrite(pwmDer, -valorPWMDer);
  }
}
//------------------------------------------------------------------------------------------------------------------------------
void atacarEnemigo() {  //si el enemigo esta cerca, lo ataca
  motores();
  if (distCen < minDistancia) {
    estadoMotores = ADELANTE;
  }
  if (distDer < minDistancia) {
    estadoMotores = DERECHA;
  }
  if (distIzq < minDistancia) {
    estadoMotores = IZQUIERDA;
  }
}

int senCNY(int a) {
  int valorSenCNY = analogRead(a);
  return valorSenCNY;
}

float medirDistancia(int a, int b) {  //para medir la distancia
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  int tiempo = pulseIn(b, HIGH, 20000);
  if (tiempo == 0) {
    return 999;
  }
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


  Serial.begin(9600);
}

void loop() {
  motores();
  switch (estadoActual) {
    case APAGADO:
      {
        estadoMotores = DETENIDO;

        if (digitalRead(swInicio) == LOW) {  //si se presiona el boton empieza a esperar
          estadoActual = ESPERA;
          ti = millis();
        }
        break;
      }
    case ANALIZAR:
      {
        int leerDip = analogRead(DIP);
       int dipAnalizado = leerDip & 0b11110000;
      }
    case ESPERA:
      {
        estadoMotores = DETENIDO;
        if (intervalo(ti, esperaInicio)) {  // se fija si pasaron 5 seg
          estadoActual = INICIO;
          ti = millis();
        }
        break;
      }
    case INICIO:
      {

        digitalWrite(D2, HIGH);
        //irDerecha();  //gira por dos segundos antes de empezar a leer el piso (estrategia de inicio)
        estadoMotores = DERECHA;
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
          estadoMotores = ATRAS;

          if (intervalo(ti, 2000)) {

            estadoMotores = IZQUIERDA;
          }
        } else if (senCNY(cnyDer) < umbralCNY && senCNY(cnyIzq) > umbralCNY) {  //si el lado derecho ve blanco, gira
          estadoMotores = ATRAS;
          if (intervalo(ti, 2000)) {
            estadoMotores = DERECHA;
          }
        } else {  //si lee blanco completamente
          estadoMotores = ATRAS;
        }
      }
      break;
    case ATAQUE:
      {
        distDer = medirDistancia(ECHO_1, TRIG_1);
        distCen = medirDistancia(ECHO_2, TRIG_2);
        distIzq = medirDistancia(ECHO_3, TRIG_3);
        estadoMotores = DERECHA;
        atacarEnemigo();
        if (senCNY(cnyDer) < umbralCNY || senCNY(cnyIzq) < umbralCNY) {
          estadoActual = PISO;
          ti = millis();
        }
        break;
      }
  }
}