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
int button_state_cnt = 0;
int led_blink_speed = 50;


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

  // LED 모드선택(최초 부팅시)
  blink_fn(button_state_cnt);          
}
// 부팅시 만 실행


//-----
// 메인(반복루프)
void loop()
{
  // 모드스위치(푸쉬스위치) 눌림 512이상 입력값 생성 :: 아날로그 입력값[0~1023]
  if (analogRead(button_pin) > 512)
  {
    // 모드 선택시
    // LED OFF
    digitalWrite(led_pin, LOW);    
    // button_state_cnt 누적 더하기
    button_state_cnt += 1;

    // 모드스위치(푸쉬스위치) 누르고 기다리는 시간
    delay(500);

    // 모드 처음부터 다시 시작
    if (button_state_cnt == 4)
    {
      button_state_cnt = 0;
    }
    // 시리얼 프린터 알림
    Serial.println(button_state_cnt);
    // LED 깜박임
    blink_fn(button_state_cnt);
  }

  // mode setting:
  mode_setting_fn(button_state_cnt);
}
// 메인 끝


//-----
// 모드 셋팅 함수
// mode_setting_fn
void mode_setting_fn(int button_state_cnt)
{
  // mode setting
  if (button_state_cnt == 0)
  {

  }
  else if (button_state_cnt == 1)
  {

  }
  else if (button_state_cnt == 2)
  {

  }
  else if (button_state_cnt == 3)
  {

  }
}


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
void blink_fn(int button_state_cnt)
{
  for (int i = 0; i < button_state_cnt; i++)
  {
    // LED ON      
    digitalWrite(led_pin, HIGH);
    delay(led_blink_speed);   
    // LED OFF
    digitalWrite(led_pin, LOW);
    delay(led_blink_speed);   
  }
  // LED ON    
  digitalWrite(led_pin, HIGH);
}