#include "StdAfx.h"
#include "NBT.h"

using namespace std;
#ifdef SIMPLE_NBT

enum tags{
	TAG_End,
	TAG_Byte,
	TAG_Short,
	TAG_Int,
	TAG_Long,
	TAG_Float,
	TAG_Double,
	TAG_Byte_Array,
	TAG_String,
	TAG_List,
	TAG_Compound,
	TAG_Int_Array
};

const static char tagSize[12] = {
	0, //end
	1, //byte
	2, //short
	4, //int
	8, //long
	4, //float
	8, //double
	-1,
	-1,
	-1,
	-1,
	-1,
};

unsigned int ReadInt(const unsigned char*& data){
	const unsigned int result = (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3];
	data += 4;
	return result;
}

void IgnoreTag(unsigned char id, const unsigned char*& data){
	switch(id){
		case TAG_Byte_Array:
			data += ReadInt(data);
			break;
		case TAG_String:
			data += ((data[0]<<8)|data[1])+2;
			break;
		case TAG_List:{
			const unsigned char newID = (data++)[0];
			const unsigned int size = ReadInt(data);
			for (unsigned int i = 0; i < size; i++){
				IgnoreTag(newID,data);
			}
			break;
		}
		case TAG_Compound:{
			unsigned char newId;
			do{
				newId = (data++)[0];
				if (newId != TAG_End) data += ((data[0]<<8)|data[1])+2;
				IgnoreTag(newId,data);
			} while (newId != TAG_End);
			break;
		}
		case TAG_Int_Array:
			data += ReadInt(data)*4;
			break;
		default: data += tagSize[id];
	};
}

ChunkSection::ChunkSection(){};

ChunkSection::ChunkSection(ChunkSection&& other) throw() {
	blocks = std::move(other.blocks);
	y = other.y;
}

WorldInfo::WorldInfo(const unsigned char*& data){
	data += 1+2+1+2+5; //rootId, rootNameLength(2), levelId, levelNameLength(2), levelName (5)
	unsigned char id;
	unsigned short nameLength;
	do {
		id = (data++)[0];
		if (id != TAG_End){
			nameLength = (data[0]<<8)|data[1];
			data += 2;
		}
		if (id == TAG_Int){
				string name((char*)data,nameLength);
				data += nameLength;
				const int payload = ReadInt(data);
				if (name == "xPos") x = payload;
				else if (name == "zPos") z = payload;
		} else if (id == TAG_List){
				string name((char*)data, nameLength);
				data += nameLength;
				if (name == "Sections"){
					unsigned int size = ReadInt(++data);
					sections.reserve(size);
					for (unsigned int i = 0; i < size; i++){
						ChunkSection section;
						section.blocks.reserve(16*16*16);
						unsigned char newId;
						do {
							newId = (data++)[0];
							string name;
							if (newId != TAG_End){
								nameLength = (data[0]<<8)|data[1];
								data += 2;
								name = string((char*)data, nameLength);
								data += nameLength;
							}
							if (newId == TAG_Byte && name == "Y") 
								section.y = (data++)[0];
							else if (newId == TAG_Byte_Array && name == "Blocks"){
								data+=4;
								for (unsigned int i = 0; i < 16*16*16; i++){
									section.blocks.push_back(data[i]);
								}
								data += 16*16*16;
							} else IgnoreTag(newId, data);
						} while (newId != TAG_End);

						sections.push_back(std::move(section));
					}
				} else IgnoreTag(id, data);
		} else IgnoreTag(id, data+=nameLength);
	} while (id != TAG_End);
}



#else
NBT_Payload::~NBT_Payload(){}

NBT_Tag* NBT_Tag::operator[](string child){
	if (id != TAG_Compound) throw invalid_argument("Tag is not a TAG_Compound!");
	return  (*((NBT_Compound*)payload))[child];
}

NBT_Tag::NBT_Tag(const unsigned char*& data){
	id = *(data++);
	if (id != TAG_End){
		unsigned short nameLength = (data[0]<<8)|data[1];
		data += 2;
		name = string((char*)data,nameLength);
		data += nameLength;
	}
	switch(id){
		case TAG_End: payload = 0; break;
		case TAG_Byte: payload = new NBT_Byte(data); break;
		case TAG_Short: payload = new NBT_Short(data); break;
		case TAG_Int: payload = new NBT_Int(data); break;
		case TAG_Long: payload = new NBT_Long(data); break;
		case TAG_Float: payload = new NBT_Float(data); break;
		case TAG_Double: payload = new NBT_Double(data); break;
		case TAG_Byte_Array: payload = new NBT_Byte_Array(data); break;
		case TAG_String: payload = new NBT_String(data); break;
		case TAG_List: payload = new NBT_List(data); break;
		case TAG_Compound: payload = new NBT_Compound(data); break;
		case TAG_Int_Array: payload = new NBT_Int_Array(data); break;
		default:{
			payload = 0;
			throw runtime_error(string("Unknown NBT tag ")+(char)id);
		}
	}
}

