#pragma once
#include <string>
struct ConvertParam{
	std::string programDir,mapFolder,mapName,outputFilename;
	HWND window, bar;
	//void (*SetProgressBarValue)(unsigned long);
	//void (*SetProgressBarRange)(unsigned long);
};
void Convert(ConvertParam* param);
bool ShouldThreadTerminate();

#define THREAD_DONE 4