#include <Wire.h>
#include "LiquidCrystal_I2C.h"
//INCLUIDO MOTORES --------------------------------------------------
#include <Servo.h>
#include <Stepper.h>

#include <Keypad.h>
#include <EEPROM.h>

struct EMPLEADO {
  char ID[10];
  char PSW[10];
};

//Entradas
#define INLAB1 51
#define INLAB2 53

LiquidCrystal_I2C lcd (0x20, 16, 2);

//VARIABLES MOTORES-------------------------------------------------
Servo srv;

const int MST1[4] = {23, 25, 27, 29};

const int patron[4][4] = {
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1},
};

//Mis variables:
byte lock[8]  = {0xe, 0x11, 0x11, 0x1f, 0x1b, 0x1b, 0x1f};
byte unlock[8]  = {0xe, 0x1, 0x1, 0x1f, 0x1b, 0x1b, 0x1f};
byte smyle[8]  = {0x0, 0xa, 0xa, 0x0, 0x11, 0x0e};
byte checked[8] = {0x1, 0x2, 0x16, 0x1c, 0x8};

int slab1 = 0;
int slab2 = 0;
int pospa = 0;
int clkst2 = 0;
int cerrarPuerta = 0;
unsigned long t = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;
unsigned long t8 = 0;
boolean acces = false;
boolean cerrado = true;

//EL SIGUIENTE SEGMENTO SE PUEDE ELIMINAR:--------------------------&&&&
#define INMENU 47
#define INLABDIR 45
#define OPCLSRV 43
int menu = 0;
int slabdir = 0;
//------------------------------------------------------------------&&&&


char state;   // Variable lectrura dato serial

#define pinTemp A0
int temperatura = 0;
float temp;

