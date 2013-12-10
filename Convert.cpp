#include "StdAfx.h"
#include "Convert.h"
#include "NBT.h"
#include "Data.h"
#include "zlib/zlib.h"

using namespace std;
HWND bar;

//void (*SetProgressBarValue)(unsigned long);
//void (*SetProgressBarRange)(unsigned long);


void SetProgressBarValue(unsigned long value){
	if (ShouldThreadTerminate()) throw runtime_error("");
	PostMessage(bar, PBM_SETPOS, value, 0);
}

void SetProgressBarRange(unsigned long value){
	PostMessage(bar, PBM_SETRANGE32, 0, value);
}

static void Write(ofstream& file, unsigned char c){
	switch(c){
		case '\\': file.write("\\\\",2); break;
		case 255U: file.write("\\0",2); break;
		case 0: file.put(255U); break;
		case '"': file.write("&quot;",6); break;
		case '\'': file.write("&apos;",6); break;
		case '<': file.write("&lt;",4); break;
		case '>': file.write("&gt;",4); break;
		case '&': file.write("&amp;", 5); break;
		default:
			if (c >= 128 && c <= 160){
				file.put('\\');
				c -= 127;
			}
			file.put(c);
	}
}

static void WriteInt(ofstream& file, int num){ //big endian

	unsigned int newNum = abs(num);
	if (num < 0) newNum |= 0x80000000;
	Write(file, (newNum>>24)&0xff);
	Write(file, (newNum>>16)&0xff);
	Write(file, (newNum>>8)&0xff);
	Write(file, newNum&0xff);
}

static unsigned int GetNum(ifstream& file, unsigned char digits, bool bigEndian = true){
	unsigned int result = 0;
	for (unsigned char i = 0; i < digits; i++){
		if (bigEndian) result = result*256 + file.get();
		else result = result + (file.get()*(i*256));
	}
	return result;
}

template<typename T>

class SafeArray{
public:
	T* data;
	SafeArray(SafeArray&& right){
		data = right.data;
		right.data = 0;
	}
	void Kill(){
		if (data) delete[] data;
		data = 0;
	}
	T operator[](size_t index){
		if (data) return data[index];
	}
	SafeArray(size_t length){
		data = new T[length];
	}
	~SafeArray(){
		if (data) delete[] data;
	}
};

static SafeArray<unsigned char> ReadAndUncompressData(ifstream& file, unsigned long compressedDataLen){
	unsigned long dataLen = 0xFFFFF;
	SafeArray<unsigned char> data(dataLen);
	SafeArray<unsigned char> compressedData(compressedDataLen);
	file.read((char*)compressedData.data, compressedDataLen);
	const int err = uncompress(data.data, &dataLen, compressedData.data, compressedDataLen);
	switch(err){
		case Z_MEM_ERROR: throw runtime_error("Error reading map: Out of memory!");
		case Z_BUF_ERROR: throw logic_error("Error reading map: Destination buffer size too small!");
		case Z_DATA_ERROR: throw runtime_error("Error reading map: Invalid map format!"); 
	}
	return data;
}

