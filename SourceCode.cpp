#include <MSGEQ7.h>
#include <Adafruit_NeoPixel.h>
#define MIN_K_value 100
#define MAX_K_value 200
int K_value = 100;
MSGEQ7 eq;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, 6, NEO_GRB + NEO_KHZ800); //네오픽셀LED의 개수, 네오픽셀이 연결된 아두이노의 핀번호, 네오픽셀의 타입에 따라 바뀌는 flag

uint32_t color_map[40] =
{ //위치별 픽셀 색상설정. 
  0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x005050af,
  0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x005050af,
  0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x005050af,
  0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x005050af,
  0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x00efef00,0x005050af
}
;

void setup() {
  eq.init();//스펙트럼 쉴드를 사용하기 시작
  pixels.begin();//네오픽셀을 초기화 하기위해 모든 LED를 OFF 시킴
  pixels.clear();//네오픽셀을 사용하기 시작
}
int cnt = 0;
void loop() {
  eq.poll();//아두이노 디지털 핀들에 들어오는 전압을 읽고 출력하는 digitalWrite함수를 이용하여 다중입출력을 제어하는 poll함수
  int max_v = 0;
  pixels.clear();
  for(int i = 0;i < 7;i++){//입력받은 신호에 따라 네오픽셀을 제어
     //네오픽셀의 7번째 열까지 반복
    int l = eq.getLValue(i); //i번째의 spectrumLvalues 값
    int r = eq.getRValue(i); //i번째의 spectrumRvalues 값
    int v = (l<r? r:l); // l과r중 큰갑을 v에 저장
    max_v = (v>max_v? v:max_v); //v와 max_v중 큰값을 max_v에 저장
    for(int j = 0;j < 5;j++){ //네오픽셀의 5번째 행까지 반복
      if(v > K_value*(5-j)){ //v값이 K_value*(5-j)보다 크면
        on_pixel(i+(8*j)); //color_map[i+(8*j)]에 설정된 값으로 설정?????????????????????????
      }
    }
  }
   //마지막 열을 표시한다 (5행 반복)
  for(int j = 0;j < 5;j++){//최대음량을 마지막 줄에 표시
    if(max_v > K_value*(5-j)){ //저장한 max_v값이 K_value*(5-j)값보다 크면
      on_pixel(7+(8*j)); //color_map[7+(8*j)]에 설정된 값으로 설정????????????????????????
    }
  }
  K_value = constrain(max_v/5,MIN_K_value,MAX_K_value);//5등분 분할의 기준을 음악의 최대값의 크기에 맞게 조절(변수 max_v/5의 값을 MIN_K_value~MAX_K_value 사이의 값으로 한정한다.)
  pixels.show(); //코드에서 작성한 명령을 NeoPixel로 넘겨 실제로 LED를 켜고 끄는 명령을 하게 하는 함수
  delay(100); // 0.1 초 딜레이를 준다.
}

void on_pixel(int index){
   pixels.setPixelColor(index, color_map[index]);//각위치에 맞는 색상을 선택
}
