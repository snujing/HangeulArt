#pragma once
#include <map>
#include <vector>
#include "HangeulMapGenerator.h"
using namespace std;

// 원본 이미지 열기
void GetImage();

/*
* 이미지 감마 조절
* 0 ~ 25 사이 감마 값을 넣는다 (r = 1 이면 동일)
* 감마 공식 O = pow(I / 255, r) * 255
*/
void adjustGamma();

// 이미지 한글로 변환 
void convertImageToHangeul(map<int, vector<wchar_t>>& map);


/*
* 이미지를 9등분하여 가장 유사한 글자를 선택해 출력
*/
void convertImageToHangeulInGrid(map<int, vector<wchar_t>>& hangeulMap, map<wchar_t, vector<float>>& ImagePartitionMap);
