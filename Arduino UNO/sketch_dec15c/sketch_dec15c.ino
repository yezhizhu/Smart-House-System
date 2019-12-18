#include <Wire.h>
#include <dht.h>
#include <Servo.h>
#include <SoftwareSerial.h>
Servo myservo;//create servo object to control a servo

const int ledPin = 13;//the led attach to pin13
const int GasanalogPin=A0; //the DO on the Module attach to Pin7 on the SunFounder 
const int GasdigitalPin=7;  //the D0 attach to pin7
const int buzzerPin=8;
const int RainanalogPin=A1;
const int RaindigitalPin=4;

int Astate=0;//GAS
boolean  Dstate=0;//GAS

int AstateRain=0; //RAIN
boolean DstateRain=0;  //RAIN
// 温湿度传感器（DHT11）
#define DHT11_PIN 5
dht DHT;
float hum;
float tem;


#define SSID        "try" //改为你的热点名称, 不要有中文
#define PASSWORD    ""//密码Wi-Fi密码
#define DEVICEID    "575681988" //OneNet上的设备ID
String apiKey = "=c=iJ1e3YuFG6LMj3KLz6B1QUZs=";//与你的设备绑定的APIKey


#define HOST_NAME   "api.heclouds.com"
#define HOST_PORT   (80)
#define INTERVAL_SENSOR   5000             //定义传感器采样时间间隔  597000
#define INTERVAL_NET      5000             //定义发送时间
#include <ESP8266.h>
#include <SoftwareSerial.h>
#define EspSerial mySerial
#define UARTSPEED  9600
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(&EspSerial);

unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器


String postString;                                //用于存储发送数据的字符串
//WEBSITE     
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED;

#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;
int rain;

int servopin = 9;
int servopin2 = 10;
int myangle;//定义角度变量
int pulsewidth;//定义脉宽变量
int val;


void servopulse(int servopin,int myangle)//定义一个脉冲函数
{
pulsewidth=(myangle*11)+500;//将角度转化为500-2480 的脉宽值
digitalWrite(servopin,HIGH);//将舵机接口电平至高
delayMicroseconds(pulsewidth);//延时脉宽值的微秒数
digitalWrite(servopin,LOW);//将舵机接口电平至低
delay(20-pulsewidth/1000);
}



void setup() 
{
  //set the pins state 
  pinMode (GasdigitalPin,INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(buzzerPin,OUTPUT);

  pinMode(RaindigitalPin,INPUT); 

  //myservo.attach(9);//attachs the servo on pin 9 to servo object
  //myservo.write(0);//back to 0 degrees 
  pinMode(servopin, OUTPUT); 
  pinMode(servopin2, OUTPUT); 


   
  Serial.println(DHT_LIB_VERSION);//初始化温湿度传感器
  //初始化串口波特率  
//    Wire.begin();
//    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    Serial.print(F("setup begin\r\n"));
    delay(100);


  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));

  
} 

void loop()
{  
  Astate=analogRead(GasanalogPin);//read the value of A0
  Serial.println(Astate); //peint 
  Dstate=digitalRead(GasdigitalPin);//read the value of D0
  Serial.println(Dstate);//print
 // myservo.write(130);
  AstateRain=analogRead(RainanalogPin);  //read the value of A0
  Serial.print("A0: ");
  Serial.println(AstateRain);  //print the value in the serial monitor
  DstateRain=digitalRead(RaindigitalPin);  //read the value of D0
  Serial.print("D0: ");
  Serial.println(DstateRain);  
  
  if( Dstate==HIGH ) //if the value of D0 is HIGH
  {
    digitalWrite(ledPin,LOW);//turn off led
    digitalWrite(buzzerPin,HIGH);
  }
  if( Dstate==LOW)//else
  {
    digitalWrite(ledPin,HIGH);//turn on the led
    digitalWrite(buzzerPin,LOW);
  }
  
  
  if(DstateRain==HIGH)  
  {
   digitalWrite(ledPin,LOW);
    rain=0;
  }
  else //if the value of D0 is LOW
  {
  digitalWrite(ledPin,HIGH); //turn on the led

  for(int i=0;i<=50;i++) //给予舵机足够的时间让它转到指定角度
   {
    servopulse(servopin,90);//引用脉冲函数
    servopulse(servopin2,90);//引用脉冲函数
    }

   

 // myservo.write(85);//goes to 15 degrees
   rain=1;
  delay(1000);
  }

Serial.print("rain: ");
  Serial.println(rain);
  gethumtem();
  updateSensorData();
  delay(1000);//delay 200ms
}


void gethumtem(){
   Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
 // DISPLAY DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
  float tem=DHT.temperature;
  float hum=DHT.humidity;
  delay(1000);
  }

void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

    jsonToSend="{\"Temperature\":";
    //dtostrf(sensor_tem,1,2,buf);
    jsonToSend+="\""+String(DHT.temperature)+"\"";
    jsonToSend+=",\"Humidity\":";
    //dtostrf(sensor_hum,1,2,buf);
    jsonToSend+="\""+String(DHT.humidity)+"\"";
    jsonToSend+=",\"Rain\":";
    //dtostrf(sensor_lux,1,2,buf);
    jsonToSend+="\""+String(rain)+"\"";
    jsonToSend+="}";




    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
}
