#include "ImageProcessor.h"
#include "HangeulMapGenerator.h"
#include <algorithm>

const int imageSize = 6;

// ���� �̹��� ����
Mat images[imageSize] = {};

// hangeul art ũ�� 
const int hangeulMaxSize = 120;

// �� �� ����� �ʿ��� ���ڵ� �迭
vector<wchar_t> doublePrintCharacters = { U' ', U'.', U'/', U'~', U'!', U'@', U'#', U'$', U'%', U'^', U'&', U'*', U'-', U'+', U'=' };

// ���� �̹��� ����
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
		// ���� �̹����� �˾Ƽ� ������
		Mat image = imread(imagesPath[i]);

		// �̹����� ���������� ���ȴ��� Ȯ��
		if (image.empty()) {
			cerr << "Error: Could not open or find the image!" << endl;
			return;
		}

		// ȭ��, ��Ʈ�� ���
		cout << "GetImage / " << imagesPath[i] << " = ";
		cout << "row : " << image.rows << ", col : " << image.cols << endl;

		// images �迭�� ����
		images[i] = image;

	}
}


/*
* �̹��� ���� ����
* 0 ~ 25 ���� ���� ���� �ִ´� (r = 1 �̸� ����)
* ���� ���� O = pow(I / 255, r) * 255
*/ 
void adjustGamma() {
	// ���� �� ( 0 ~ 25 ) 
	int gamma = 2;

	for (auto& image : images) {
		//Mat gamma_corrected;

		// LUT �Լ��� ����Ͽ� ���� ����
		Mat lookUpTable(1, 256, CV_8U);
		uchar* p = lookUpTable.ptr();

		// ���� ����
		for (int i = 0; i < 256; i++)
			p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

		LUT(image, lookUpTable, image);

		//imshow("Original Image", image);
		//imshow("Gamma Corrected Image", gamma_corrected);

		waitKey(0);
	}
}


/*
* 2���� �̹��� ���絵�� ����, 0 ~1 ���̸� ��ȯ(Ŭ���� �����ϴ�)
* �̹����� ũ��� �����ؾ���
* 2���� �̹����� ���� ���̰��� ����ȭ�� ��Ÿ���� ���
*/
double calculateImageSimilarity(const Mat & image1, const Mat & image2) {
	// �̹����� ũ�Ⱑ �ٸ��� ���絵�� ����� �� ����
	if (image1.size() != image2.size()) {
		return 0.0;
	}

	// �� �̹��� ���� ���̸� ���
	Mat difference;
	absdiff(image1, image2, difference);

	// ���� �̹����� �׷��̽����Ϸ� ��ȯ
	cvtColor(difference, difference, cv::COLOR_BGR2GRAY);

	// ���� �̹����� ��ü �ȼ� ���� ���� ���
	double totalDifference = cv::sum(difference)[0];

	// �̹��� ũ�⸦ ����Ͽ� ����ȭ�� ���絵�� ���
	double similarity = 1.0 - (totalDifference / (image1.rows * image1.cols * 255.0));

	return similarity;
}



// �̹��� �ѱ۷� ��ȯ 
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

				// 2�� ����� �ʿ��� (���� ���ڵ�)�� 1�� �� ���
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
* �̹����� 9����Ͽ� ���� ������ ���ڸ� ������ ���
*/
void convertImageToHangeulInGrid(map<int, vector<wchar_t>>& hangeulMap, map<wchar_t, vector<float>>& ImagePartitionMap) {

	int spacing;
	int totalBright;
	vector<float> imageBright(9, 0.0f);

	// ���� �̹��� ����
	for (int i = 0; i < imageSize; i++) {

		// �� �̹��� ȭ���� �ٸ��� ���� ���
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

				// ���� ������ �̹��� ����
				Mat image = images[i](Rect(x, y, spacing, spacing));

				// ���� ������ ��Ⱚ ��� and ����
				totalBright = mean(image)[0];

				int gridSpacing = spacing / 3;
				int vectorIndex = 0;

				// �̹����� 3x3���� ����Ͽ� �� ������ ��Ⱚ�� ���
				for (int startY = y; startY < y + (gridSpacing*3); startY += gridSpacing) {
					for (int startX = x; startX < x + (gridSpacing * 3); startX += gridSpacing) {
						// ���� ������ �̹��� ����
						cv::Mat gridImage = images[i](cv::Range(startY, startY + gridSpacing), cv::Range(startX, startX + gridSpacing));
						
						// ���� ������ ��Ⱚ ��� and ����
						imageBright[vectorIndex] = mean(gridImage)[0];
						vectorIndex++;
					}
				}
				
				// �̹��� ��⿡ �´� ���ڵ� ã��
				auto& chars = hangeulMap[totalBright / 16];
				
				vector<int> totalScore(chars.size(), 0);
				vector<int> score;
				
				// ã�� map���� �� Grid���� �� �� ���� �ű��
				for (int i = 0; i < 9; i++) {
					score.clear();
					 
					// �� ���ڵ��� n��° ��� �� ��������
					for (wchar_t ch : chars) {
						auto& brightVec = ImagePartitionMap[ch];
						score.push_back(255 - brightVec[i]);
					}

					// �̹����� ���� �� ������ ��� ���̰� ���
					// ���밪(�� ���� ��� - �̹��� ���)
					for (int b = 0; b < score.size(); b++) {
						score[b] = abs(score[b] - imageBright[i]);
					}

					vector<int> sortScore(score);
					sort(sortScore.begin(), sortScore.end());

					// ���� �Űܼ� �ֱ�
					for (size_t j = 0; j < score.size(); ++j) {
						auto it = find(score.begin(), score.end(), sortScore[j]);
						*it = j;
					}

					// �ش� ���� �������� totalScore�� ��ġ��
					for (int a = 0; a < totalScore.size(); a++) {
						totalScore[a] += score[a];
					}

				}

				// totalScore�� ���� ���� ��Ҹ� ã��
				auto minIter = min_element(totalScore.begin(), totalScore.end());
				int minIndex = distance(totalScore.begin(), minIter);

				// ���
				std::wcout << chars[minIndex];

				// 2�� ����� �ʿ��� (���� ���ڵ�)�� ���鹮�� ���
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

