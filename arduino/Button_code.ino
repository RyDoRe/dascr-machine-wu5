/*
found on https://robojax.com/learn/arduino/?vid=robojax_analog_push_buttons1

*/

unsigned int pushbuttonsCount=10;//number of push buttons and resistors
unsigned long resitorValue=1000;
unsigned long resitorToGround=100000;

byte inPin =A0;
float sourceVoltage=5;//5V or 3.3V depending on your arduino
char *push_button_name[]={"PB1", "PB2", "PB3", "PB4","PB5","PB6","PB7", "PB8", "PB9", "PB10"};
bool pbValue[]={0,0,0,0,0,0,0,0,0,0};//this holds the status of each push button
unsigned int outputPin[]={2,3,4,5,6,7,8,9,10,11};


int pushDelayTime=100;//100 milliseconds watch video https://youtu.be/MeWgnt0YLj8 for details
float V_tolerance =0.10;//10% (for example) for 1% set 0.01 for 5% set 0.05
bool debug=false;//to display votlage and calculations
///https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/ 
//https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/
const byte BIT_SOLUTION=12;
unsigned int r_total;

void readPushButtons();
void setup()
{
 //analogReadResolution(BIT_SOLUTION);//for Due, MKR only
 Serial.begin(9600);
 Serial.println("Robojax.com Code");
 Serial.print(pushbuttonsCount);
 Serial.println(" Push buttons");
 for(int i=0; i<pushbuttonsCount; i++)
 {
  pinMode(outputPin[i], OUTPUT);//set pin from outputPin[] as output.
 }

}//setup ends

void loop()
{
  readPushButtons();//

      
    for(unsigned int i=0; i< pushbuttonsCount; i++)
    { 
      if(pbValue[i])
      {
        Serial.print("pressed ");        
        Serial.println(push_button_name[i]);
        controlOutput(i);//control the output/relay or LED
     
      }//if
            


    }//for
  delay(100);
}//loop

/*
 * 
 * @brief reads input voltage when push buttons are pressed and updates the value of state of each push button
 * @param none
 * @return none
 * Written by Ahmad Shamshiri for Robojax.com 
 * on Dec 04, 2020 in Ajax, Ontatio, Canada
 * 
 */
void readPushButtons()
{
   float VR_Calculated,RT, voltage_maximum, voltage_minimum;
   float V_measured =analogRead(inPin);
//        Serial.print("RAW A0: ");
//        Serial.println(V_measured);  
    
   V_measured = V_measured* ( (float)sourceVoltage/(pow(2,BIT_SOLUTION)-1));
//        Serial.print("Measured Voltage: ");
//        Serial.print(V_measured);  
//        Serial.println("V");  
      if(debug)
      {       
     
        Serial.print("Measured Voltage: ");
        Serial.print(V_measured);  
        Serial.println("V");    

        Serial.print(pushbuttonsCount);
        Serial.println(" Push buttons");                
      }
    for(unsigned int i=0; i< pushbuttonsCount; i++)
    { 


     
       //calculate the R value for parallel resistor with the groudn resistors
      RT= 1/(   (1/ ((i+1)*(float)resitorValue) + (1/(float)resitorToGround)));
    
      VR_Calculated =( sourceVoltage * RT)/ (  (pushbuttonsCount - (i+1)) *(float)resitorValue + RT);//get voltage for the current resistor i
      if( i+1 == pushbuttonsCount)
      {
        VR_Calculated = sourceVoltage;//for last push button, voltage is the same as power supply
      }
  
 // watch video https://youtu.be/MeWgnt0YLj8 for details
   voltage_minimum = VR_Calculated-V_tolerance*VR_Calculated;//calcualte minium voltage for 10% (or whaterver set) reistors
   voltage_maximum = VR_Calculated+V_tolerance*VR_Calculated;//calcualte maximum voltage for 10% (or whaterver set) reistors

 if( i< pushbuttonsCount-1 && debug)
 {
  
  Serial.print("	 R-BP-");
  Serial.print(i+1);  
  Serial.print(" ");
  Serial.print(RT); 
  Serial.print("Ω");    
   if(i==0)
   {
    Serial.print("	");//this is fixing the tab space for first item less than 1000Ω
   }   
   Serial.print("	 VR ");
   Serial.print( VR_Calculated );   
   Serial.print("V");     

   Serial.print("	 V-Min ");
   Serial.print( voltage_minimum );   
   Serial.print("V");   
   
   Serial.print("	 V-Max ");
   Serial.print( voltage_maximum );   
   Serial.println("V"); 
 }
      // watch video https://youtu.be/MeWgnt0YLj8 for details
      if( V_measured <= ( voltage_maximum) && 
          V_measured >= ( voltage_minimum)
         )
         {
            pbValue[i]=true;//set puths button value to true
            delay(pushDelayTime);//push time limit
        }else{
          pbValue[i]=false;
        }//if
    }//for 
    
   Serial.println();
}//readPushButtons()


/*
 * controlOutput(int i)
 * @brief contorls the output pin
 * @param i is the corresponding to the push button pressed starting with zero 
 * @return none
 * Written by Ahmad Shamshiri for Robojax.com 
 * on Dec 04, 2020 in Ajax, Ontatio, Canada
 * 
 */
void controlOutput(int i)
{

  digitalWrite(outputPin[i], HIGH);
    if(debug){
  Serial.print("
Pin ");
  Serial.print(outputPin[i]);
  Serial.println(" is ON");
    }
}//controlOutput(int i) end
