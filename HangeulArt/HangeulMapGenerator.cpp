#include "HangeulMapGenerator.h"
#include "ImageBrightnessSum.h"

// 텍스트와 이미지의 동기화를 위해 사용
// 0 ~ 2349까지
int index = 0;

// 뒤에 index(xxxx)와 .png를 붙이면 된다
// ex) sprite_0000.png
// ex) sprite_1234.png
const string image_path = "../Data/Hangeul_PNG/sprite_";
const int imageNum = 2390;

// 각 이미지 밝기 평균 배열
int imageAVGArr[imageNum];

// 한글을 순서대로 담은 배열 (이미지 순서와 동일하다)
wchar_t hangeulArr[imageNum]; 

int rangeMax = 1000;

// 0~1000 한글 밝기 맵
map<int, vector<wchar_t>> weightHangeulMap;

// 0~255 (한글 밝기 + 기호) 맵
map<int, vector<wchar_t>> hangeul255Map;

// 255를 16으로 등분한 map
map<int, vector<wchar_t>> hangeul16Map;

/*
* first에 해당하는 글자 이미지를 9등분해서 Brightness를 저장
* top left		| top	 | top right 
* middle left	| middle | middle right
* bottom left	| bottom | bottom right
*/ 
map<wchar_t, vector<float>> ImageBrightPartitionMap;

map<wchar_t, vector<float>>& getImageBrightPartitionMap() {
	return ImageBrightPartitionMap;
}

// 각 이미지들의 픽셀 밝기 합을 imageAVGArr에 저장
// 9등분된 밝기 값은 ImageBrightPartitionMap에 저장
void GetEveryImageBright() {
	cout << "GetEveryImageBright " << endl;

	// 모든 이미지 평균 구하기 위한 sum 변수
	int sumAVG = 0;

	for (int i = 0; i < imageNum; i++) {
		// 자동으로 다음 이미지 경로 생성
		string imagePath = makeImagePath();

		int colorAVG = getAVGFromHangeul(imagePath);
		imageAVGArr[i] = colorAVG;
		sumAVG += colorAVG;

		makeImagePartition(imagePath, hangeulArr[i]);
	}
	cout << "\n\n\n" << endl;
	for (int i = 0; i < imageNum; i++) {
		cout << imageAVGArr[i] << ", ";
	}
}


void InitMap() {
	for (int i = 0; i <= rangeMax; i++) {
		weightHangeulMap.insert(make_pair(i, vector<wchar_t>{}));
	}

	for (int i = 0; i <= 255; i++) {
		hangeul255Map.insert(make_pair(i, vector<wchar_t>{}));
	}
}


/*
* 이미지 경로를 만들어서 반환
* 호출 때마다 index가 알아서 증가 (1, 2, 3, ... 2399)
*/
string makeImagePath() {
	// Index 조건 확인
	if (index > imageNum) { return ""; }

	// 0000에서 만듬
	string indexPathStr("0000");
	// index >> str
	string indexStr = to_string(index);

	// 0000 문자열에 index 합치기
	int pathIndex = indexPathStr.size() - 1;
	for (int j = indexStr.size() - 1; j >= 0; j--, pathIndex--) {
		indexPathStr[pathIndex] = indexStr[j];
	}

	// 최종 경로 문자열 생성
	string indexImagePath = image_path + indexPathStr + ".png";
	index++;

	return indexImagePath;
}

/*
* 한글 이미지의 픽셀(x, y)을 단일 채널 GrayScale로 변환
* 반환 = 0 ~ 255
*/
int getPixelAVG(Mat& image, int x, int y) {
	uchar b = image.at<Vec3b>(x, y)[0];
	uchar g = image.at<Vec3b>(x, y)[1];
	uchar r = image.at<Vec3b>(x, y)[2];
	/*
	* 비색법(지각 휘도 보존) 회색조 변환 중에 하나
	* 일반적으로 인간이 보는 삼색성 휘도 인식을 표현
	* 다른 가중치를 가지는 변환이 존재
	*/
	int gray = (r * 2126 + g * 7152 + b * 722) / 10000;
	return gray;
}


map<int, vector<wchar_t>>& createHangeulMapUsingWeight() {
	cout << "createHangeul255UsingWeightHangeul" << endl;
	int index = 0;
	
	for (auto m : weightHangeulMap) {

		if (m.second.size() == 0) {
			continue;
		}
		for (auto ch : m.second) {
			// 밝기에 따라 Map에 저장
			hangeul255Map[index].push_back(ch);
			hangeul16Map[index / 15].push_back(ch);
		}
		index++;
	}

	//return hangeul255Map;
	return hangeul16Map;
}


