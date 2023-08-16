  /*******************************************/
  /*                                         */
  /*   Encendido de LEDs mediante Joystick   */
  /*     Carlos Andrés De la rosa Rincón     */
  /*          12 de Agosto de 2023           */    
  /*                                         */
  /*******************************************/


///////////  VARIABLES PARA CONTROLAR EL PANEL LED 8X8  ///////////

// Configuración inicial del panel LED, posición [0,0] encendido
byte filaLED[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  

// Posiciones del led encendido según en que columna se encuentre
int columnaLED[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};      

// Coordenadas del LED encendido
int coordenadasLED[]={ 0, 0 };

// Posición inicial del LED encendido
int fila = 0;
int columna = 0;

// Comprueba si el joystick está en el centro
bool joystickListo = true;

///////////  VARIABLES PARA CONTROLAR EL JOYSTICK  ///////////

//  Ubiación del PIN del eje X y Y
const int X_PIN = A4;
const int Y_PIN = A5;

//  Ángulo X y Y
int angleX;
int angleY;


void setup() 				
{   
  // Definimos todos los pines del arduino como salida
  for (int pin=2; pin<19; pin++)
     	    pinMode(pin, OUTPUT);         
   Serial.begin(9600);
}



// Se ubica en el panel el LED que se desea encender  
void ConfiguraPanel(int fila, int columna)
{
  for(int y = 0; y < 8; y++)
    if (y == fila)
      filaLED[y] = columnaLED[columna];
    else  
      filaLED[y] = byte(0x00);
}


// Enciende el LED previamente seleccionado
void EnciendeLed() 
{    
  // Se apagan todos los LEDs del panel
  LimpiaPanel();
  
  for (int fil = 0; fil <8 ; fil++)
  {
    // Activamos la fila para el barrido
    digitalWrite( fil + 10 , LOW) ; 

    for (int col =7; col >= 0 ; col--)
    {  
      //Bajamos la columna
      digitalWrite(8-col, LOW);
      bool encendido = GetBit(filaLED[fil], col)  ;
      
      // Enciende o apaga el LED
      if (encendido)  digitalWrite( 9 - col  ,HIGH); 
      else  digitalWrite( 9 - col  ,LOW); 
    }                                    
    
    // Apagamos fila antes de salir          
    digitalWrite( fil + 10 , HIGH) ;             
  }
}

 
bool GetBit( byte N, int pos)
{                            
  // Posición = 7 6 5 4 3 2 1 0  
  int b = N >> pos ;         
  
  // Shift bits
  b = b & 1 ;                
  
  // Devolver solo el último bit
  return b ;                 
}


void LimpiaPanel()
{
  // Columnas apagadas
  for (int j=2; j<10; j++)  digitalWrite(j, LOW);
         
  // Filas desactivadas
  for (int k= 10 ; k<18 ; k++) digitalWrite(k, HIGH);	
}

void loop()
{
  /* ===========    JOYSTICK    ===========   */

  // Se captura el movimiento en los ejes X y Y del Joystick
  int valueX = analogRead(X_PIN);
  int valueY = analogRead(Y_PIN);

  // Se determina el ángulo en X y Y
  angleX = map(valueX, 0, 1023, 0, 180);
  angleY = map(valueY, 0, 1023, 0, 180);




  /* ===========    DIRECCIÓN JOYSTICK    ===========   */

  //  Arriba
  if(joystickListo && angleY < 5){
    fila--;
    joystickListo = false;
  }
  
  //  Abajo
  else if(joystickListo && angleY > 175){
    fila++;
    joystickListo = false;
  }

  //  Izquierda
  else if(joystickListo && angleX < 5){
    //columna--;
    //joystickListo = false;
  }

  //  Derecha
  else if(joystickListo && angleX > 138){
    columna++;
    joystickListo = false;
  }
  
  // Comprueba si el joystick está en el centro
  if(angleX < 10 && angleY > 80 && angleY < 100)
    joystickListo = true;
  
  
  // Restablece las coordenadas al llegar al límite
  if(fila < 0)      fila = 7;
  if(fila > 7)      fila = 0;  
  if(columna < 0)   columna = 7;
  if(columna > 7)   columna = 0;

 
  /* ===========    PANEL LED    ===========   */

  // Se asigna al panel la posición del LED que se desea encender
  ConfiguraPanel(fila, columna);

  // Se enciende el led seleccionado
  EnciendeLed();
}