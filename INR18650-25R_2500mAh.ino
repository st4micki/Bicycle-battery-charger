
#include <LiquidCrystal_I2C.h>
#define ANALOG_PIN_IN0 A0
#define ANALOG_PIN_IN1 A1

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
// PRZETWORNIK ANALOGOWO CYFROWY

double ref = 2.55;
double res = ref/1024;



// WSPÓŁCZYNNIKI INTERPOLACJI CHARAKTERYSTYKI
double a1 = 12466;
double a2 = -223237;
double a3 = 1590071;
double a4 = -5628728;
double a5 = 9900448;
double a6 = -6921352;
// WŁASNY SYMBOL
byte customChar[] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};
byte blysk[] = {
  0x0F,
  0x1E,
  0x1C,
  0x0E,
  0x07,
  0x0E,
  0x0C,
  0x08
};
byte bateria[] = {
  0x0E,
  0x15,
  0x1F,
  0x15,
  0x11,
  0x1F,
  0x11,
  0x0E
};
byte strzalka[] = {
  0x04,
  0x04,
  0x04,
  0x04,
  0x04,
  0x15,
  0x0E,
  0x04
};

// CZUJNIK NAPIĘCIA
float R1 = 30000.0;
float R2 = 7500.0;
float adc_voltage = 0.0;
float in_voltage = 0.0;
int V_adc = 0;

// CZUJNIK PRĄDU
double Id = 0.0;
double V_sens = 0.0;
double scale = 0.1;
int I_adc = 0;
double zero = 2.5; 

// ZMIENNE
double Ri = 0.088;
double SOC;
double procent;
int i;
int j;



void setup()
{
  analogReference(EXTERNAL);
	lcd.begin(); 
	lcd.backlight();
  lcd.createChar(0, customChar);
  lcd.createChar(1, blysk);
  lcd.createChar(2, bateria);
  lcd.createChar(3, strzalka);
  lcd.home();
}

void loop()
{
	V_adc = analogRead(ANALOG_PIN_IN0);
  adc_voltage = V_adc * res;              // 10-bitowy przetwornik analogowo cyfrowy
  in_voltage = adc_voltage / (R2/(R1+R2)); 
  I_adc = analogRead(ANALOG_PIN_IN1);
  V_sens =(res * I_adc);
  Id = (V_sens - zero)/scale;
  Id =0;
  if(abs(Id) > 0.2){
    //in_voltage = in_voltage + (Ri*Id);
  }
  else{
    in_voltage = in_voltage;
  }
  
  if(in_voltage < 2.8){
    SOC = 0; 
  }
  else if(in_voltage < 4.17){   
    SOC = a1*(pow(in_voltage, 5)) 
    + a2*(pow(in_voltage, 4)) 
    + a3*(pow(in_voltage, 3)) 
    + a4*(pow(in_voltage, 2)) 
    + a5*(pow(in_voltage, 1)) 
    + a6;
  }
  else{
    SOC = 2500;   
  }

  if(SOC<0){
    SOC=0;
  }
  
  procent = (SOC/2500)*100;
  procent = (int)procent;

  lcd.print("BATTERY:");
  lcd.print(procent,0);
  lcd.print("%");

  procent = procent/10;  
  procent = int(procent);

  lcd.createChar(0, customChar); 
  lcd.home();
  lcd.setCursor(0, 1);
  lcd.print("[");
  for(i=0;i<procent;i++){
    lcd.write(0);
  }
  for(j=10-i;j>0;j--){
    lcd.print(" ");      
  }
  lcd.print("]"); 
  if(Id > 0.2){
    lcd.home();
    lcd.setCursor(14,1);
    lcd.write(2);    
    lcd.write(3);
  }
  else if(Id < -0.2){
    lcd.home();
    lcd.setCursor(14,1);
    lcd.write(2);    
    lcd.write(1); 
  }
  else{}


  delay(1000);          
  lcd.clear();
}
