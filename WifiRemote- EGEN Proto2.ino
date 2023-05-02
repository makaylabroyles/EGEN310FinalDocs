
//Author: Makayla Broyles 

#include <WiFi.h>
#include <WiFiClient.h>
#include <stdio.h>
#include <string.h>
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// int motor1Pin = 25;
// int motor1Pin2 = 26;
// int motor2Pin = 32;
// int motor2Pin2 =33;
// int motor1Speed = 13; 
// int motor2Speed = 12;

 int motor1Pin = 16;
int motor1Pin2 = 17;
int motor2Pin = 27;
int motor2Pin2 =26;

Servo endArm; 
Servo startArm; 
Servo entireArm; 
Servo claw;

const char* ssid     = "ESP32AP";
const char* password = "123456789";

WiFiServer server(80);

String received;
char inputChar;
char c;
char* tok;
char control_vector[30];
int i;


void setup(void){

  Serial.begin(9600);
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
 
  endArm.attach(22);
  startArm.attach(23);
  entireArm.attach(21);
  claw.attach(19);

  
  delay(10);
  Serial.println('\n');

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println(WiFi.softAPIP());                // print IP to serial monitor

  server.begin();
  Serial.println("Server started");


}

void loop() {
  
  // listen for incoming clients
  WiFiClient client = server.available();

  // client has connected
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    String currentLine = "";

    // while client remains connected
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //received += c;
        Serial.write(c);
        if (c == '\n') {
          if(currentLine.length() == 0){ // two newlines received
            // Gets any data from POST
            i = 0;
            while(client.available()){
              inputChar = client.read();
              switch(inputChar){
                case 'B': //back
                  Serial.println(inputChar);
                  analogWrite(motor1Pin, 0);
                  analogWrite(motor1Pin2, 255);
                  analogWrite(motor2Pin, 255);
                  analogWrite(motor2Pin2, 0);
                  // analogWrite(motor1Speed, 100);
                  // analogWrite(motor2Speed, 100);
                  break;
              
                case 'S': //stop
                    Serial.println(inputChar);
                    analogWrite(motor1Pin, 0);
                    analogWrite(motor1Pin2, 0);
                    analogWrite(motor2Pin, 0);
                    analogWrite(motor2Pin2, 0);
                    break;
              
                
                case 'F': //forward
                    Serial.println(inputChar);
                    analogWrite(motor1Pin, 255);
                    analogWrite(motor1Pin2, 0);
                    analogWrite(motor2Pin, 0);
                    analogWrite(motor2Pin2, 255);
                    break;
  
                case 'E': //forward FAST
                    Serial.println(inputChar);
                    analogWrite(motor1Pin, 170);
                    analogWrite(motor1Pin2, 0);
                    analogWrite(motor2Pin, 0);
                    analogWrite(motor2Pin2, 170);
                    break;
                
                case 'L': //left
                    Serial.println(inputChar);
                    analogWrite(motor1Pin, 70);
                    analogWrite(motor1Pin2, 0);
                    analogWrite(motor2Pin, 0);
                    analogWrite(motor2Pin2, 200);
                    break;

                case 'R': //right
                    Serial.println(inputChar);
                    analogWrite(motor1Pin, 200);
                    analogWrite(motor1Pin2, 0);
                    analogWrite(motor2Pin, 0);
                    analogWrite(motor2Pin2, 70);
                    break;

                case 'U': 
                  startArm.write(0);
                  break;

                case 'D': //start arm down
                    Serial.println(inputChar);
                    for(int n =0; n < 75; n++){
                        delay(100);
                        startArm.write(n);
                    }
                    break;
                case 'M': //end arm up
                    Serial.println(inputChar);
                    endArm.write(140);
                    break;
                case 'N': //end arm down
                    Serial.println(inputChar);
                    endArm.write(45);
                    break;

                case 'T':
                    Serial.println(inputChar);
                    entireArm.write(90);
                    break;
                case 'C': //claw closed
                    claw.write(0);
                    break;
                case 'O': //claw open
                    claw.write(45);
                    break;
                    
              }
             }
            }

            // html for site
            Serial.println();
            
            //http header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // close connection after completion of response
            //client.println("Refresh: 10");  // refresh every 10 sec
            client.println();
  
            // display web page
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<style>");
            client.println("html {display:inline-block;margin:0px auto;text-align:center;}");
            client.println("button {padding:20px 40px; margin:4px;}");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");

            // title
            client.println("<h2>Group F.4</h2>");

            // buttons for all functionality
             client.println("<h3>Drive Controls</h3>");
            client.println("<form action=\"783\" method=\"post\">");
            client.println("<button name=\"offbutton\" value=\"S:\" style=\"height:60px;width:300px;color:red;font-size:50px\">STOP</button>");
            client.println("<button name=\"forwardbutton\" value=\"F:\" style=\"height:60px;width:300px;color:blue;font-size:50px\">Forward</button>");
            client.println("<button name=\"forwardFastbutton\" value=\"E:\" style=\"height:60px;width:300px;color:blue;font-size:50px\">SLOW</button>");
            client.println("<button name=\"backbutton\" value=\"B:\" style=\"height:60px;width:300px;color:blue;font-size:50px\">Backward</button>");
            client.println("<button name=\"leftbutton\" value=\"L:\" style=\"height:60px;width:300px;color:blue;font-size:50px\">Left</button>");
            client.println("<button name=\"rightbutton\" value=\"R:\" style=\"height:60px;width:300px;color:blue;font-size:50px\">Right</button>");
             client.println("<h3>Arm Controls</h3>");
            client.println("<button name=\"armupbutton\" value=\"U:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">SA Up</button>");
            client.println("<button name=\"armdownbutton\" value=\"D:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">SA Down</button>");
            client.println("<button name=\"armupbutton\" value=\"M:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">EA Up</button>");
            client.println("<button name=\"armdownbutton\" value=\"N:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">EA Down</button>");
            client.println("<button name=\"clawUpbutton\" value=\"O:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">Claw Open</button>");
            client.println("<button name=\"clawUpbutton\" value=\"C:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">Claw Closed</button>");
            client.println("<button name=\"armdownbutton\" value=\"T:\" style=\"height:60px;width:300px;color:blue;font-size:35px\">Turn</button>");

            

            client.println("</form>");


            // javascript
            client.println("<script type=\"text/javascript\">"); 
            client.println("vector.addEventListener('mousedown', e => {");
            client.println("var xhr = new XMLHttpRequest();");
            client.println("rect = vector.getBoundingClientRect();"); 
            client.println("x = event.clientX - rect.left - 150;"); 
            client.println("y = (event.clientY - rect.top - 150) * -1;");
            client.println("xhr.open(\"POST\", \"78\", true);");
            client.println("xhr.setRequestHeader(\"Accept\", \"text/xml\");");
            client.println("xhr.send(\"my=\" + x + \"+\" + y + \"%\");");
            client.println("console.log(\"Coordinate x: \" + x, \"Coordinate y: \" + y);"); 
            client.println("});"); 
     
            client.println("</script>"); 
           
            client.println("</body>");
            client.println("</html>");
            client.println();  
            break;
            
          }
          else {
            // received one newline so clear currentLine
            currentLine = "";
          }
        }
        else if(c != '\r'){
          currentLine += c;
        }  
      } 
    } 
  }
