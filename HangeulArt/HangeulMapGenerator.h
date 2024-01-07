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


// map �ʱ�ȭ
void InitMap();

// �ѱ� �ؽ�Ʈ ��� �迭�� ����ֱ�
void makeHangeulArr();

// �̹��� 2400���� ��θ� 0���� ������� �������
string makeImagePath();

// �ȼ� 1���� ��� ��� ��ȯ
int getPixelAVG(Mat& image, int x, int y);

// getPixelAVGFromHangeul�� ����� �̹��� ��ü�� ��� ��� ��ȯ
int getAVGFromHangeul(const string& imagePath);

// getAVGFromHangeul���� ���� ��� ����� rangeMax ������ ���� ����
void convertAVGToColor();

// ImageBrightPartition map�� 9��е� �̹��� ��� ����
void makeImagePartition(const string& imagePath, wchar_t ch);

// WeightHangeulMap ���
void printWeightHangeulMap();

// hangeul255Map ���
void printHangeul255Map();

// hangeul16Map ���
void printHangeul16Map();

// ImageBrightPartitionMap ���
void printImageBrightPartitionMap();

// ���� ����ϴ� Hangeul255Map�� ����
map<int, vector<wchar_t>>& createHangeulMapUsingWeight();

// �� �̹������� �ȼ� ��� ���� imageAVGArr�� ����
// 9��е� ��� ���� ImageBrightPartitionMap�� ����
void GetEveryImageBright();

map<wchar_t, vector<float>>& getImageBrightPartitionMap();