static void ConvertRegion(string filePath, string filename, ofstream& outputFile, unsigned int count, unsigned int total){
	const auto firstDot = filename.find_first_of('.');
	const auto secondDot = filename.find_first_of('.',firstDot+1);
	const auto thirdDot = filename.find_first_of('.',secondDot+1);

	const int regionX = atoi(filename.substr(firstDot+1,secondDot-firstDot-1).c_str());
	const int regionY = atoi(filename.substr(secondDot+1,thirdDot-secondDot-1).c_str());

	ifstream file(filePath+filename,ios::in|ios::binary);
	if (!file.good()) throw runtime_error("Error reading map: Failed to open region file!");

	bool seek = false;
	unsigned int last = 100*count/total;
	for (unsigned int chunkCount = 0; chunkCount < 1024; chunkCount++){ //chunk locations
		if (seek){
			file.seekg(chunkCount*4,ios_base::beg);
			seek = false;
		}
		//const unsigned char chunkZ = i/32;
		//const unsigned char chunkX = i%32;
		const unsigned int offset = GetNum(file, 3); //byte offset from start of file of chunk
		const unsigned char sectors = file.get(); //bytes in each chunk
		if (offset == 0 && sectors == 0) continue;
		else if (offset < 2) throw runtime_error("Error reading map: Invalid map format!");
		seek = true;
		file.seekg(((std::streampos)offset)*4096, ios_base::beg);
		const unsigned int chunkLength = GetNum(file,4)-1;
		const unsigned char compressionFormat = file.get();
		if (compressionFormat != 2) throw runtime_error("Error reading map: Invalid map format!");

		SafeArray<unsigned char> data = ReadAndUncompressData(file,chunkLength);
		const unsigned char* ptr = data.data;

#ifdef SIMPLE_NBT
		WorldInfo chunk(ptr);
#else
		NBT_Tag chunk(ptr); //parse nbt
#endif

		data.Kill();

#ifdef SIMPLE_NBT
		const int chunkX = chunk.x;
		const int chunkZ = chunk.z;
		vector<ChunkSection>& sections = chunk.sections;
#else
		NBT_Tag* const level = chunk["Level"];
		auto sections = ((NBT_List*)(((*level)["Sections"])->payload))->data;

		const int chunkX = ((NBT_Int*)((*level)["xPos"]->payload))->data;
		const int chunkZ = ((NBT_Int*)((*level)["zPos"]->payload))->data;
#endif

		vector<unsigned char> blocks(16*16*256);
		//std::sort(sections.begin(), sections.end(), [](NBT_Payload* a, NBT_Payload* b){return ((NBT_Byte*)((*((NBT_Compound*)a))["Y"]->payload))->data > ((NBT_Byte*)((*((NBT_Compound*)b))["Y"]->payload))->data;});
		const auto end = sections.end();
		for (auto sectionCount = sections.begin(); sectionCount != end; sectionCount++){
			const auto& section =
#ifdef SIMPLE_NBT
				*sectionCount;
			const int chunkY = section.y;
			auto& blockBytes = section.blocks;
#else
			(NBT_Compound*)*sectionCount;
			const int chunkY = ((NBT_Byte*)(((*section)["Y"])->payload))->data;
			auto& blockBytes = ((NBT_Byte_Array*)(((*section)["Blocks"])->payload))->data;
#endif
			for (unsigned char y = 0; y < 16; y++){
				for (unsigned char z = 0; z < 16; z++){
					for (unsigned char x = 0; x < 16; x++){
						blocks[(y+chunkY*16)+x*256+z*256*16] = blockBytes[y*16*16+z*16+x];
					}
				}
			}
		}
		//Write(outputFile,254);
		WriteInt(outputFile, chunkX);
		WriteInt(outputFile, chunkZ);

		/*
		for (unsigned int x = 0; x < 16*256*16; x+=256*16){
			for (unsigned int z = 0; z < 16*256; z+= 256){
				unsigned char lastBlock;
				unsigned char numBlocks;
				unsigned short y = 0;

				while (y < 255){
					lastBlock = blocks[x+z+(y++)];
					numBlocks = 1;
					Write(outputFile,lastBlock);
					while (y < 255 && blocks[x+z+y] == lastBlock){
						numBlocks++;
						y++;
					}
					Write(outputFile,numBlocks);
				}
			}
		}*/
		for (unsigned int column = 0; column < 16*16*256; column+= 256){ //for each column
			unsigned short y = 0;
			while (y < 255){
				const unsigned char lastBlock = blocks[column+y];
				unsigned char numBlocks = 0;
				Write(outputFile, lastBlock);
				while (y < 255 && blocks[column+y] == lastBlock){
					if (numBlocks++ < numBlocks) throw logic_error("Error reading map: Block overflow!");
					y++;
				}
				if (numBlocks == 0){
					throw logic_error("Error reading map: Tried to write an flat column of blocks!");
				}
				Write(outputFile, numBlocks);
			}
		}
		SetProgressBarValue(count*1024+chunkCount);//SendMessage(progressBar, PBM_SETPOS, count*1024+chunkCount,0);
		const unsigned int n = 100*(count*1024+chunkCount)/(1024*total);
		if (n != last){
			last = n;
		}
	}
	SetProgressBarValue((count+1)*1024);//SendMessage(progressBar, PBM_SETPOS, (count+1)*1024,0);
	//cout<<100*((count+1)*1024)/(1024*total)<<"%\n";
}


vector<string> GetFilesInDir(string dir, string extension, int mask = ~0){
	vector<string> result;
	WIN32_FIND_DATA findData;
	string a = dir+"*"+extension;
	HANDLE fileFind = FindFirstFile((dir+"*"+extension).c_str(), &findData);
	if (fileFind == INVALID_HANDLE_VALUE)return result;
	
	const string upDir("..");
	do {
		string filename(findData.cFileName);
		if ((findData.dwFileAttributes&mask) && filename != upDir){
			result.push_back(filename);
		};
	} while (FindNextFile(fileFind, &findData) != 0);
	FindClose(fileFind);
	return result;
}

void Convert(ConvertParam* param){//string programDir, string mapFolder, string mapName, string outputFilename, HWND window, HWND bar){
	string programDir = param->programDir;
	string mapFolder = param->mapFolder;
	string mapName = param->mapName;
	string outputFilename = param->outputFilename;
	bar = param->bar;
	//SetProgressBarValue = param->SetProgressBarValue;
	//SetProgressBarRange = param->SetProgressBarRange;
	HWND window = param->window;
	delete param;
	try{
		vector<string> regions = GetFilesInDir(mapFolder, ".mca", FILE_ATTRIBUTE_ARCHIVE);
		if (regions.size() == 0) throw runtime_error("Map is empty!");

		programDir.resize(programDir.find_last_of('\\')+1);
		ofstream outputFile(outputFilename,ios::out|ios::binary);
		outputFile.write(fileStart, sizeof(fileStart)-1);

		unsigned int count = 0;
		const unsigned int total = regions.size();
		SetProgressBarRange(1024*total);
		for (auto i = regions.begin(); i != regions.end(); i++){
			ConvertRegion(mapFolder, *i, outputFile, count++, total);
		}
		outputFile.write(fileEnd, sizeof(fileEnd)-1);
		outputFile.close();
		if (!outputFile.good()) throw runtime_error("Failed to save roblox place!\nDo you have enough disk space?");
		MessageBox(window, "Completed successfully!", "Done!", MB_ICONINFORMATION);
	} catch (exception &e){
		//SetProgressBarValue(0);//SendMessage(progressBar, PBM_SETPOS, 0, 0);
		string msg= e.what();
		if (msg != "") MessageBox(window, e.what(), "Error!", MB_ICONWARNING);
	}
	PostMessage(window,WM_USER,THREAD_DONE,0);
}