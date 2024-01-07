#include "HangeulMapGenerator.h"
#include "ImageBrightnessSum.h"

// �ؽ�Ʈ�� �̹����� ����ȭ�� ���� ���
// 0 ~ 2349����
int index = 0;

// �ڿ� index(xxxx)�� .png�� ���̸� �ȴ�
// ex) sprite_0000.png
// ex) sprite_1234.png
const string image_path = "../Data/Hangeul_PNG/sprite_";
const int imageNum = 2390;

// �� �̹��� ��� ��� �迭
int imageAVGArr[imageNum];

// �ѱ��� ������� ���� �迭 (�̹��� ������ �����ϴ�)
wchar_t hangeulArr[imageNum]; 

int rangeMax = 1000;

// 0~1000 �ѱ� ��� ��
map<int, vector<wchar_t>> weightHangeulMap;

// 0~255 (�ѱ� ��� + ��ȣ) ��
map<int, vector<wchar_t>> hangeul255Map;

// 255�� 16���� ����� map
map<int, vector<wchar_t>> hangeul16Map;

/*
* first�� �ش��ϴ� ���� �̹����� 9����ؼ� Brightness�� ����
* top left		| top	 | top right 
* middle left	| middle | middle right
* bottom left	| bottom | bottom right
*/ 
map<wchar_t, vector<float>> ImageBrightPartitionMap;

map<wchar_t, vector<float>>& getImageBrightPartitionMap() {
	return ImageBrightPartitionMap;
}

// �� �̹������� �ȼ� ��� ���� imageAVGArr�� ����
// 9��е� ��� ���� ImageBrightPartitionMap�� ����
void GetEveryImageBright() {
	cout << "GetEveryImageBright " << endl;

	// ��� �̹��� ��� ���ϱ� ���� sum ����
	int sumAVG = 0;

	for (int i = 0; i < imageNum; i++) {
		// �ڵ����� ���� �̹��� ��� ����
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
* �̹��� ��θ� ���� ��ȯ
* ȣ�� ������ index�� �˾Ƽ� ���� (1, 2, 3, ... 2399)
*/
string makeImagePath() {
	// Index ���� Ȯ��
	if (index > imageNum) { return ""; }

	// 0000���� ����
	string indexPathStr("0000");
	// index >> str
	string indexStr = to_string(index);

	// 0000 ���ڿ��� index ��ġ��
	int pathIndex = indexPathStr.size() - 1;
	for (int j = indexStr.size() - 1; j >= 0; j--, pathIndex--) {
		indexPathStr[pathIndex] = indexStr[j];
	}

	// ���� ��� ���ڿ� ����
	string indexImagePath = image_path + indexPathStr + ".png";
	index++;

	return indexImagePath;
}

/*
* �ѱ� �̹����� �ȼ�(x, y)�� ���� ä�� GrayScale�� ��ȯ
* ��ȯ = 0 ~ 255
*/
int getPixelAVG(Mat& image, int x, int y) {
	uchar b = image.at<Vec3b>(x, y)[0];
	uchar g = image.at<Vec3b>(x, y)[1];
	uchar r = image.at<Vec3b>(x, y)[2];
	/*
	* �����(���� �ֵ� ����) ȸ���� ��ȯ �߿� �ϳ�
	* �Ϲ������� �ΰ��� ���� ����� �ֵ� �ν��� ǥ��
	* �ٸ� ����ġ�� ������ ��ȯ�� ����
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
			// ��⿡ ���� Map�� ����
			hangeul255Map[index].push_back(ch);
			hangeul16Map[index / 15].push_back(ch);
		}
		index++;
	}

	//return hangeul255Map;
	return hangeul16Map;
}


// ImageBrightPartition map�� 9��е� �̹��� ��� ����
void makeImagePartition(const string& imagePath, wchar_t ch) {
	// �̹��� ����
	Mat image = imread(imagePath);

	// �̹����� ���������� ���ȴ��� Ȯ��
	if (image.empty()) {
		cerr << "makeImagePartition Error: Could not open or find the image!" << endl;
		return;
	}

	// �̹����� ���̿� �ʺ�
	int height = image.rows;
	int width = image.cols;

	// �̹����� 3x3���� ����Ͽ� �� ������ ��Ⱚ�� ���
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			// ���� ������ �������� ���� ���
			int startX = x * width / 3;
			int endX = (x + 1) * width / 3;
			int startY = y * height / 3;
			int endY = (y + 1) * height / 3;

			// ���� ������ �̹��� ����
			cv::Mat region = image(cv::Range(startY, endY), cv::Range(startX, endX));

			// ���� ������ ��Ⱚ ���
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


// hangeul16Map ���
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


// ImageBrightPartitionMap ���
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
* �̹��� 1���� ��� �ȼ� ��հ��� ���Ѵ�
*/
int getAVGFromHangeul(const string& imagePath) {
	// �̹��� ����
	Mat image = cv::imread(imagePath);

	// �̹����� ���������� ���ȴ��� Ȯ��
	if (image.empty()) {
		cerr << "Error: Could not open or find the image!" << endl;
		return -1;
	}

	// �� �ȼ� RGB ��հ� ���
	int sum = 0;
	for (int x = 0; x < image.rows; x++) {
		for (int y = 0; y < image.cols; y++) {
			sum += getPixelAVG(image, x, y);
		}
	}

	return sum;
}

/*
* ImageBrightnessSum�� ������ 0~255 ������ �����
* weightHangeulMap �迭�� ����ִ´�
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
* �ؽ�Ʈ ������ �о ���� �迭 hangeulArr�� ����ִ� �Լ�
*/
void makeHangeulArr() {
	// �ؽ�Ʈ ���� ����
	wifstream inputFile("../Data/Hangeul.txt");
	if (!inputFile) {
		cerr << "������ �� �� �����ϴ�." << endl;
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

	// ���� �ݱ�
	inputFile.close();
}