// ImageBrightPartition map에 9등분된 이미지 밝기 저장
void makeImagePartition(const string& imagePath, wchar_t ch) {
	// 이미지 열기
	Mat image = imread(imagePath);

	// 이미지가 정상적으로 열렸는지 확인
	if (image.empty()) {
		cerr << "makeImagePartition Error: Could not open or find the image!" << endl;
		return;
	}

	// 이미지의 높이와 너비
	int height = image.rows;
	int width = image.cols;

	// 이미지를 3x3으로 등분하여 각 구역의 밝기값을 계산
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			// 현재 구역의 시작점과 끝점 계산
			int startX = x * width / 3;
			int endX = (x + 1) * width / 3;
			int startY = y * height / 3;
			int endY = (y + 1) * height / 3;

			// 현재 구역의 이미지 추출
			cv::Mat region = image(cv::Range(startY, endY), cv::Range(startX, endX));

			// 현재 구역의 밝기값 계산
			ImageBrightPartitionMap[ch].push_back(mean(region)[0]);
		}
	}
}


void printWeightHangeulMap() {
	cout << "printWeightHangeulMap" << endl;
	int vailIndexCount = 0;

	for (auto m : weightHangeulMap) {
		cout << m.first << " = ";
		if (m.second.size() > 0) {
			vailIndexCount++;
		}
		for (auto ch : m.second) {
			wcout << ch << " ";
		}
		cout << endl;
	}

	cout << "vailIndexCount" << vailIndexCount << endl;
}


void printHangeul255Map() {
	cout << "printHangeul255Map" << endl;

	for (auto m : hangeul255Map) {
		cout << m.first << " = ";

		for (auto ch : m.second) {
			wcout << ch << " ";
		}
		cout << endl;
	}
}


// hangeul16Map 출력
void printHangeul16Map() {
	cout << "printHangeul16Map" << endl;

	for (auto m : hangeul16Map) {
		cout << m.first << " = ";

		for (auto ch : m.second) {
			wcout << ch << " ";
		}
		cout << endl;
	}
}


// ImageBrightPartitionMap 출력
void printImageBrightPartitionMap() {
	cout << "printImageBrightPartitionMap" << endl;

	for (auto map : ImageBrightPartitionMap) {
		wcout << "ch : " << map.first << " = ";

		for (float bright : map.second) {
			cout << bright << ", ";
		}
		cout << endl;
	}
}



/*
* 이미지 1개의 모든 픽셀 평균값을 구한다
*/
int getAVGFromHangeul(const string& imagePath) {
	// 이미지 열기
	Mat image = cv::imread(imagePath);

	// 이미지가 정상적으로 열렸는지 확인
	if (image.empty()) {
		cerr << "Error: Could not open or find the image!" << endl;
		return -1;
	}

	// 각 픽셀 RGB 평균값 얻기
	int sum = 0;
	for (int x = 0; x < image.rows; x++) {
		for (int y = 0; y < image.cols; y++) {
			sum += getPixelAVG(image, x, y);
		}
	}

	return sum;
}

/*
* ImageBrightnessSum의 값들을 0~255 범위로 만들고
* weightHangeulMap 배열에 집어넣는다
*/
void convertAVGToColor() {

	for (int i = 0; i < imageNum; i++) {
		int color = ImageBrightnessSum[i] * rangeMax / 96900;
		color = 255 - color;
		//int color = 96900 *1000 / ImageBrightnessSum[i];

		if (color == rangeMax) {
			cout << "color " << rangeMax << " = " << ImageBrightnessSum[i];
		}

		weightHangeulMap[color].push_back(hangeulArr[i]);
	}


}

/*
* 텍스트 파일을 읽어서 전역 배열 hangeulArr에 집어넣는 함수
*/
void makeHangeulArr() {
	// 텍스트 파일 열기
	wifstream inputFile("../Data/Hangeul.txt");
	if (!inputFile) {
		cerr << "파일을 열 수 없습니다." << endl;
		return;
	}

	inputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));

	int i_hangeulArr = 0;
	wchar_t ch;
	while (inputFile.get(ch)) {
		if (ch == '\n') {
			continue;
		}
		hangeulArr[i_hangeulArr++] = ch;

		
		ImageBrightPartitionMap.insert(make_pair(ch, vector<float>()));
	}

	// 파일 닫기
	inputFile.close();
}