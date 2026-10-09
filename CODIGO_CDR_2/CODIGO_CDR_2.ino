//maquinas de estado!!
enum estados { APAGADO,
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

enum estrategiasIni { INIT,    //init
                      G_IZQ,   //giro izq
                      G_IZQ2,  //giro izq
                      G_DER,   //giro der
                      G_DER2   //giro Der
};
estrategiasIni estrategiaIniActual = 0;

enum estrategiasAta { INIT_A,    //init
                      G_IZQ_A,   //giro izq
                      G_IZQ2_A,  //giro izq, sin arco
                      G_DER_A,   //giro der
                      G_DER2_A   //giro Der, sin arco
};
estrategiasAta estrategiaAtaActual = 0;
//----------------------------------------------------------------------------------------------------------------------
//botones
#define swInicio 10
#define DIP A0

//----------------------------------------------------------------------------------------------------------------------
//ultraSonicos:
//#define ECHO_1 4
//#define TRIG_1 2

#define ECHO_2 A4  //se usa derecha
#define TRIG_2 A5  //se usa

#define ECHO_3 11  //se usa izq
#define TRIG_3 12

#define ECHO_4 A2  //se usa centro
#define TRIG_4 A1  //se usa

//#define ECHO_5 A2
//#define TRIG_5 13

//CNYs
#define cnyDer A7
#define cnyIzq A6

//----------------------------------------------------------------------------------------------------------------------
//motores
#define motorIZQ_1 5
#define motorIZQ_2 6

#define motorDER_1 8
#define motorDER_2 7

//pwm
#define pwmDer 9
#define pwmIzq 3
//----------------------------------------------------------------------------------------------------------------------
//parametros
const int minDistancia = 70;
const int esperaInicio = 5000;
unsigned long ti = 0;
const int buscaEspera = 3000;
const int umbralCNY_der = 500;
const int umbralCNY_izq =  1;

const int velMaxIzq = 255;
const int velMaxDer = 255;
//variables
int distDer_1 = 0;
int distDer_2 = 0;
int distCen = 0;
int distIzq_1 = 0;
int distIzq_2 = 0;

int dipAnalizado = 0;

//byte dipAnalizado = 0b00000000;
//--------------------------------------------MAQUINAS DE ESTADO--------------------------------------------------------------------------
//velocidades
void motores(int pwm_der, int pwm_izq) {
  static int velocidadPWM_DER = 0;
  static int velocidadPWM_IZQ = 0;
  switch (estadoMotores) {
    case ADELANTE:
      {
        Serial.println("va adelante");
        velocidadPWM_DER += 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ += 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case ATRAS:
      {
        Serial.println("va atras");
        velocidadPWM_DER -= 20;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ -= 20;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case DERECHA:
      {
        Serial.println("va derecha");
       velocidadPWM_DER -= 30;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ += 10;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case IZQUIERDA:
      {
        Serial.println("va izquierda");
        velocidadPWM_DER += 10;
        velocidadPWM_DER = constrain(velocidadPWM_DER, -pwm_der, pwm_der);
        velocidadPWM_IZQ -= 30;
        velocidadPWM_IZQ = constrain(velocidadPWM_IZQ, -pwm_izq, pwm_izq);
        pasarPWMAMotores(velocidadPWM_IZQ, velocidadPWM_DER);
        break;
      }
    case DETENIDO:
      {
        Serial.println("esta detenido");
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
    case INIT:
      {
        break;
      }
    case G_IZQ:
      {
        estadoMotores = IZQUIERDA;  //gira 90 a la izq
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }  //g izq
    case G_IZQ2:
      {
        estadoMotores = IZQUIERDA;  //gira 90 a la izq
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }
    case G_DER:
      {
        estadoMotores = DERECHA;  //gira 90 a la derecha
        if (intervalo(ti, 170)) {
          estadoActual = COMBATE;
          ti = millis();
        }
        break;
      }  //g izq
    case G_DER2:
      {
        estadoMotores = DERECHA;  //gira 90 a la derecha
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
    case INIT_A:
      {
        break;
      }
    case G_IZQ_A:
      {
        if (distCen < minDistancia) {  //si sensa adelante, se acerca y dsp gira
          estadoMotores = ADELANTE;
          ti = millis();
          if (intervalo(ti, 400)) {
            estadoMotores = IZQUIERDA;  //hace un arco a la izquierda
            motores(180, 255);          //lo tengo que hacer con PID --> prox competencia
          }
        }
        break;
      }
    case G_IZQ2_A:
      {
        static bool girandoDer = false;
        static bool girandoIzq = false;
        if (distCen < minDistancia) {  //si esta de frente al robot, va hacia el
          estadoMotores = ADELANTE;
          motores(255, 255);
          girandoDer = false;
          girandoIzq = false;
          Serial.println("lee adelante");
        } else if (distDer_1 < minDistancia) {  //si el robot esta de costado, se possiciona para enfrentarlo
          Serial.println("lee derecha");
          if (!girandoDer) {
            ti = millis();
            girandoDer = true;
          }
          estadoMotores = DERECHA;
          if (intervalo(ti, 170)) {
            estadoMotores = DERECHA;
            motores(255, 180);  //gira 90 a la derecha
            girandoDer = false;
          }
        } else if (distIzq_1 < minDistancia) {  //si el robot esta de costado, se possiciona para enfrentarlo
          Serial.println("lee izquierda");
          if (!girandoIzq) {
            ti = millis();
            girandoIzq = true;
          }
          estadoMotores = IZQUIERDA;
          if (intervalo(ti, 170)) {
            estadoMotores = IZQUIERDA;
            motores(180, 255);
            girandoIzq = false;
          }
        } else {
          estadoMotores = IZQUIERDA;
          motores(100, 100);
          girandoDer = false;
          girandoIzq = false;
        }
        break;
      }
    case G_DER_A:
      {
        if (distCen < minDistancia) {  //si sensa va para adelante
          estadoMotores = ADELANTE;
          ti = millis();
          if (intervalo(ti, 400)) {   //y dsp de un tiempo hace un arco (querria implementar que gire para donde vuelve a sensar)
            estadoMotores = DERECHA;  //lo tengo que hacer con PID --> prox competencia
            motores(180, 255);
          }
        }
        break;
      }
    case G_DER2_A:
      {
        static bool girandoDer = false;
        static bool girandoIzq = false;
        if (distCen < minDistancia) {  //cuando sensa, va a max velocidad
          estadoMotores = ADELANTE;
          motores(255, 255);
          girandoDer = false;
          girandoIzq = false;
        } else if (distDer_1 < minDistancia) {  //si sensa un costado, gira 90° y hace un arco a la derecha
          if (!girandoDer) {
            ti = millis();
            girandoDer = true;
          }
          estadoMotores = DERECHA;
          if (intervalo(ti, 170)) {
            estadoMotores = DERECHA;
            motores(255, 180);
            girandoDer = false;
          }
        } else if (distIzq_1 < minDistancia) {  //si sensa un costado, gira 90° y hace un arco a la izquierda
          if (!girandoIzq) {
            ti = millis();
            girandoIzq = true;
          }
          estadoMotores = IZQUIERDA;
          if (intervalo(ti, 170)) {
            estadoMotores = IZQUIERDA;
            motores(180, 255);
            girandoIzq = false;
          }
        } else {
          estadoMotores = ADELANTE;
          motores(100, 100);
          girandoDer = false;
          girandoIzq = false;
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
    digitalWrite(motorIZQ_1, HIGH);  //ESTAN INVERTIDOS
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
  //pinMode(ECHO_1, INPUT);
  //pinMode(TRIG_1, OUTPUT);

  pinMode(ECHO_2, INPUT);
  pinMode(TRIG_2, OUTPUT);

  pinMode(ECHO_3, INPUT);
  pinMode(TRIG_3, OUTPUT);

  pinMode(ECHO_4, INPUT);
  pinMode(TRIG_4, OUTPUT);

  //pinMode(ECHO_5, INPUT);
  //pinMode(TRIG_5, OUTPUT);

  //motores
  pinMode(motorDER_1, OUTPUT);
  pinMode(motorDER_2, OUTPUT);
  pinMode(motorIZQ_1, OUTPUT);
  pinMode(motorIZQ_2, OUTPUT);


  //botones

  pinMode(swInicio, INPUT_PULLUP);


  Serial.begin(9600);
}

void loop() {
  motores(200, 200);
  int lectCNY_der = senCNY(cnyDer);
  int lectCNY_izq = senCNY(cnyIzq);
  switch (estadoActual) {
    case APAGADO:
      {
        Serial.println("estado apagado");
        estadoMotores = DETENIDO;

        if (digitalRead(swInicio) == LOW) {  //si se presiona el boton empieza a esperar
          ti = millis();
          estadoActual = ESPERA;
          int leerDip = 0;
          for (byte i = 0; i < 8; i++) {  //
            leerDip += analogRead(DIP);
          }
          leerDip = leerDip / 8;
          if (leerDip <= 20) {
            dipAnalizado = 0;
          } else if (abs(leerDip - 180) <= 20) {
            dipAnalizado = 1;
          } else if (abs(leerDip - 306) <= 20) {
            dipAnalizado = 2;
          } else if (abs(leerDip - 400) <= 20) {
            dipAnalizado = 3;
          } else if (abs(leerDip - 471) <= 20) {
            dipAnalizado = 4;
          }
        }
        break;
      }
    case ESPERA:
      {
        Serial.println("estado espera");
        estadoMotores = DETENIDO;
        if (intervalo(ti, esperaInicio)) {  // se fija si pasaron 5 seg
          estadoActual = INICIO;
          ti = millis();
        }
        break;
      }
    case INICIO:
      {
        Serial.println("estado inicio");
        estrategiaIniAnalizada();
        break;
      }
    case COMBATE:
      {
        if (lectCNY_der < umbralCNY_der && lectCNY_izq < umbralCNY_izq) {  //si el piso lee negro pasa a ver si hay enemigos
          distDer_1 = medirDistancia(TRIG_2, ECHO_2);
          //distDer_2 = medirDistancia(ECHO_3, TRIG_3);  //NO SE USA
          distCen = medirDistancia(TRIG_4, ECHO_4);
          distIzq_1 = medirDistancia(TRIG_3, ECHO_3);
          //Serial.println("Leyendo negro cny");
          //distIzq_2 = medirDistancia(ECHO_1, TRIG_1);  //NO SE USA
          estrategiaAtaque();
          //Serial.print("C:"); Serial.println(distCen);
          //Serial.print(" D:"); Serial.println(distDer_1);
          //Serial.print(" I:"); Serial.println(distIzq_1);

        } else {  //si lee blanco completamente
          estadoMotores = ATRAS;
          motores(150, 150);
          Serial.println("leyendo blanco cny");
        }
        break;
      }
  }
}