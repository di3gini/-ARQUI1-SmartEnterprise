//bluetooth hc-05
int ledPin = 2; // usamos un pin de salida al LED
int ledPin2 = 3;
int ledPin3 = 4;

 char state;   // Variable lectrura dato serial

#define pinTemp A0
int temperatura = 0;
float temp;

void setup() {
    pinMode(ledPin, OUTPUT);   //Declara pin de Salida
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(A0,INPUT);
    digitalWrite(ledPin, LOW); //Normalmente Apagado
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    Serial.begin(9600);
}
 
void loop() {


        if(Serial.available() > 0){
             state = Serial.read();
             if (state == 'A') {
             
                 digitalWrite(ledPin, HIGH);
                 state = 0;
             }
            
             if (state == 'B')
             {
                 digitalWrite(ledPin, LOW);
                 state = 0;
             }
             if (state == 'C')
             {
                 digitalWrite(ledPin2, HIGH);
                 state = 0;
             }
             if (state == 'D')
             {
                 digitalWrite(ledPin2, LOW);
                 state = 0;
             }
             if (state == 'E')
             {
                 digitalWrite(ledPin3, HIGH);
                 state = 0;
             }
             if (state == 'F')
             {
                 digitalWrite(ledPin3, LOW);
                 state = 0;
             }
             if (state == 'T')
             {
                  temperatura = analogRead(A0);
                  temp = temperatura/2;
                  Serial.print(temp);

                
                state = 0;
             }
        }
}
