/*
KeongonS
BuiltIn(ATMEGA32U4-MU)
PCB_ver
*/


// 디지털핀 설계
#define led_pin 13                    // 13번 디지털핀(led_pin)                 :: LED 출력
#define buzzer_pin 6                  // 6번 디지털핀(buzzer_pin)               :: BUZZER 출력
#define left_A_pin 4                  // 4번 디지털핀(left_A_pin)               :: MOTER 출력
#define left_B_pin 7                  // 7번 디지털핀(left_B_pin)               :: MOTER 출력
#define right_A_pin 12                // 12번 디지털핀(right_A_pin)             :: MOTER 출력
#define right_B_pin 8                 // 8번 디지털핀(right_B_pin)              :: MOTER 출력

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
  pinMode(buzzer_pin, OUTPUT);                // buzzer_pin
  pinMode(left_A_pin, OUTPUT);                // left_A_pin
  pinMode(left_B_pin, OUTPUT);                // left_B_pin
  pinMode(right_A_pin, OUTPUT);               // right_A_pin
  pinMode(right_B_pin, OUTPUT);               // right_B_pin
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
      // MOTER 후진
      left_moter_R_fn();
      right_moter_R_fn();      
   }
    else if (button_state == 1)
    {
      button_state = 0;
      // MOTER 전진
      left_moter_F_fn();
      right_moter_F_fn();
    }   
    // LED ON/OFF 함수호출
    blink_fn(button_state);
    // BUZZER ON/OFF 함수
    buzzer_fn(button_state);
  }
}
// 메인 끝


//-----
// MOTER 구동 함수
// left_moter_F_fn
void left_moter_F_fn()
{
  digitalWrite(left_A_pin, HIGH);
  digitalWrite(left_B_pin, LOW);
}
// left_moter_R_fn
void left_moter_R_fn()
{
  digitalWrite(left_A_pin, LOW);
  digitalWrite(left_B_pin, HIGH);
}
// right_moter_F_fn
void right_moter_F_fn()
{
  digitalWrite(right_A_pin, HIGH);
  digitalWrite(right_B_pin, LOW);
}
// right_moter_R_fn
void right_moter_R_fn()
{
  digitalWrite(right_A_pin, LOW);
  digitalWrite(right_B_pin, HIGH);
}


//-----
// BUZZER ON/OFF 함수
// buzzer_fn
void buzzer_fn(int button_state)
{
    if (button_state == 0)
    {
      // BUZZER OFF
      digitalWrite(buzzer_pin, LOW);
      Serial.println("BUZZER OFF");
    }
    else if (button_state == 1)
    {
      // BUZZER ON
      digitalWrite(buzzer_pin, HIGH);
      Serial.println("BUZZER ON");    
    }   
}


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