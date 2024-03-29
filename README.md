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


# 범위 변경 / 감마 조정 / 모음, 자음 추가

범위를 1마다 쪼갰었는데 문제가 크다

밝기 n과 n+1의 문자가 다르니 가독성이 매우 떨어지는 문제점이 있어서 범위를 16씩 합쳤다

감마 조정 함수도 추가해 사진 밝기 평균을 따져서 적용된다

모음, 자음이 추가되었는데 위에서 필요했던 0 ~ 140 범위의 밝기를 어느정도 해결했다

![image](https://github.com/snujing/HangeulArt/assets/57716676/61acd91b-d272-46e8-961b-72ba9587b34a)

(이걸 향상이라 해야하나...? 달라지기는 했다)


# 같은 범위 내에서 최적의 문자 찾기

범위를 16씩 합쳐서 가독성을 좋아졌지만 

**수많은 문자 중에서 어떤 문자를 선택할 것인가**라는 문제가 더욱 두각되었다

처음에는 원본 이미지와 한글 이미지 유사도 측정을 하려고 시도했지만 문제점이 있었다

1. 이미지 vs 이미지를 비교하는 것이기 때문에 비용이 높다
2. 모든 한글 이미지를 가지고 있거나 상황마다 다시 가져와야한다
3. 비교 함수이므로 미리 계산하고 저장할 수 없다 

최종적으로는 정확도를 낮추고 계산을 저장해 비용을 낮추는 방법을 선택했다

![image](https://github.com/snujing/HangeulArt/assets/57716676/596cd544-7bcb-4163-b4fe-cf2f23107c00)

미리 한글 이미지를 9등분하여 각 밝기 값을 저장하고

원본 이미지 또한 9등분하여 절대값(한글 밝기 - 원본 밝기) 계산으로 등수를 매겨 

가장 유사한 글자를 선택하는 방법을 구현했다

따라서 **밝기가 비슷한 문자들 중에서 9등분한 이미지의 차이값이 가장 작은 이미지 선택함으로써 가장 유사한 글자를 찾아낼 수 있었다**



# 최종 결과

비교 계산이 들어감으로 원본 이미지의 화질이 높아야 좋은 결과물이 나왔다

![image](https://github.com/snujing/HangeulArt/assets/57716676/b38ced5b-533b-4c86-83a8-bff52cce8adf)


![image](https://github.com/snujing/HangeulArt/assets/57716676/1044a649-a9a6-4304-849b-9a1eb961c5cd)


![image](https://github.com/snujing/HangeulArt/assets/57716676/24c719e7-58c4-4dd3-a3c2-b9b5698c3630)
