#pragma once
#include <string>
#include <vector>

#define SIMPLE_NBT



#ifdef SIMPLE_NBT




class ChunkSection{
public:
	unsigned char y;
	std::vector<unsigned char> blocks;
	ChunkSection();
	ChunkSection(ChunkSection&& other) throw();
};
class WorldInfo{
public:
	int x;
	int z;
	std::vector<ChunkSection> sections;
	WorldInfo(const unsigned char*& data);
};
#else

class NBT_Payload{
public:
	virtual ~NBT_Payload();
};

class NBT_Tag{
public:
	unsigned char id;
	std::string name;
	NBT_Tag* operator[](std::string child);
	NBT_Payload* payload;
	NBT_Tag(const unsigned char*& data);
	~NBT_Tag();
};

class NBT_Byte:public NBT_Payload{
public:
	signed char data;
	NBT_Byte(const unsigned char*& data);
};
class NBT_Short:public NBT_Payload{
public:
	signed short data;
	NBT_Short(const unsigned char*& data);
};
class NBT_Int:public NBT_Payload{
public:
	signed int data;
	NBT_Int(const unsigned char*& data);
};
class NBT_Long:public NBT_Payload{
public:
	signed long long data;
	NBT_Long(const unsigned char*& data);
};
class NBT_Float:public NBT_Payload{
public:
	float data;
	NBT_Float(const unsigned char*& data);
};
class NBT_Double:public NBT_Payload{
public:
	double data;
	NBT_Double(const unsigned char*& data);
};
class NBT_Byte_Array:public NBT_Payload{
public:
	std::vector<signed char> data;
	NBT_Byte_Array(const unsigned char*& data);
};
class NBT_String:public NBT_Payload{
public:
	std::string data;
	NBT_String(const unsigned char*& data);
};
class NBT_List:public NBT_Payload{
public:
	unsigned char id;
	std::vector<NBT_Payload*> data;
	NBT_List(const unsigned char*& data);
	~NBT_List();
};
class NBT_Compound:public NBT_Payload{
public:
	std::vector<NBT_Tag*> data;
	NBT_Tag* operator[](std::string child);
	NBT_Compound(const unsigned char*& data);
	~NBT_Compound();
};
class NBT_Int_Array:public NBT_Payload{
public:
	std::vector<int> data;
	NBT_Int_Array(const unsigned char*& data);
};
#endif