NBT_Tag::~NBT_Tag(){
	if (payload) delete payload;
}

NBT_Byte::NBT_Byte(const unsigned char*& data){
	this->data = (data++)[0];
}
NBT_Short::NBT_Short(const unsigned char*& data){
	this->data = (data[0]<<8)|data[1];
	data += 2;
}
NBT_Int::NBT_Int(const unsigned char*& data){
	this->data = (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3];
	data += 4;
}
NBT_Long::NBT_Long(const unsigned char*& data){
	this->data = (data[0]<<56)|(data[1]<<48)|(data[2]<<40)|(data[3]<<32)|(data[4]<<24)|(data[5]<<16)|(data[6]<<8)|data[7];
	data += 8;
}
NBT_Float::NBT_Float(const unsigned char*& data){
	this->data = *((float*)data);
	data += 4;
}
NBT_Double::NBT_Double(const unsigned char*& data){
	this->data = *((double*)data);
	data += 8;
}
NBT_Byte_Array::NBT_Byte_Array(const unsigned char*& data){
	const unsigned int size = (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3];
	data += 4;
	this->data.reserve(size);
	for (unsigned int i = 0; i < size; i++){
		this->data.push_back(*(data++));
	}
}
NBT_String::NBT_String(const unsigned char*& data){
	const unsigned short length = (data[0]<<8)|data[1];
	data += 2;
	this->data = string((char*)data,length);
	data += length;
}
NBT_List::NBT_List(const unsigned char*& data){
	this->id = data[0];
	const unsigned int size = (data[1]<<24)|(data[2]<<16)|(data[3]<<8)|data[4];
	this->data.reserve(size);
	data += 5;
	for (unsigned int i = 0; i < size; i++){
		switch(id){
			case TAG_End: this->data.push_back(new NBT_Payload);
			case TAG_Byte: this->data.push_back(new NBT_Byte(data)); break;
			case TAG_Short: this->data.push_back(new NBT_Short(data)); break;
			case TAG_Int: this->data.push_back(new NBT_Int(data)); break;
			case TAG_Long: this->data.push_back(new NBT_Long(data)); break;
			case TAG_Float: this->data.push_back(new NBT_Float(data)); break;
			case TAG_Double: this->data.push_back(new NBT_Double(data)); break;
			case TAG_Byte_Array: this->data.push_back(new NBT_Byte_Array(data)); break;
			case TAG_String: this->data.push_back(new NBT_String(data)); break;
			case TAG_List: this->data.push_back(new NBT_List(data)); break;
			case TAG_Compound: this->data.push_back(new NBT_Compound(data)); break;
			case TAG_Int_Array: this->data.push_back(new NBT_Int_Array(data)); break;
			default:{
				throw runtime_error("Unknown NBT tag!");
			}
		}
	}
}
NBT_List::~NBT_List(){
	const auto end = data.end();
	for (auto i = data.begin(); i!= end; i++){
		delete *i;
	}
}
NBT_Tag* NBT_Compound::operator[](string child){
	const auto end = data.end();
	for (auto i = data.begin(); i != end; i++){
		if ((*i)->name == child) return *i;
	}
	throw runtime_error("Tag has no child "+child);
}
NBT_Compound::NBT_Compound(const unsigned char*& data){
	NBT_Tag* tag = new NBT_Tag(data);
	while (tag->id != TAG_End){
		this->data.push_back(tag);
		tag = new NBT_Tag(data);
	}
	delete tag;
}
NBT_Compound::~NBT_Compound(){
	const auto end = data.end();
	for (auto i = data.begin(); i != end; i++){
		delete *i;
	}
}
NBT_Int_Array::NBT_Int_Array(const unsigned char*& data){
	const unsigned int size  = (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3];
	data += 4;
	this->data.reserve(size);
	for (unsigned int i = 0; i < size; i++){
		this->data.push_back((data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3]);
		data += 4;
	}
}
#endif