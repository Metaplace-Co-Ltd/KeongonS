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
#define left_pwm_pin 9                // 9번 디지털핀(left_pwm_pin)             :: MOTER PWM
#define right_pwm_pin 10              // 10번 디지털핀(right_pwm_pin)           :: MOTER PWM
#define wave_sensor_trig_pin 5        // 5번 디지털핀(wave_sensor_trig_pin)     :: Wave_Sensor
#define wave_sensor_echo_pin 11       // 11번 디지털핀(wave_sensor_echo_pin)    :: Wave_Sensor

// 아날로그핀 설계
#define button_pin A0                 // A0번 아날로그핀(button_pin)             :: 푸쉬스위치 입력
#define line_sensor_L_pin A1          // A1번 아날로그핀(line_sensor_L_pin)      :: 라인센서 입력
#define line_sensor_R_pin A2          // A2번 아날로그핀(line_sensor_R_pin)      :: 라인센서 입력

// 버튼상태 변수 선언
int button_state = 0;
int button_state_cnt = 0;
int led_blink_speed = 50;

// 블루투스 받는 문자열
String bluetooth_string = "";

// 모터 속도 셋팅
int L_moter_max_speed = 255;          // MOTER speed
int R_moter_max_speed = 255;          // MOTER speed
int L_moter_min_speed = 127;          // MOTER speed
int R_moter_min_speed = 127;          // MOTER speed
int L_moter_control_speed;            // MOTER speed
int R_moter_control_speed;            // MOTER speed

int line_sensor_L, line_sensor_R;     // 라인센서 변수선언
int line_sensor_limit = 100;


//-----
// 부팅시 만 실행
void setup()
{
  // 시리얼통신 초기화(9600)bps(bits per second)
  Serial.begin(9600);

  //블루투스시리얼[하드웨어] 초기화(9600)bps(bits per second)
  Serial1.begin(9600);  

  // 디지털핀 초기화(OUTPUT)
  pinMode(led_pin, OUTPUT);                   // led_pin
  pinMode(buzzer_pin, OUTPUT);                // buzzer_pin
  pinMode(left_A_pin, OUTPUT);                // left_A_pin
  pinMode(left_B_pin, OUTPUT);                // left_B_pin
  pinMode(right_A_pin, OUTPUT);               // right_A_pin
  pinMode(right_B_pin, OUTPUT);               // right_B_pin
  pinMode(left_pwm_pin, OUTPUT);              // left MOTER PWM
  pinMode(right_pwm_pin, OUTPUT);             // right MOTER PWM
  pinMode(wave_sensor_trig_pin, OUTPUT);      // Wave_Sensor
  pinMode(wave_sensor_echo_pin, INPUT);       // Wave_Sensor

  // LED 모드선택(최초 부팅시)
  blink_fn(button_state_cnt);          
}
// 부팅시 만 실행


//-----
// 메인(반복루프)
void loop()
{
  // 모드스위치(푸쉬스위치) 눌림 512이상 입력값 생성 :: 아날로그 입력값[0~1023]
  // 블루투스시리얼 0 => ASCII["48"]
  if ((analogRead(button_pin) > 512) || (bluetooth_string == "48"))
  {
    // 블루투스 받는 문자열 초기화
    bluetooth_string = "";

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
    // 블루투스시리얼[하드웨어] 프린터 알림
    Serial1.println(button_state_cnt);    
    // LED 깜박임
    blink_fn(button_state_cnt);
  }

  // 블루투스시리얼[하드웨어] 명령 수신
  if (Serial1.available())
  { // Serial1[하드웨어] 값이 있으면
    bluetooth_string = Serial1.read();  // 블루투스측 내용 저장
    // 0 => ASCII["48"]
    // Serial.println(bluetooth_string);
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
    // BLE_rx_tx_fn
    BLE_rx_tx_fn();
  }
  else if (button_state_cnt == 1)
  {
    // line_sensor_fn:
    line_sensor_fn();
  }
  else if (button_state_cnt == 2)
  {
    // avoid_run_fn
    avoid_run_fn();
  }
  else if (button_state_cnt == 3)
  {

  }
}