int ledPASE = 12;
int ledBLOQUEO = 11;
const byte ROWS = 4;
const byte COLS = 4;
int buzzer = 10;
//CCONTADOR DE INTENTOS DE ACCESO
int contadorIntento = 0;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '-'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '-'},
  {'*', '0', '#', 'E'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte candado [8] = { B00000, B01110, B01010, B11111, B10001, B10001, B11111, B00000};
int cantempleados;

//ESTADOS DE ACCESO AL SISTEMA
boolean estadoMsg = true;
boolean estadoID = false;
boolean estadoContrasena = false;
boolean estadoFallido = false;
boolean estadoBloqueado = false;
boolean estadoCorrecto = false;
boolean estadoRegistro = false;
boolean estadoDesbloquear = false;

//VARIABLES TEMPORALES DE ID Y PSW INGRESADOS
String ID = "";
String PSW = "";
String EID = "";
String EPSW = "";
String EGRT = "";


void setup() {
  // put your setup code here, to run once:
  //MOTOR 1 STEPER
  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(29, OUTPUT);
  //MOTOR 2 STEPPER
  //RELOJ 2 STEP
  pinMode(37, OUTPUT);
  //DIRECCION 2 STEP
  pinMode(39, OUTPUT);
  //LEDS
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  //LAB1
  pinMode(INLAB1, INPUT);
  //LAB2
  pinMode(INLAB2, INPUT);
  //LUCES
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  //SENSOR TEMPERATURA
  pinMode(A0, INPUT);



  //EL SIGUIENTE SEGMENTO SE PUEDE ELIMINAR, CUANDO ESTE LO DE BLUETOOTH:--------------------------&&&&
  //LABDIR
  pinMode(INLABDIR, INPUT);
  //MENU
  pinMode(INMENU, INPUT);
  //CerrarSRV
  pinMode(OPCLSRV, INPUT);
  //------------------------------------------------------------------&&&&


  lcd.init();
  lcd.backlight();


  lcd.createChar(2, lock);
  lcd.createChar(3, unlock);
  lcd.createChar(4, smyle);
  lcd.createChar(5, checked);
  srv.attach(33);
  srv.write(0);
  t = millis();
  t2 = millis();
  t8 = millis();

  cerrado = true;
  Serial.begin(9600);

  pinMode(ledPASE, OUTPUT);
  pinMode(ledBLOQUEO, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, candado);


  //LIMPIAR PRIMEROS 200 BYTES
  /*for (int i = 0 ; i < 200 ; i++) {
    EEPROM.write(i, 0);
    Serial.println(i);
    }*/

  //Borrando los empleados
  //cantempleados = 0;
  //EEPROM.put(0, cantempleados);
  //EEPROM.get(0, cant);

  int seed;
  EEPROM.get(0, seed);
  randomSeed(seed + 32);

}


boolean login_state;

int period = 1000;
unsigned long time_now = 0;

void loop() {

     if (cerrado == false && millis() - t3 >= 6000) {
            cerrarPorton();
            cerrado = true;
     }
  
  login_state = LOGIN();
  if (!login_state) {
    //Y se envia cuando no hay acceso a la app

    if (millis() > time_now + period) {
      time_now = millis();
      Serial.print("Y");
    }
    return;
  } else {
    Serial.print("");
    if (Serial.available() > 0) {
      state = Serial.read();
      switch (state) {
        case 'A':
          digitalWrite(14, HIGH);
          state = 0;
          break;
        case 'B':
          digitalWrite(14, LOW);
          state = 0;
          break;
        case 'C':
          digitalWrite(15, HIGH);
          state = 0;
          break;
        case 'D':
          digitalWrite(15, LOW);
          state = 0;
          break;
        case 'E':
          digitalWrite(16, HIGH);
          state = 0;
          break;
        case 'F':
          digitalWrite(16, LOW);
          state = 0;
          break;
        case 'G':
          digitalWrite(17, HIGH);
          state = 0;
          break;
        case 'H':
          digitalWrite(17, LOW);
          state = 0;
          break;
        case 'I':
          digitalWrite(14, HIGH);
          digitalWrite(15, HIGH);
          digitalWrite(16, HIGH);
          digitalWrite(17, HIGH);
          state = 0;
          break;
        case 'J':
          digitalWrite(14, LOW);
          digitalWrite(15, LOW);
          digitalWrite(16, LOW);
          digitalWrite(17, LOW);
          state = 0;
          break;
        case 'K':
          slabdir = 0;
          fun_barra_trans();
          state = 0;
          break;
        case 'L':
          slabdir = 1;
          fun_barra_trans();
          state = 0;
          break;
        case 'M':
          cerrarPuerta = 0;
          abrirPorton();
          t3 = millis();
          cerrado = false;
          state = 0;
          break;
        case 'N':
          cerrarPuerta = 1;
          cerrarPorton();
          cerrado = true;
          state = 0;
          break;
        case 'T':
          temperatura = analogRead(A0);
          temp = temperatura / 2;
          Serial.print(temp);
          state = 0;
      }
    }
    //Eliminar el digitalRead, a que se lea el Bluetooth. el switch puede seguir, el tipo de dato menu es int, cambiar a char.
  }
}

void fun_barra_trans() {

  //VEO HACIA QUE DIRECCION TIENE QUE IR LA CARGA, ESTE IF, PUEDE IR EN LA SECCION DEL SWITCH DEL LOOP,
  //PARA DESAPARECER ESTE METODO Y QUE SOLO SE DECIDA SI HAY QUE IR A LOS METODOS Lab1_to_Lab2(); o al otro.


  slab1 = digitalRead(INLAB1);
  slab2 = digitalRead(INLAB2);
  if (slabdir == 0) {
    //VA DE LAB1 A LAB2
    Lab1_to_Lab2();
  } else {
    //VA DE LAB2 A LAB1
    Lab2_to_Lab1();
  }
}

void Lab1_to_Lab2() {
  if (slab1 == 1 && slab2 == 0) {
    bool salir = false;
    bool sono = false;
    while (!salir) {
      lcd.setCursor(0, 0);
      lcd.print("Corriendo hacia ");
      lcd.setCursor(0, 1);
      lcd.print("la izquierda    ");
      funStepIzq();
      slab1 = digitalRead(INLAB1);
      slab2 = digitalRead(INLAB2);
      if (!sono) {
        tone(10, 100, 3000);
        sono = true;
      }
      if (slab1 == 0 && slab2 == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("La muestra llego");
        lcd.setCursor(0, 1);
        lcd.print("al LAB 2        ");
        tone(10, 100, 1000);
        delay(1500);
        salir = true;
      }
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Poner Muestra en");
    lcd.setCursor(0, 1);
    lcd.print("la banda LAB1   ");
  }
}

void Lab2_to_Lab1() {
  if (slab1 == 0 && slab2 == 1) {
    bool salir = false;
    bool sono = false;
    while (!salir) {
      lcd.setCursor(0, 0);
      lcd.print("Corriendo hacia ");
      lcd.setCursor(0, 1);
      lcd.print("la derecha     ");
      funStepDer();
      slab1 = digitalRead(INLAB1);
      slab2 = digitalRead(INLAB2);
      if (!sono) {
        tone(10, 800, 3000);
        sono = true;
      }
      if (slab1 == 1 && slab2 == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("La muestra llego");
        lcd.setCursor(0, 1);
        lcd.print("al LAB 1        ");
        tone(10, 800, 1000);
        delay(1500);
        salir = true;
      }
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Poner Muestra en");
    lcd.setCursor(0, 1);
    lcd.print("la banda LAB2   ");
  }
}

void funStepIzq() {
  for (int j = 0; j < 4; j++) {
    if (patron[pospa][j] == 1) {
      digitalWrite(MST1[j], HIGH);
    } else {
      digitalWrite(MST1[j], LOW);
    }
  }
  digitalWrite(39, HIGH);
  if (millis() - t2 >= 65) {
    if (clkst2 == 0) {
      digitalWrite(37, HIGH);
      clkst2 = 1;
    } else {
      digitalWrite(37, LOW);
      clkst2 = 0;
    }
    t2 = millis();
  }
  if (millis() - t >= 130) {
    pospa--;
    if (pospa == -1) {
      pospa = 3;
    }
    t = millis();
  }
}

void funStepDer() {
  for (int j = 0; j < 4; j++) {
    if (patron[pospa][j] == 1) {
      digitalWrite(MST1[j], HIGH);
    } else {
      digitalWrite(MST1[j], LOW);
    }
  }
  digitalWrite(39, LOW);
  if (millis() - t2 >= 65) {
    if (clkst2 == 0) {
      digitalWrite(37, HIGH);
      clkst2 = 1;
    } else {
      digitalWrite(37, LOW);
      clkst2 = 0;
    }
    t2 = millis();
  }
  if (millis() - t >= 130) {
    pospa++;
    if (pospa == 4) {
      pospa = 0;
    }
    t = millis();
  }
}

//FUNCION DEL PORTON
void abrirPorton() {
  if(cerrado){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(5));
    lcd.setCursor(1, 0);
    lcd.print("&ControlPorton&");
    lcd.setCursor(2, 1);
    lcd.write(byte(3));
    lcd.setCursor(3, 1);
    lcd.write(byte(4));
    lcd.setCursor(4, 1);
    lcd.print("Abriendo");
    lcd.setCursor(12, 1);
    lcd.write(byte(4));
    for (int i = 0; i <= 180; i++) {
      srv.write(i);
      delay(15);
    }
    digitalWrite(34, HIGH);
  }
}

void cerrarPorton() {
  if(!cerrado) {
    digitalWrite(34, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(5));
    lcd.setCursor(1, 0);
    lcd.print("&ControlPorton&");
    lcd.setCursor(2, 1);
    lcd.write(byte(2));
    lcd.setCursor(3, 1);
    lcd.write(byte(4));
    lcd.setCursor(4, 1);
    lcd.print("Cerrando");
    lcd.setCursor(12, 1);
    lcd.write(byte(4));
    for (int i = 180; i >= 0; i--) {
      srv.write(i);
      delay(15);
    }
    digitalWrite(35, HIGH);
    tone(10, 550, 3000);
    delay(3000);
    digitalWrite(35, LOW);
  }
}



boolean LOGIN() {



  //ESTADO DE MENSAJE DE BIENVENIDA
  if (estadoMsg) {
    msg_bienvenida();
    estadoMsg = false;
    estadoID = true;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("IDENTIFICADOR:");
    lcd.setCursor(6, 1);
  }

  //ESTADO DE INGRESO DE ID
  if (estadoID) {
    char key = keypad.getKey();
    if (key) {
      if (key == 'E') {
        if (ID == "0000") {
          ID = "";
          estadoID = false;
          estadoRegistro = true;
        } else {
          estadoID = false;
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("CONTRASENA:");
          lcd.setCursor(4, 1);
          estadoContrasena = true;
        }


      } else {
        lcd.print(key);
        ID = ID + key;
      }
    }
  }

  //ESTADO INGRESO DE CONTRASEÑA
  if (estadoContrasena) {
    char key = keypad.getKey();
    if (key) {
      if (key == 'E') {
        estadoContrasena = false;
        if (ValidacionDatos(ID, PSW)) {
          estadoCorrecto = true;
        } else {
          estadoFallido = true;
        }
        ID = "";
        PSW = "";
        lcd.clear();
      } else {
        lcd.print(key);
        PSW  = PSW  + key;
      }
    }
  }

  //ESTADO ERROR (INTENTELO DE NUEVO)
  if (estadoFallido) {
    contadorIntento++;
    lcd.setCursor(5, 0);
    lcd.print("ERROR");
    lcd.setCursor(15, 0);
    lcd.print(contadorIntento);
    lcd.setCursor(0, 1);
    lcd.print("ACCESO DENEGADO");
    delay(2000);
    if (contadorIntento == 4) {
      lcd.clear();
      estadoBloqueado = true;
    } else {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("IDENTIFICADOR:");
      lcd.setCursor(6, 1);
      estadoID = true;
    }
    estadoFallido = false;
  }

  //ESTADO MODO BLOQUEO
  if (estadoBloqueado) {
    lcd.setCursor(1, 0);
    lcd.print("SIST BLOQUEADO");
    lcd.setCursor(0, 1);
    lcd.print("AVISE AL GERENTE");
    digitalWrite(ledBLOQUEO, HIGH);
    for (int i = 0; i <= 5; i++) {
      SPKBLOQUEO();
    }
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CLAVE GERENTE:");
    lcd.setCursor(6, 1);
    estadoBloqueado = false;
    estadoDesbloquear = true;
  }

  //ESTADO DESBLOQUEAR
  if (estadoDesbloquear) {
    char key = keypad.getKey();
    if (key) {
      if (key == 'E') {
        if (EGRT == "0106") {
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("IDENTIFICADOR:");
          lcd.setCursor(6, 1);
          estadoID = true;
          contadorIntento = 0;
          estadoDesbloquear = false;
          digitalWrite(ledBLOQUEO, LOW);
        } else {
          estadoBloqueado = true;
          estadoDesbloquear = false;
          lcd.clear();
        }
        EGRT = "";
      } else {
        lcd.print(key);
        EGRT = EGRT  + key;
      }
    }
  }


  //ESTADO DE REGISTRO DE EMPLEADO
  if (estadoRegistro) {
    Registro();
  }
  //ESTADO CREDENCIALES CORRECTAS


  if (estadoCorrecto) {
    acces = true;
    lcd.setCursor(5, 0);
    lcd.print("ACCESO");
    lcd.setCursor(4, 1);
    lcd.print("PERMITIDO");
    digitalWrite(ledPASE, HIGH);
    estadoCorrecto = false;
    for (int i = 0; i <= 5; i++) {
      SPKBIENVENIDO();
    }

  }


  return acces;
}



int estadoregistro = 0;
String nuevoID = "";
String nuevaPsw = "";
String repeatPsw = "";
String adminPsw = "";


void Registro() {

  //MOSTRANDO MENSAJE NUEVA CONTRASEÑA
  if (estadoregistro == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NUEVA CONTRASENA");
    lcd.setCursor(4, 1);
    estadoregistro = 1;
  }

  //INTRODUCIENDO CLAVE NUEVA
  if (estadoregistro == 1) {

    char key = keypad.getKey();
    if (key) {
      if (key == 'E') {
        if (nuevaPsw.length() <= 8) {
          estadoregistro = 2;
        } else {
          estadoregistro = 7;
        }
        lcd.clear();
      } else {
        lcd.print(key);
        nuevaPsw = nuevaPsw  + key;
      }
    }
  }

  //MOSTRANDO MENSAJE DE CONFIRMACION DE CONTRASEÑA
  if (estadoregistro == 2) {

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("CONFIRMACION");
    lcd.setCursor(4, 1);
    estadoregistro = 3;
  }

  //INTRODUCIENDO LA CONFIRMACION DE CLAVE
  if (estadoregistro == 3) {
    char key = keypad.getKey();
    if (key) {
      if (key == 'E') {
        if (nuevaPsw == repeatPsw) {
          estadoregistro = 4;
        } else {
          estadoregistro = 8;
          repeatPsw = "";
        }
      } else {
        lcd.print(key);
        repeatPsw = repeatPsw  + key;
      }
    }
  }

  //MOSTRANDO MENSAJE: CLAVE GERENTE
  if (estadoregistro == 4) {

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CLAVE GERENTE:");
    lcd.setCursor(7, 1);
    estadoregistro = 9;
  }

  //INTRODUCIENDO LA CLAVE DEL GERENTE
  if (estadoregistro == 9) {

    char key = keypad.getKey();
    if (key) {
      if (key == 'E') {
        estadoregistro = 2;
        if (adminPsw == "0106") {
          adminPsw = "";
          estadoregistro = 5;
          lcd.clear();
        } else {
          estadoregistro = 6;
          adminPsw = "";
        }
      } else {
        lcd.print(key);
        adminPsw = adminPsw  + key;
      }
    }
  }



  //REGISTRO EXITOSO
  if (estadoregistro == 5) {
    lcd.setCursor(0, 0);
    lcd.print("REGISTRO EXITOSO");
    lcd.setCursor(1, 1);

    int randomNumber;
    randomNumber = random(2300, 6982);
    String nuevoID = "#" + String(randomNumber) + "*";
    lcd.print("SU ID: " + nuevoID);

    //----------------GUARDANDO EN MEMORIA---------------------
    char IDN[10];
    char PSWN[10];
    nuevoID.toCharArray(IDN, 10);
    nuevaPsw.toCharArray(PSWN, 10);
    EMPLEADO nuevo;
    strncpy(nuevo.ID, IDN, sizeof(nuevo.ID));
    strncpy(nuevo.PSW, PSWN, sizeof(nuevo.PSW));
    int registrados = 0;
    EEPROM.get(0, registrados);
    int puntero = 2 + (registrados * 20);
    EEPROM.put(puntero, nuevo);
    registrados = registrados + 1;
    EEPROM.put(0, registrados);
    //-------------------------------------------------------------
    nuevaPsw = "";
    adminPsw = "";
    repeatPsw = "";

    delay(4000);

    estadoregistro = 0;
    estadoRegistro = false;
    estadoID = true;


    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("IDENTIFICADOR:");
    lcd.setCursor(6, 1);
  }

  //ERROR: CLAVE GERENTE  INCORRECTA
  if (estadoregistro == 6) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("INCORRECTA");
    lcd.setCursor(6, 1);
    lcd.print("XXX");
    delay(3000);
    estadoregistro = 4;
  }

  //ERROR: CONTRASEÑA SUPERA 8 DIGITOS
  if (estadoregistro == 7) {

    lcd.setCursor(4, 0);
    lcd.print("EXCEDE 8");
    lcd.setCursor(5, 1);
    lcd.print("DIGITOS");
    delay(3000);
    estadoregistro = 0;
    nuevaPsw = "";
  }

  //ERROR: CONTRASEÑA NO COINCIDE
  if (estadoregistro == 8) {
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print("NO");
    lcd.setCursor(4, 1);
    lcd.print("COINCIDEN");
    delay(3000);
    estadoregistro = 2;
    repeatPsw = "";
  }
}

boolean ValidacionDatos(String IDE, String Password) {
  int cant = 0;
  EEPROM.get(0, cant);
  for (int i = 0; i < cant; i++) {
    EMPLEADO emp;
    EEPROM.get(2 + (i * 20), emp);
    if (String(emp.ID) == IDE && String(emp.PSW) == Password) {
      //Serial.println("Encontrado");
      return true;
    }
  }
  return false;
}


void MostrarTodo() {
  int cant = 0;
  EEPROM.get(0, cant);
  for (int i = 0; i < cant; i++) {
    EMPLEADO emp;
    EEPROM.get(2 + (i * 20), emp);
    Serial.println("#" + i + 1);
    Serial.print("ID: ");
    Serial.println(emp.ID);
    Serial.print("PASSWORD: ");
    Serial.println(emp.PSW);
    Serial.println("");
    delay(100);
  }
}

void SPKBLOQUEO() {
  tone(10, 200 , 80);
  delay(500);
  noTone(10);
  delay(500);
}

void SPKBIENVENIDO() {
  tone(10, 600 , 80);
  delay(500);
  noTone(10);
  delay(500);
}

void msg_bienvenida() {
  lcd.setCursor(1, 0);
  lcd.write(byte(1));
  lcd.setCursor(3, 0);
  lcd.print("BIENVENIDO");
  lcd.setCursor(14, 0);
  lcd.write(byte(1));
  delay(5000);
}
