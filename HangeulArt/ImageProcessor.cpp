#include "ImageProcessor.h"
#include "HangeulMapGenerator.h"
#include <algorithm>

const int imageSize = 6;

// 원본 이미지 저장
Mat images[imageSize] = {};

// hangeul art 크기 
const int hangeulMaxSize = 120;

// 두 번 출력이 필요한 글자들 배열
vector<wchar_t> doublePrintCharacters = { U' ', U'.', U'/', U'~', U'!', U'@', U'#', U'$', U'%', U'^', U'&', U'*', U'-', U'+', U'=' };

// 원본 이미지 열기
void GetImage() {
	const string imagesPath[6] = {
		"../Data/Image/image0.jpg",
		"../Data/Image/image1.jpg",
		"../Data/Image/image2.jpg",
		"../Data/Image/image3.jpg",
		"../Data/Image/image4.jpg",
		"../Data/Image/image5.jpg"
	};
	const int imagesPathSize = sizeof(images) / sizeof(images[0]);

	for (int i = 0; i < imagesPathSize; i++) {
		// 다음 이미지를 알아서 가져옴
		Mat image = imread(imagesPath[i]);

		// 이미지가 정상적으로 열렸는지 확인
		if (image.empty()) {
			cerr << "Error: Could not open or find the image!" << endl;
			return;
		}

		// 화질, 비트수 출력
		cout << "GetImage / " << imagesPath[i] << " = ";
		cout << "row : " << image.rows << ", col : " << image.cols << endl;

		// images 배열에 저장
		images[i] = image;

	}
}


/*
* 이미지 감마 조절
* 0 ~ 25 사이 감마 값을 넣는다 (r = 1 이면 동일)
* 감마 공식 O = pow(I / 255, r) * 255
*/ 
void adjustGamma() {
	// 감마 값 ( 0 ~ 25 ) 
	int gamma = 2;

	for (auto& image : images) {
		//Mat gamma_corrected;

		// LUT 함수를 사용하여 감마 조절
		Mat lookUpTable(1, 256, CV_8U);
		uchar* p = lookUpTable.ptr();

		// 감마 공식
		for (int i = 0; i < 256; i++)
			p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

		LUT(image, lookUpTable, image);

		//imshow("Original Image", image);
		//imshow("Gamma Corrected Image", gamma_corrected);

		waitKey(0);
	}
}


/*
* 2개의 이미지 유사도를 측정, 0 ~1 사이를 반환(클수록 유사하다)
* 이미지의 크기는 동일해야함
* 2개의 이미지를 빼서 차이값을 정규화해 나타내는 방식
*/
double calculateImageSimilarity(const Mat & image1, const Mat & image2) {
	// 이미지의 크기가 다르면 유사도를 계산할 수 없음
	if (image1.size() != image2.size()) {
		return 0.0;
	}

	// 두 이미지 간의 차이를 계산
	Mat difference;
	absdiff(image1, image2, difference);

	// 차이 이미지를 그레이스케일로 변환
	cvtColor(difference, difference, cv::COLOR_BGR2GRAY);

	// 차이 이미지의 전체 픽셀 값의 합을 계산
	double totalDifference = cv::sum(difference)[0];

	// 이미지 크기를 고려하여 정규화된 유사도를 계산
	double similarity = 1.0 - (totalDifference / (image1.rows * image1.cols * 255.0));

	return similarity;
}



// 이미지 한글로 변환 
void convertImageToHangeul(map<int, vector<wchar_t>>& map) {
	int spacing;

	for (int i = 0; i < imageSize; i++) {
		if (images[i].cols >= images[i].rows) {
			spacing = images[i].cols / hangeulMaxSize;
		}
		else {
			spacing = images[i].rows / hangeulMaxSize;
		}


		for (int x = 0; x < images[i].rows; x += spacing) {
			for (int y = 0; y < images[i].cols; y += spacing) {
				int brightness = getPixelAVG(images[i], x, y);
				brightness /= 16;

				// 2번 출력이 필요한 (작은 문자들)은 1번 더 출력
				for (auto ch : doublePrintCharacters) {
					if (map[brightness][0] == ch) {
						std::wcout << ' ';
					}
				}

				//wcout << brightness;
				std::wcout << map[brightness][0];
			}
			cout << endl;
		}
		cout << "\n\n\n";
	}
}


