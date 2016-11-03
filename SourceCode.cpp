#include <MSGEQ7.h>
#include <Adafruit_NeoPixel.h>
#define MIN_K_value 100
#define MAX_K_value 200
int K_value = 100;
MSGEQ7 eq; //객체생
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, 6, NEO_GRB + NEO_KHZ800);

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
  pixels.begin();//네오픽셀을 사용하기 시작
  pixels.clear();//네오픽셀을 사용하기 시작
}
int cnt = 0;
void loop() {
  eq.poll();//아두이노 디지털 핀들에 들어오는 전압을 읽고 출력하는 digitalWrite함수를 이용하여 다중입출력을 제어하는 poll함수
  int max_v = 0;
  pixels.clear();
  for(int i = 0;i < 7;i++){//입력받은 신호에 따라 네오픽셀을 제어
    int l = eq.getLValue(i);
    int r = eq.getRValue(i);
    int v = (l<r? r:l);
    max_v = (v>max_v? v:max_v);
    for(int j = 0;j < 5;j++){
      if(v > K_value*(5-j)){
        on_pixel(i+(8*j));
      }
    }
  }
  for(int j = 0;j < 5;j++){//최대음량을 마지막 줄에 표시
    if(max_v > K_value*(5-j)){
      on_pixel(7+(8*j));
    }
  }
  K_value = constrain(max_v/5,MIN_K_value,MAX_K_value);//5등분 분할의 기준을 음악의 최대값의 크기에 맞게 조절
  pixels.show();
  delay(100);
}

void on_pixel(int index){
   pixels.setPixelColor(index, color_map[index]);//각위치에 맞는 색상을 선택
}
