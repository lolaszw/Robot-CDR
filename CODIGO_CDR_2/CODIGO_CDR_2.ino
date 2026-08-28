//maquinas de estado!!
enum estados { APAGADO,
               ANALIZAR,
               ESPERA,
               INICIO,
               COMBATE
};
estados estadoActual = APAGADO;

enum movMotores { ADELANTE,
                  ATRAS,
                  DERECHA,
                  IZQUIERDA,
                  ARCO_IZQ,
                  ARCO_DER,
                  DETENIDO };
movMotores estadoMotores = DETENIDO;

enum estrategiasIni { INIT = 0b00000000,    //init
                      G_IZQ = 0b11000000,   //giro izq
                      G_IZQ2 = 0b11010000,  //giro izq
                      G_DER = 0b10000000,   //giro der
                      G_DER2 = 0b10010000   //giro Der
};
estrategiasIni estrategiaIniActual = 0b00000000;

enum estrategiasAta { INIT_A = 0b00000000,    //init
                      G_IZQ_A = 0b11000000,   //giro izq
                      G_IZQ2_A = 0b11010000,  //giro izq
                      G_DER_A = 0b10000000,   //giro der
                      G_DER2_A = 0b10010000   //giro Der
};
estrategiasAta estrategiaAtaActual = 0b00000000;
//----------------------------------------------------------------------------------------------------------------------
//botones
#define swInicio 2
#define DIP A2

//----------------------------------------------------------------------------------------------------------------------
//ultraSonicos:
#define ECHO_1 A4
#define TRIG_1 12

#define ECHO_2 A7
#define TRIG_2 11

#define ECHO_3 A1
#define TRIG_3 

#define ECHO_4 10
#define TRIG_4 A3

#define ECHO_5 A0
#define TRIG_5 A5

//CNYs
#define cnyDer A6
#define cnyIzq A7

//----------------------------------------------------------------------------------------------------------------------
//motores
#define motorDER_1 4
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
const int umbralCNY = 400;
const int velMaxIzq = 255;
const int velMaxDer = 255;
//variables
int distDer = 0;
int distCen = 0;
int distIzq = 0;

byte dipAnalizado = 0b00000000;
//--------------------------------------------MAQUINAS DE ESTADO--------------------------------------------------------------------------
//velocidades
void motores(int pwm_der, int pwm_izq) {
  static int velocidadPWM_DER = 0;
  static int velocidadPWM_IZQ = 0;
  switch (estadoMotores) {
    case ADELANTE:
      {
        velocidadPWM_DER += 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ += 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case ATRAS:
      {
        velocidadPWM_DER -= 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ -= 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case DERECHA:
      {
        velocidadPWM_DER -= 30;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ += 10;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case IZQUIERDA:
      {
        velocidadPWM_DER += 10;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ -= 30;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
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

void estrategiaIniAnalizada() {
  estrategiaIniActual = dipAnalizado;
  switch (estrategiaIniActual) {
    case 0b00000000:
      {
        break;
      }
    case G_IZQ:
      {
        estadoMotores = IZQUIERDA;
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }  //g izq
    case G_IZQ2:
      {
        estadoMotores = IZQUIERDA;
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }  // g dder
    case G_DER:
      {
        estadoMotores = DERECHA;
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }  //g izq
    case G_DER2:
      {
        estadoMotores = DERECHA;
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }
  }
}
void estrategiaAtaque() {
  estrategiaAtaActual = dipAnalizado;
  switch (estrategiaAtaActual) {
    case 0b00000000:
      {
        break;
      }
    case G_IZQ_A:
      {
        if (distCen < minDistancia) { //si sensa adelante, se acerca y dsp gira
          estadoMotores = ADELANTE;
          millis();
          if (intervalo(ti, 400)) {
            estadoMotores = IZQUIERDA;
            motores(180, 255);
          }
        }
        break;
      }
    case G_IZQ2_A:
      {
        if (distCen < minDistancia) {

          estadoMotores = ADELANTE;
          motores(255, 255);
        } else if (distDer < minDistancia) {
          estadoMotores = DERECHA;
          millis();
          if(intervalo(ti, 170)){
           estadoMotores = DERECHA;
            motores(255, 180); 
          }
        } else if (distIzq < minDistancia) {
          estadoMotores = IZQUIERDA;
          millis();
          if (intervalo(ti, 170)) {
            estadoMotores = IZQUIERDA;
            motores(180, 255);
          }
        }
        break;
      }
    case G_DER_A:
      {
        if (distCen < minDistancia) { //si sensa va para adelante 
          estadoMotores = ADELANTE;
          millis();
          if (intervalo(ti, 400)) { //y dsp de un tiempo hace un arco (querria implementar que gire para donde vuelve a sensar)
            estadoMotores = IZQUIERDA;
            motores(180, 255);
          }
        }
        break;
      }
    case G_DER2_A:
      {
        if (distCen < minDistancia) { //cuando sensa, va a max velocidad
          estadoMotores = ADELANTE;
          motores(255, 255);
        } else if (distDer < minDistancia) { //si sensa un costado, gira 90° y hace un arco a la derecha 
          estadoMotores = DERECHA;
          millis();
          if(intervalo(ti, 170)){ /
           estadoMotores = DERECHA;
            motores(255, 180); 
          }
        } else if (distIzq < minDistancia) { //si sensa un costado, gira 90° y hace un arco a la izquierda 
          estadoMotores = IZQUIERDA;
          millis();
          if (intervalo(ti, 170)) {
            estadoMotores = IZQUIERDA;
            motores(180, 255);
          }
        }
        break;
      }
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

  if (distCen < minDistancia) {
    estadoMotores = ADELANTE;
  } else if (distDer < minDistancia) {
    estadoMotores = DERECHA;
  } else if (distIzq < minDistancia) {
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
  motores(200, 200);
  switch (estadoActual) {
    case APAGADO:
      {
        estadoMotores = DETENIDO;

        if (digitalRead(swInicio) == LOW) {  //si se presiona el boton empieza a esperar
          ti = millis();
          estadoActual = ANALIZAR;
          int leerDip = 0;
          for(byte i = 0; i < 8; i++)
          {
            leerDip += analogRead(DIP);
          }
          leerDip = leerDip / 8;
          if(leerDip <= 20)
          {
            dipAnalizado = 0;
          }
          else if(abs(leerDip - 180) <= 20)
          {
            dipAnalizado = 1;
          }
          else if(abs(leerDip - 306) <= 20)
          {
            dipAnalizado = 2;
          }
          else if(abs(leerDip - 400) <= 20)
          {
            dipAnalizado = 3;
          }
          else if(abs(leerDip - 471) <= 20)
          {
            dipAnalizado = 4;
          }
        }
        break;
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
        estrategiaIniAnalizada();
        break;
      }
    case COMBATE:
      {
        if (senCNY(cnyDer) > umbralCNY && senCNY(cnyIzq) > umbralCNY) {  //si el piso lee negro pasa a ver si hay enemigos
          distDer_1 = medirDistancia(ECHO_1, TRIG_1);
          distDer_2 = medirDistancia(ECHO_4, TRIG_4);
          distCen = medirDistancia(ECHO_2, TRIG_2);
          distIzq_1 = medirDistancia(ECHO_3, TRIG_3);
          distIzq_2 = medirDistancia(ECHO_3, TRIG_3);

          estrategiaAtaque();
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
        break;
      }
  }
}