/*
* 이미지를 9등분하여 가장 유사한 글자를 선택해 출력
*/
void convertImageToHangeulInGrid(map<int, vector<wchar_t>>& hangeulMap, map<wchar_t, vector<float>>& ImagePartitionMap) {

	int spacing;
	int totalBright;
	vector<float> imageBright(9, 0.0f);

	// 원본 이미지 루프
	for (int i = 0; i < imageSize; i++) {

		// 각 이미지 화질이 다르니 간격 계산
		if (images[i].cols >= images[i].rows) {
			spacing = images[i].cols / hangeulMaxSize;
		}
		else {
			spacing = images[i].rows / hangeulMaxSize;
		}

		cout << "images[i].rows = " << images[i].rows << endl;
		cout << "images[i].cols = " << images[i].cols << endl;
		cout << "spacing = " << spacing << endl;

		for (int y = 0; y < images[i].rows - spacing - spacing; y += spacing) {
			for (int x = 0; x < images[i].cols - spacing - spacing; x += spacing) {

				// 현재 구역의 이미지 추출
				Mat image = images[i](Rect(x, y, spacing, spacing));

				// 현재 구역의 밝기값 계산 and 저장
				totalBright = mean(image)[0];

				int gridSpacing = spacing / 3;
				int vectorIndex = 0;

				// 이미지를 3x3으로 등분하여 각 구역의 밝기값을 계산
				for (int startY = y; startY < y + (gridSpacing*3); startY += gridSpacing) {
					for (int startX = x; startX < x + (gridSpacing * 3); startX += gridSpacing) {
						// 현재 구역의 이미지 추출
						cv::Mat gridImage = images[i](cv::Range(startY, startY + gridSpacing), cv::Range(startX, startX + gridSpacing));
						
						// 현재 구역의 밝기값 계산 and 저장
						imageBright[vectorIndex] = mean(gridImage)[0];
						vectorIndex++;
					}
				}
				
				// 이미지 밝기에 맞는 문자들 찾기
				auto& chars = hangeulMap[totalBright / 16];
				
				vector<int> totalScore(chars.size(), 0);
				vector<int> score;
				
				// 찾은 map에서 각 Grid마다 비교 후 순위 매기기
				for (int i = 0; i < 9; i++) {
					score.clear();
					 
					// 각 문자들의 n번째 밝기 값 가져오기
					for (wchar_t ch : chars) {
						auto& brightVec = ImagePartitionMap[ch];
						score.push_back(255 - brightVec[i]);
					}

					// 이미지에 대해 각 글자의 밝기 차이값 계산
					// 절대값(각 문자 밝기 - 이미지 밝기)
					for (int b = 0; b < score.size(); b++) {
						score[b] = abs(score[b] - imageBright[i]);
					}

					vector<int> sortScore(score);
					sort(sortScore.begin(), sortScore.end());

					// 순위 매겨서 넣기
					for (size_t j = 0; j < score.size(); ++j) {
						auto it = find(score.begin(), score.end(), sortScore[j]);
						*it = j;
					}

					// 해당 라인 점수들을 totalScore에 합치기
					for (int a = 0; a < totalScore.size(); a++) {
						totalScore[a] += score[a];
					}

				}

				// totalScore가 가장 낮은 요소를 찾기
				auto minIter = min_element(totalScore.begin(), totalScore.end());
				int minIndex = distance(totalScore.begin(), minIter);

				// 출력
				std::wcout << chars[minIndex];

				// 2번 출력이 필요한 (작은 문자들)은 공백문자 출력
				for (auto ch : doublePrintCharacters) {
					if (chars[minIndex] == ch) {
						//wcout << map[brightness][0];
						std::wcout << ' ';
					}
				}
			}
			cout << endl;
		}
	}
}

