#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <string>
#include <map>
#include <vector>

using namespace std;
using namespace cv;


// map 초기화
void InitMap();

// 한글 텍스트 열어서 배열에 집어넣기
void makeHangeulArr();

// 이미지 2400개의 경로를 0부터 순서대로 만들어줌
string makeImagePath();

// 픽셀 1개의 밝기 평균 반환
int getPixelAVG(Mat& image, int x, int y);

// getPixelAVGFromHangeul를 사용해 이미지 전체의 밝기 평균 반환
int getAVGFromHangeul(const string& imagePath);

// getAVGFromHangeul에서 만든 밝기 평균을 rangeMax 범위에 따라 변경
void convertAVGToColor();

// ImageBrightPartition map에 9등분된 이미지 밝기 저장
void makeImagePartition(const string& imagePath, wchar_t ch);

// WeightHangeulMap 출력
void printWeightHangeulMap();

// hangeul255Map 출력
void printHangeul255Map();

// hangeul16Map 출력
void printHangeul16Map();

// ImageBrightPartitionMap 출력
void printImageBrightPartitionMap();

// 실제 사용하는 Hangeul255Map를 생성
map<int, vector<wchar_t>>& createHangeulMapUsingWeight();

// 각 이미지들의 픽셀 밝기 합을 imageAVGArr에 저장
// 9등분된 밝기 값은 ImageBrightPartitionMap에 저장
void GetEveryImageBright();

map<wchar_t, vector<float>>& getImageBrightPartitionMap();