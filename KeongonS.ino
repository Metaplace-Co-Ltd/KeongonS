/*
KeongonS
BuiltIn(ATMEGA32U4-MU)
PCB_ver
*/


// 디지털핀 설계
#define led_pin 13                    // 13번 디지털핀(led_pin)                 :: LED 출력


//-----
// 부팅시 만 실행
void setup()
{
  // 시리얼통신 초기화(9600)bps(bits per second)
  Serial.begin(9600);

  // 디지털핀 초기화(OUTPUT)
  pinMode(led_pin, OUTPUT);                   // led_pin
}
// 부팅시 만 실행


//-----
// 메인(반복루프)
void loop()
{
  // LED ON   
  digitalWrite(led_pin, HIGH);
  Serial.println("LED ON");
}
// 메인 끝