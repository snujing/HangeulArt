#include "HangeulMapGenerator.h"
#include "ImageProcessor.h"


int main(int argc, const char* argv[]) {
	/* 
	* �ΰ��� locale ������ �ʿ��� ��                      
	* stream(stream buffer)�� ���ο� locale�� �ο��Ѵ�
	* �Ű������� "" ��������� �ý��� locale�� ����� 
	* (imbue �����ϴ� ) 
	*/
	wcout.imbue(locale("Korean"));

	makeHangeulArr();

	GetEveryImageBright();

	//printImageBrightPartitionMap();

	convertAVGToColor();

	//printWeightHangeulMap();

	auto hangeulMap = createHangeulMapUsingWeight();

	//printHangeul255Map();

	//printHangeul16Map();

	// ���� �̹��� ��������
	GetImage();

	// ���� ����
	adjustGamma();

	// �̹��� ���
	//convertImageToHangeul(hangeulMap);

	// �̹��� ����ȭ ���
	convertImageToHangeulInGrid(hangeulMap, getImageBrightPartitionMap());

}
