#include "HangeulMapGenerator.h"
#include "ImageProcessor.h"


int main(int argc, const char* argv[]) {
	/* 
	* 민감한 locale 연산이 필요할 때                      
	* stream(stream buffer)에 새로운 locale을 부여한다
	* 매개변수를 "" 비워놓으면 시스템 locale이 적용됨 
	* (imbue 주입하다 ) 
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

	// 원본 이미지 가져오기
	GetImage();

	// 감마 조정
	adjustGamma();

	// 이미지 출력
	//convertImageToHangeul(hangeulMap);

	// 이미지 최적화 출력
	convertImageToHangeulInGrid(hangeulMap, getImageBrightPartitionMap());

}
