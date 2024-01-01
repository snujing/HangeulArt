# HangeulArt

한글 아트 이미지 넣기

- 제작기간 :
- 개발규모 : 1인 개발


# 아스키 아트 넘어 한글로

아스키로 표현된 이미지를 Ascii Art라고 부른다

기존의 영역을 재구성한 2차 창작은 새로운 분위기와 느낌을 자아내는데 

**한글을 사용해 만들면 어떨까?** 라는 생각으로 시작한 프로젝트

![image](https://github.com/snujing/HangeulArt/assets/57716676/9c727924-07f8-451b-bc23-622609c75781)



# 최대한 많은 한글을 넣어보자

목표
1. Ascii Art를 한글을 사용해 만들자
2. 최대한 많은 한글을 사용하자


# 한글 데이터 생성

실제 한국어에 사용되는 2350자를 선정

각 한글 이미지의 밝기를 측정하고 0 ~ 255 범위로 분류

![image](https://github.com/snujing/HangeulArt/assets/57716676/79429c85-9380-4e3c-b975-ee3a94aef82f)


# 초기 문제점


![image2](https://github.com/snujing/HangeulArt/assets/57716676/46c51b48-e547-4a70-b0c1-dbb7dc5dbe24)


![image](https://github.com/snujing/HangeulArt/assets/57716676/a7c98e0d-119e-460c-9043-07684f7642bd) 


가장 어두운 문자가 '스', '그', '느' 등이 었는데

**검정을 표현하기에 문자가 너무 밝다**는 것이다

따라서 공백문자와 기호 몇가지를 추가해보았다


# 공백문자와 기호 추가

공백문자와 기호가 한글의 절반 크기만 차지한다는 사실을 깨달았다

doublePrintCharacters라는 배열을 만들어 따로 관리해 출력해주었다

![image](https://github.com/snujing/HangeulArt/assets/57716676/d76b12cf-ef30-4815-9783-c1baa16742c5)

어두운 부분이 좀더 자연스러워졌다

하지만 너무 밝다

왜 이렇게 밝은지 계산은 해보았다

<img src="https://github.com/snujing/HangeulArt/assets/57716676/2a112493-0c19-4c88-bc5f-b856d3188099" width="1000px">

각 이미지를 0 ~ 255 범위로 표현해봤는데

대부분의 글자가 170 ~ 200 사이에 분포한다는 문제점을 알았다

적어도 0 ~ 140 범위에 대해 다른 방법을 찾아봐야한다









