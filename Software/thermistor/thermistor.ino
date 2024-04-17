#define Thermister A2
// Constants for the specific thermistor
const double Vbus = 3.3; 
const double Rs = 9380.0; 

// Given constants for the thermistor

const double R0 = 11600.0; // Nominal resistance of the thermistor at room temperature (in Ohms)
//const float ROOM_TEMP = (23.2+273.15) // room temperature
const double beta = 3950.0; // Beta value for the NTC-MF52 thermistor



void setup() {
pinMode(Thermister, INPUT);
Serial.begin(9600);

}

void loop() {
  
int data = analogRead(Thermister);
double voltage = (Vbus / 1024.0) * (data+1); // Convert analog reading to voltage
//double R_measured = ((data-1023)/(data+1))*Rs;
double coffecient = log(voltage * Rs / (Vbus-voltage) / R0)/2.0;
//double coffecient = (voltage*Rs)/(Vbus-voltage);
// Using the Steinhart-Hart equation
double temp = 1.0 / ((1.0 / (273.15 + 25)) + (coffecient/beta));
//double temp = 1.0 / ((1.0 / (273.15 + 19)) + ((log(coffecient/R0))/beta));
double temperature = temp - 273.15; 

Serial.print("Temperature: ");
Serial.print(temperature);
Serial.println(" Celsius");
delay(1000);



}
