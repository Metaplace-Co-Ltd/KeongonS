/*
KeongonS
BuiltIn(ATMEGA32U4-MU)
PCB_ver
*/


// 디지털핀 설계
#define led_pin 13                    // 13번 디지털핀(led_pin)                 :: LED 출력

// 아날로그핀 설계
#define button_pin A0                 // A0번 아날로그핀(button_pin)             :: 푸쉬스위치 입력

// 버튼상태 변수 선언
int button_state = 0;


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
  // 모드스위치(푸쉬스위치) 눌림 512이상 입력값 생성 :: 아날로그 입력값[0~1023]
  if (analogRead(button_pin) > 512)
  {
    // 모드스위치(푸쉬스위치) 누르고 기다리는 시간
    delay(500);

    if (button_state == 0)
    {
      button_state = 1;
   }
    else if (button_state == 1)
    {
      button_state = 0;
    }   
    // LED ON/OFF 함수호출
    blink_fn(button_state);
  }
}
// 메인 끝


//-----
// LED ON/OFF 함수
// blink_fn
void blink_fn(int button_state)
{
    if (button_state == 0)
    {
      // LED OFF
      digitalWrite(led_pin, LOW);
      Serial.println("LED OFF");
    }
    else if (button_state == 1)
    {
      // LED ON   
      digitalWrite(led_pin, HIGH);
      Serial.println("LED ON");
    }   
}