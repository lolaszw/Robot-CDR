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
                 DETENIDO };
movMotores estadoMotores = DETENIDO;


//botones
#define swInicio 10
#define DIP A0


//----------------------------------------------------------------------------------------------------------------------
//ultraSonicos:




#define ECHO_2 A5  //se usa derecha
#define TRIG_2 A4  //se usa


#define ECHO_3 11  //se usa izq
#define TRIG_3 12


#define ECHO_1 A2  //se usa centro
#define TRIG_1 A1  //se usa




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


//parametros
const int minDistancia = 70;
const int esperaInicio = 5000;
unsigned long ti = 0;
const int buscaEspera = 3000;
const int umbralCNY_der = 600;
const int umbralCNY_izq = 450;


const int velMaxIzq = 255;
const int velMaxDer = 255;
//variables
int distDer = 0;
int distCen = 0;
int distIzq = 0;




int dipAnalizado = 0;


void setup() {
 //ultraSonicos


 pinMode(ECHO_2, INPUT);
 pinMode(TRIG_2, OUTPUT);


 pinMode(ECHO_3, INPUT);
 pinMode(TRIG_3, OUTPUT);


 pinMode(ECHO_1, INPUT);
 pinMode(TRIG_1, OUTPUT);




 //motores
 pinMode(motorDER_1, OUTPUT);
 pinMode(motorDER_2, OUTPUT);
 pinMode(motorIZQ_1, OUTPUT);
 pinMode(motorIZQ_2, OUTPUT);




 //botones


 pinMode(swInicio, INPUT_PULLUP);




 //Serial.begin(9600);
}


void motores() {
 switch (estadoMotores) {
   case ADELANTE:
     digitalWrite(motorDER_1, HIGH);
     digitalWrite(motorDER_2, LOW);
     analogWrite(pwmDer, 255);
     digitalWrite(motorIZQ_1, HIGH);
     digitalWrite(motorIZQ_2, LOW);
     analogWrite(pwmIzq, 255);
     break;
   case ATRAS:
     digitalWrite(motorDER_1, LOW);
     digitalWrite(motorDER_2, HIGH);
     analogWrite(pwmDer, 255);
     digitalWrite(motorIZQ_1, LOW);
     digitalWrite(motorIZQ_2, HIGH);
     analogWrite(pwmIzq, 255);
     break;
   case DERECHA:
     digitalWrite(motorDER_1, HIGH);
     digitalWrite(motorDER_2, LOW);
     analogWrite(pwmDer, 230);
     digitalWrite(motorIZQ_1, LOW);
     digitalWrite(motorIZQ_2, HIGH);
     analogWrite(pwmIzq, 255);
     break;
   case IZQUIERDA:
     digitalWrite(motorDER_1, LOW);
     digitalWrite(motorDER_2, HIGH);
     analogWrite(pwmDer, 255);
     digitalWrite(motorIZQ_1, HIGH);
     digitalWrite(motorIZQ_2, LOW);
     analogWrite(pwmIzq, 230);
     break;
   case DETENIDO:
     digitalWrite(motorDER_1, LOW);
     digitalWrite(motorDER_2, LOW);
     analogWrite(pwmDer, 0);
     digitalWrite(motorIZQ_1, LOW);
     digitalWrite(motorIZQ_2, LOW);
     analogWrite(pwmIzq, 0);
     break;
 }
}



bool intervalo(long tiempoi, int t) {  //para ver si x intervalo se cumplio o no


 if (millis() >= tiempoi + t) {
   return true;
 } else {
   return false;
 }
}


float medirDistancia(int a, int b) {  //para medir la distancia
 digitalWrite(a, HIGH);
 delayMicroseconds(10);
 digitalWrite(a, LOW);
 int tiempo = pulseIn(b, HIGH, 30000);
 if (tiempo == 0) {
   return 999;
 }
 return tiempo / 59;
}


void loop() {
 motores();
 int lectCNY_der = analogRead(cnyDer);
 int lectCNY_izq = analogRead(cnyIzq);
 switch (estadoActual) {
   case APAGADO:
     estadoMotores = DETENIDO;
     if (digitalRead(swInicio) == LOW) {
       ti = millis();
         estadoActual = ESPERA;
     }
     break;
   case ESPERA:
     estadoMotores = DETENIDO;
     if (intervalo(ti, esperaInicio)) {  // se fija si pasaron 5 seg
       estadoActual = INICIO;
       ti = millis();
     }
     break;
   case INICIO:
     estadoMotores = IZQUIERDA;  //gira 90 a la izq
     if (intervalo(ti, 170)) {
       estadoActual = COMBATE;
       ti = millis();
     }
     break;
     break;
   case COMBATE:
     if (lectCNY_der < umbralCNY_der && lectCNY_izq < umbralCNY_izq) {  //si el piso lee negro pasa a ver si hay enemigos
       distDer = medirDistancia(TRIG_2, ECHO_2);
       distCen = medirDistancia(TRIG_1, ECHO_1);
       distIzq = medirDistancia(TRIG_3, ECHO_3);

        static bool girandoDer = false;
        static bool girandoIzq = false;
        if (distCen < minDistancia) {  //si esta de frente al robot, va hacia el
          estadoMotores = ADELANTE;
          girandoDer = false;
          girandoIzq = false;
          //Serial.println("lee adelante");
        } else if (distDer < minDistancia) {  //si el robot esta de costado, se possiciona para enfrentarlo
          //Serial.println("lee derecha");
          if (!girandoDer) {
            ti = millis();
            girandoDer = true;
          }
          estadoMotores = DERECHA;
          if (intervalo(ti, 170)) {
            estadoMotores = DERECHA; //gira 90 a la derecha
            girandoDer = false;
          }
        } else if (distIzq < minDistancia) {  //si el robot esta de costado, se possiciona para enfrentarlo
          //Serial.println("lee izquierda");
          if (!girandoIzq) {
            ti = millis();
            girandoIzq = true;
          }
          estadoMotores = IZQUIERDA;
          if (intervalo(ti, 170)) {
            estadoMotores = IZQUIERDA;
            girandoIzq = false;
          }
        } else {
          estadoMotores = ADELANTE;
          girandoDer = false;
          girandoIzq = false;
        }


     } else {  //si lee blanco completamente
       estadoMotores = ATRAS;
       //Serial.println("leyendo blanco cny");
     }
     break;
 }
}