//-----
// avoid_run_fn
void avoid_run_fn()
{
  // 거리계산
  int distance_measure = distance_measure_fn();
  Serial.print("거리 : ");
  Serial.println(distance_measure);  
  // 블루투스시리얼[하드웨어] 프린터 알림
  Serial1.println(distance_measure);  

  if (distance_measure == 0)
  {
    Serial.println("중지");

    left_moter_stop_fn();
    right_moter_stop_fn();
  }  

  else if (distance_measure > 30)
  {
    Serial.println("전진");

    left_moter_F_fn(L_moter_max_speed);
    right_moter_F_fn(R_moter_max_speed);    
  }

  else if (distance_measure <= 30)
  {
    Serial.println("우회전");

    left_moter_F_fn(L_moter_max_speed);
    right_moter_stop_fn();
  }
}

//-----
// 거리측정 센서
int distance_measure_fn(void)
{
  // 초음파센서
  float distance;
  float duration;
  digitalWrite(wave_sensor_trig_pin, LOW);
  delayMicroseconds(5);  
  digitalWrite(wave_sensor_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(wave_sensor_trig_pin, LOW);

  duration = pulseIn(wave_sensor_echo_pin, HIGH, 20000);
  // 0.01초(사양서의 측정범위 :3.5메타까지 측정, 왕복 7메타, 마이크로초(20000)

  distance = duration * 17 / 1000;  // duration을 연산하여 센싱한 거리값을 distance에 저장
  return distance;
  /*
   거리 = 시간 * 속도
   속도는 음속 340m/sec 시간 * 340m이고 cm단위로 바꾸기 위해 34000cm로 변환합니다.
   시간 값이 저장된 duration은 마이크로초 단위로 저장되어 있어, 변환하기 위해 1000000을 나눠줍니다.
   그럼 시간 * 34000 / 1000000이라는 값이 나오고, 정리하여 거리 * 34 / 1000이 됩니다.
   하지만 시간은 장애물에 닿기까지와 돌아오기까지 총 두 번의 시간이 걸렸으므로 2를 나누어줍니다.
   그럼 시간 * 17 / 1000이라는 공식이 나옵니다.
  */
}


//-----
// line_sensor_fn
int line_sensor_fn()
{
  // Line_Sensor:
  line_sensor_L = analogRead(line_sensor_L_pin);
  line_sensor_R = analogRead(line_sensor_R_pin);
  // print out the value you read:
  Serial.print("line_sensor_L : ");
  Serial.println(line_sensor_L);
  Serial.print("line_sensor_R : ");
  Serial.println(line_sensor_R);
  // 블루투스시리얼[하드웨어] 프린터 알림
  Serial1.print(line_sensor_L);
  Serial1.print(" / ");
  Serial1.println(line_sensor_R);  

  // 라인센서 아날로그 신호(백색:~100, 흑색:200~)  :: 평균값임 조도에 따라서 변화 심함
  if (line_sensor_L < line_sensor_limit && line_sensor_R < line_sensor_limit)
  {
    Serial.println("전진");

    left_moter_F_fn(L_moter_max_speed);
    right_moter_F_fn(R_moter_max_speed);    
  }

  else if (line_sensor_L < line_sensor_limit && line_sensor_R > line_sensor_limit)
  {
    Serial.println("우회전");

    left_moter_F_fn(L_moter_max_speed);
    right_moter_stop_fn();
  }

  else if (line_sensor_L > line_sensor_limit && line_sensor_R < line_sensor_limit)
  {
    Serial.println("좌회전");

    left_moter_stop_fn();
    right_moter_F_fn(R_moter_max_speed);
  }

  else if (line_sensor_L > line_sensor_limit && line_sensor_R > line_sensor_limit)
  {
    Serial.println("중지");

    left_moter_stop_fn();
    right_moter_stop_fn();
  }
}


//-----
// BLE_rx_tx_fn
void BLE_rx_tx_fn()
{
  // 블루투스시리얼[하드웨어] 명령 수신
  if (Serial1.available())
  { // Serial1[하드웨어] 값이 있으면
    Serial.write(Serial1.read());       //블루투스측 내용을 시리얼모니터에 출력
  }

  // Serial.println("bluetooth_string : " + bluetooth_string);       //시리얼모니터에 bluetooth_string 값 출력
  if (bluetooth_string != "")         //bluetooth_string 값이 있다면
  { 
    // 실행(1~9)(전화버튼 기준):: ASCII

    // 1 = "49"
    if (bluetooth_string == "49")
    {
      Serial.println("좌전진");

      // BUZZER ON/OFF 함수
      button_state = 0;

      // L_moter_control_speed
      L_moter_control_speed = L_moter_min_speed + ((L_moter_max_speed - L_moter_min_speed) / 2);
      left_moter_F_fn(L_moter_control_speed);
      right_moter_F_fn(R_moter_max_speed);
    }
    // 2 = "50"
    if (bluetooth_string == "50")
    {
      Serial.println("전진");

      // BUZZER ON/OFF 함수
      button_state = 0;

      left_moter_F_fn(L_moter_max_speed);
      right_moter_F_fn(R_moter_max_speed);
    }

    // 3 = "51"
    if (bluetooth_string == "51")
    {
      Serial.println("우전진");

      // BUZZER ON/OFF 함수
      button_state = 0;

      // R_moter_control_speed
      R_moter_control_speed = R_moter_min_speed + ((R_moter_max_speed - R_moter_min_speed) / 2);
      left_moter_F_fn(L_moter_max_speed);
      right_moter_F_fn(R_moter_control_speed);
    }

    // 4 = "52"
    else if (bluetooth_string == "52")
    {
      Serial.println("좌회전");

      // BUZZER ON/OFF 함수
      button_state = 0;

      left_moter_stop_fn();
      right_moter_F_fn(R_moter_max_speed);
    }

    // 5 = "53"
    else if (bluetooth_string == "53")
    {
      Serial.println("중지");

      // BUZZER ON/OFF 함수
      button_state = 0;

      left_moter_stop_fn();
      right_moter_stop_fn();
    }
    
    // 6 = "54"
    else if (bluetooth_string == "54")
    {
      Serial.println("우회전");

      // BUZZER ON/OFF 함수
      button_state = 0;

      left_moter_F_fn(L_moter_max_speed);
      right_moter_stop_fn();
    }

    // 7 = "55"
    else if (bluetooth_string == "55")
    {
      Serial.println("좌후진");

      // BUZZER ON/OFF 함수
      button_state = 1;

      // L_moter_control_speed
      L_moter_control_speed = L_moter_min_speed + ((L_moter_max_speed - L_moter_min_speed) / 2);
      left_moter_R_fn(L_moter_control_speed);
      right_moter_R_fn(R_moter_max_speed);
    }    

    // 8 = "56"
    else if (bluetooth_string == "56")
    {
      Serial.println("후진");

      // BUZZER ON/OFF 함수
      button_state = 1;

      left_moter_R_fn(L_moter_max_speed);
      right_moter_R_fn(R_moter_max_speed);
    }

    // 9 = "57"
    else if (bluetooth_string == "57")
    {
      Serial.println("우후진");

      // BUZZER ON/OFF 함수
      button_state = 1;

      // R_moter_control_speed
      R_moter_control_speed = R_moter_min_speed + ((R_moter_max_speed - R_moter_min_speed) / 2);
      left_moter_R_fn(L_moter_max_speed);
      right_moter_R_fn(R_moter_control_speed);
    }    

    // BUZZER ON/OFF 함수
    buzzer_fn(button_state);          

  }  

/*
"No line ending [9600 baud]"
AT  (OK)
AT+NAME이름 (OK+Set:이름)
*/

  // 시리얼통신 명령 수신
  if (Serial.available())
  {
    Serial1.write(Serial.read());  // 시리얼모니터 내용을 블루투스측에 출력
  }    
}


//-----
// MOTER 구동 함수
// left_moter_F_fn
void left_moter_F_fn(int moter_pwm)
{
  // 모터 속도 셋팅
  analogWrite(left_pwm_pin, moter_pwm);

  digitalWrite(left_A_pin, HIGH);
  digitalWrite(left_B_pin, LOW);
}
// left_moter_R_fn
void left_moter_R_fn(int moter_pwm)
{
  // 모터 속도 셋팅
  analogWrite(left_pwm_pin, moter_pwm);

  digitalWrite(left_A_pin, LOW);
  digitalWrite(left_B_pin, HIGH);
}
// right_moter_F_fn
void right_moter_F_fn(int moter_pwm)
{
  // 모터 속도 셋팅
  analogWrite(right_pwm_pin, moter_pwm);

  digitalWrite(right_A_pin, HIGH);
  digitalWrite(right_B_pin, LOW);
}
// right_moter_R_fn
void right_moter_R_fn(int moter_pwm)
{
  // 모터 속도 셋팅
  analogWrite(right_pwm_pin, moter_pwm);

  digitalWrite(right_A_pin, LOW);
  digitalWrite(right_B_pin, HIGH);
}
// left_moter_stop_fn
void left_moter_stop_fn()
{
  digitalWrite(left_A_pin, LOW);
  digitalWrite(left_B_pin, LOW);
}
// right_moter_stop_fn
void right_moter_stop_fn()
{
  digitalWrite(right_A_pin, LOW);
  digitalWrite(right_B_pin, LOW);
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