#pragma once
#include <map>
#include <vector>
#include "HangeulMapGenerator.h"
using namespace std;

// ���� �̹��� ����
void GetImage();

/*
* �̹��� ���� ����
* 0 ~ 25 ���� ���� ���� �ִ´� (r = 1 �̸� ����)
* ���� ���� O = pow(I / 255, r) * 255
*/
void adjustGamma();

// �̹��� �ѱ۷� ��ȯ 
void convertImageToHangeul(map<int, vector<wchar_t>>& map);


/*
* �̹����� 9����Ͽ� ���� ������ ���ڸ� ������ ���
*/
void convertImageToHangeulInGrid(map<int, vector<wchar_t>>& hangeulMap, map<wchar_t, vector<float>>& ImagePartitionMap);
