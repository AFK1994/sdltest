#include "mp4codec.h"

BaseBox* ParseBaseBox(std::ifstream* fin) {
	BaseBox* baseBox = (BaseBox*)malloc(sizeof(BaseBox));

	uint32_t size = 0;
	fin->read((char*)&size, sizeof(size));
	size = bytes_swap_32(size);
	baseBox->size = size;

	char* type = (char*)malloc(4 * sizeof(char));
	fin->read((char*)type, 4 * sizeof(char));
	std::string t_name(type, 4);
	baseBox->type = t_name;

	uint64_t largesize = 0;//当size==1时生效
	if (size == 1) {
		fin->read((char*)&largesize, sizeof(largesize));
		largesize = bytes_swap_64(largesize);
	}
	baseBox->largesize = largesize;

	std::cout << baseBox->type << ".size:" << baseBox->size << std::endl;
	std::cout << baseBox->type << ".type:" << baseBox->type << std::endl;
	std::cout << baseBox->type << ".largesize:" << baseBox->largesize << std::endl;
	return baseBox;
}

FtypBox* ParseFtypBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	FtypBox* ftypBox = (FtypBox*)malloc(sizeof(FtypBox));

	std::cout << "ftypBox.majorBrand:";
	ftypBox->majorBrand = (uint8_t*)malloc(4 * sizeof(uint8_t));
	for (int i = 0; i < 4; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		ftypBox->majorBrand[i] = d;
		std::cout << (char)ftypBox->majorBrand[i] << ",";
	}
	std::cout << std::endl;
	length = length - 4;

	uint32_t minorVersion = 0;
	fin->read((char*)&minorVersion, sizeof(minorVersion));
	minorVersion = bytes_swap_32(minorVersion);
	ftypBox->minorVersion = minorVersion;
	std::cout << "ftypBox.minorVersion:" << ftypBox->minorVersion << std::endl;
	length = length - 4;

	std::cout << "ftypBox.compatibleBrands:";
	ftypBox->compatibleBrands = (uint8_t*)malloc(length * sizeof(uint8_t));;
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		ftypBox->compatibleBrands[i] = d;
		std::cout << (char)ftypBox->compatibleBrands[i] << ",";
	}
	std::cout << std::endl;

	return ftypBox;
}

MvhdBox* ParseMvhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MvhdBox* mvhdBox = (MvhdBox*)malloc(sizeof(MvhdBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	mvhdBox->version = version;
	std::cout << "mvhdBox.version:" << (int)mvhdBox->version << std::endl;
	length = length - 1;

	std::cout << "mvhdBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		mvhdBox->flag[i] = d;
		std::cout << (int)mvhdBox->flag[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t createdTime;
	fin->read((char*)&createdTime, sizeof(createdTime));
	createdTime = bytes_swap_32(createdTime);
	mvhdBox->createdTime = createdTime;
	std::cout << "mvhdBox.createdTime:" << mvhdBox->createdTime << std::endl;
	length = length - 4;

	uint32_t updatedTime;
	fin->read((char*)&updatedTime, sizeof(updatedTime));
	updatedTime = bytes_swap_32(updatedTime);
	mvhdBox->updatedTime = updatedTime;
	std::cout << "mvhdBox.updatedTime:" << mvhdBox->updatedTime << std::endl;
	length = length - 4;

	uint32_t timeScale;
	fin->read((char*)&timeScale, sizeof(timeScale));
	timeScale = bytes_swap_32(timeScale);
	mvhdBox->timeScale = timeScale;
	std::cout << "mvhdBox.timeScale:" << mvhdBox->timeScale << std::endl;
	length = length - 4;

	uint32_t duration;
	fin->read((char*)&duration, sizeof(duration));
	duration = bytes_swap_32(duration);
	mvhdBox->duration = duration;
	std::cout << "mvhdBox.duration:" << mvhdBox->duration << std::endl;
	length = length - 4;

	uint32_t speed;
	fin->read((char*)&speed, sizeof(speed));
	speed = bytes_swap_32(speed);
	mvhdBox->speed = speed;
	int n = (mvhdBox->speed & 0xFFFF0000) >> 16;
	int m = mvhdBox->speed & 0x0000FFFF;
	std::cout << "mvhdBox.speed:" << n << "." << m << std::endl;
	length = length - 4;

	uint16_t voice;
	fin->read((char*)&voice, sizeof(voice));
	voice = bytes_swap_16(voice);
	mvhdBox->voice = voice;
	n = (mvhdBox->voice & 0XFF00) >> 8;
	m = mvhdBox->voice & 0X00FF;
	std::cout << "mvhdBox.voice:" << n << "." << m << std::endl;
	length = length - 2;
	\
		std::cout << "mvhdBox.remain:";
	for (int i = 0; i < 10; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		mvhdBox->remain[i] = d;
		std::cout << (int)mvhdBox->remain[i] << ",";
	}
	std::cout << std::endl;
	length = length - 10;

	std::cout << "mvhdBox.data:";
	for (int i = 0; i < 36; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		mvhdBox->data[i] = d;
		std::cout << (int)mvhdBox->data[i] << ",";
	}
	std::cout << std::endl;
	length = length - 36;

	uint32_t previewTime;
	fin->read((char*)&previewTime, sizeof(previewTime));
	previewTime = bytes_swap_32(previewTime);
	mvhdBox->previewTime = previewTime;
	std::cout << "mvhdBox.previewTime:" << mvhdBox->previewTime << std::endl;
	length = length - 4;

	uint32_t preDuration;
	fin->read((char*)&preDuration, sizeof(preDuration));
	preDuration = bytes_swap_32(preDuration);
	mvhdBox->preDuration = preDuration;
	std::cout << "mvhdBox.preDuration:" << mvhdBox->preDuration << std::endl;
	length = length - 4;

	uint32_t posterTime;
	fin->read((char*)&posterTime, sizeof(posterTime));
	posterTime = bytes_swap_32(posterTime);
	mvhdBox->posterTime = posterTime;
	std::cout << "mvhdBox.posterTime:" << mvhdBox->posterTime << std::endl;
	length = length - 4;

	uint32_t selectionTime;
	fin->read((char*)&selectionTime, sizeof(selectionTime));
	selectionTime = bytes_swap_32(selectionTime);
	mvhdBox->selectionTime = selectionTime;
	std::cout << "mvhdBox.selectionTime:" << mvhdBox->selectionTime << std::endl;
	length = length - 4;

	uint32_t selectionDuration;
	fin->read((char*)&selectionDuration, sizeof(selectionDuration));
	selectionDuration = bytes_swap_32(selectionDuration);
	mvhdBox->selectionDuration = selectionDuration;
	std::cout << "mvhdBox.selectionDuration:" << mvhdBox->selectionDuration << std::endl;
	length = length - 4;

	uint32_t currentTime;
	fin->read((char*)&currentTime, sizeof(currentTime));
	currentTime = bytes_swap_32(currentTime);
	mvhdBox->currentTime = currentTime;
	std::cout << "mvhdBox.currentTime:" << mvhdBox->currentTime << std::endl;
	length = length - 4;

	uint32_t nextTrackId;
	fin->read((char*)&nextTrackId, sizeof(nextTrackId));
	nextTrackId = bytes_swap_32(nextTrackId);
	mvhdBox->nextTrackId = nextTrackId;
	std::cout << "mvhdBox.nextTrackId:" << mvhdBox->nextTrackId << std::endl;
	length = length - 4;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}

	return mvhdBox;
}

UdtaBox* ParseUdtaBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	UdtaBox* udtaBox = (UdtaBox*)malloc(sizeof(UdtaBox));

	std::cout << "udtaBox.data:";
	for (int i = 0; i < 30; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		udtaBox->data[i] = d;
		std::cout << (char)udtaBox->data[i] << ",";
	}
	std::cout << std::endl;
	length = length - 30;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}

	return udtaBox;
}

GnreBox* ParseGnreBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	GnreBox* gnreBox = (GnreBox*)malloc(sizeof(GnreBox));

	std::cout << "gnreBox.data:";
	for (int i = 0; i < 30; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		gnreBox->data[i] = d;
		std::cout << (int)gnreBox->data[i] << ",";
	}
	std::cout << std::endl;
	length = length - 30;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}

	return gnreBox;
}

MetaBox* ParseMetaBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MetaBox* metaBox = (MetaBox*)malloc(sizeof(MetaBox));

	metaBox->data = (uint8_t*)malloc(length * sizeof(uint8_t));
	std::cout << "metaBox.data:";
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		metaBox->data[i] = d;
		std::cout << (int)metaBox->data[i] << ",";
	}
	std::cout << std::endl;
	return metaBox;
}

TkhdBox* ParseTkhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	TkhdBox* tkhdBox = (TkhdBox*)malloc(sizeof(TkhdBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	tkhdBox->version = version;
	std::cout << "tkhdBox.version:" << (int)tkhdBox->version << std::endl;
	length = length - 1;

	std::cout << "tkhdBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		tkhdBox->flags[i] = d;
		std::cout << (int)tkhdBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t createdTime;
	fin->read((char*)&createdTime, sizeof(createdTime));
	createdTime = bytes_swap_32(createdTime);
	tkhdBox->createdTime = createdTime;
	std::cout << "tkhdBox.createdTime:" << tkhdBox->createdTime << std::endl;
	length = length - 4;

	uint32_t modificationTime;
	fin->read((char*)&modificationTime, sizeof(modificationTime));
	modificationTime = bytes_swap_32(modificationTime);
	tkhdBox->modificationTime = modificationTime;
	std::cout << "tkhdBox.modificationTime:" << tkhdBox->modificationTime << std::endl;
	length = length - 4;

	uint32_t trackId;
	fin->read((char*)&trackId, sizeof(trackId));
	trackId = bytes_swap_32(trackId);
	tkhdBox->trackId = trackId;
	std::cout << "tkhdBox.trackId:" << tkhdBox->trackId << std::endl;
	length = length - 4;

	uint32_t reserved1;
	fin->read((char*)&reserved1, sizeof(reserved1));
	reserved1 = bytes_swap_32(reserved1);
	tkhdBox->reserved1 = reserved1;
	std::cout << "tkhdBox.reserved1:" << tkhdBox->reserved1 << std::endl;
	length = length - 4;

	uint32_t duration;
	fin->read((char*)&duration, sizeof(duration));
	reserved1 = bytes_swap_32(duration);
	tkhdBox->duration = duration;
	std::cout << "tkhdBox.duration:" << tkhdBox->duration << std::endl;
	length = length - 4;

	uint64_t reserved2;
	fin->read((char*)&reserved2, sizeof(reserved2));
	reserved2 = bytes_swap_64(reserved2);
	tkhdBox->reserved2 = reserved2;
	std::cout << "tkhdBox.reserved2:" << tkhdBox->reserved2 << std::endl;
	length = length - 8;

	uint16_t layer;
	fin->read((char*)&layer, sizeof(layer));
	layer = bytes_swap_16(layer);
	tkhdBox->layer = layer;
	int n = (tkhdBox->layer & 0XFF00) >> 8;
	int m = tkhdBox->layer & 0X00FF;
	std::cout << "tkhdBox.layer:" << n << "." << m << std::endl;
	length = length - 2;

	uint16_t alternateGroup;
	fin->read((char*)&alternateGroup, sizeof(alternateGroup));
	alternateGroup = bytes_swap_16(alternateGroup);
	tkhdBox->alternateGroup = alternateGroup;
	std::cout << "tkhdBox.alternateGroup:" << tkhdBox->alternateGroup << std::endl;
	length = length - 2;

	uint16_t volume;
	fin->read((char*)&volume, sizeof(volume));
	volume = bytes_swap_16(volume);
	tkhdBox->volume = volume;
	n = (tkhdBox->volume & 0XFF00) >> 8;
	m = tkhdBox->volume & 0X00FF;
	std::cout << "tkhdBox.volume:" << n << "." << m << std::endl;
	length = length - 2;

	uint16_t reserved3;
	fin->read((char*)&reserved3, sizeof(reserved3));
	reserved3 = bytes_swap_16(reserved3);
	tkhdBox->reserved3 = reserved3;
	std::cout << "tkhdBox.reserved3:" << tkhdBox->reserved3 << std::endl;
	length = length - 2;

	std::cout << "tkhdBox.matrix:";
	for (int i = 0; i < 36; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		tkhdBox->matrix[i] = d;
		std::cout << (int)tkhdBox->matrix[i] << ",";
	}
	std::cout << std::endl;
	length = length - 36;

	uint32_t width;
	fin->read((char*)&width, sizeof(width));
	width = bytes_swap_32(width);
	tkhdBox->width = width;
	n = (tkhdBox->width & 0XFFFF0000) >> 16;
	m = tkhdBox->width & 0X0000FFFF;
	std::cout << "tkhdBox.width:" << n << "." << m << std::endl;
	length = length - 4;

	uint32_t height;
	fin->read((char*)&height, sizeof(height));
	height = bytes_swap_32(height);
	tkhdBox->height = height;
	n = (tkhdBox->height & 0XFFFF0000) >> 16;
	m = tkhdBox->height & 0X0000FFFF;
	std::cout << "tkhdBox.height:" << n << "." << m << std::endl;
	length = length - 4;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}

	return tkhdBox;
}

ElstBox* ParseElstBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	ElstBox* elstBox = (ElstBox*)malloc(sizeof(ElstBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	elstBox->version = version;
	std::cout << "elstBox.version:" << (int)elstBox->version << std::endl;
	length = length - 1;

	std::cout << "elstBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		elstBox->flags[i] = d;
		std::cout << (int)elstBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	elstBox->entryCount = entryCount;
	std::cout << "elstBox.entryCount:" << elstBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return elstBox;
	}

	elstBox->segmentDuration = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	std::cout << "elstBox.segmentDuration:";
	for (int i = 0; i < entryCount; i++) {
		uint32_t d;
		fin->read((char*)&d, sizeof(uint32_t));
		elstBox->segmentDuration[i] = bytes_swap_32(d);
		std::cout << elstBox->segmentDuration[i] << ",";
	}
	std::cout << std::endl;
	length = length - entryCount * 4;

	elstBox->mediaTime = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	std::cout << "elstBox.mediaTime:";
	for (int i = 0; i < entryCount; i++) {
		uint32_t d;
		fin->read((char*)&d, sizeof(uint32_t));
		elstBox->mediaTime[i] = bytes_swap_32(d);
		std::cout << elstBox->mediaTime[i] << ",";
	}
	std::cout << std::endl;
	length = length - entryCount * 4;

	elstBox->mediaRate = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	std::cout << "elstBox.mediaRate:";
	for (int i = 0; i < entryCount; i++) {
		uint32_t d;
		fin->read((char*)&d, sizeof(uint32_t));
		elstBox->mediaRate[i] = bytes_swap_32(d);
		uint32_t m;
		uint32_t n;
		m = elstBox->mediaRate[i] >> 16;
		n = elstBox->mediaRate[i] & 0x0000FFFF;
		std::cout << m << "." << n << ",";
	}
	std::cout << std::endl;
	length = length - entryCount * 4;

	return elstBox;
}

EdtsBox* ParseEdtsBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	EdtsBox* edtsBox = (EdtsBox*)malloc(sizeof(EdtsBox));

	BaseBox* box = ParseBaseBox(fin);
	if (size == 1) {
		length = length - box->largesize;
	}
	else {
		length = length - box->size;
	}
	if (box->type == "elst") {
		ElstBox* elstBox = ParseElstBox(fin, box->size, box->largesize);
		edtsBox->elstBox = elstBox;
	}

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}
	return edtsBox;
}

MdhdBox* ParseMdhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MdhdBox* mdhdBox = (MdhdBox*)malloc(sizeof(MdhdBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	mdhdBox->version = version;
	std::cout << "mdhdBox.version:" << (int)mdhdBox->version << std::endl;
	length = length - 1;

	std::cout << "mdhdBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		mdhdBox->flags[i] = d;
		std::cout << (int)mdhdBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t createdTime;
	fin->read((char*)&createdTime, sizeof(createdTime));
	createdTime = bytes_swap_32(createdTime);
	mdhdBox->createdTime = createdTime;
	std::cout << "mdhdBox.createdTime:" << mdhdBox->createdTime << std::endl;
	length = length - 4;

	uint32_t modificationTime;
	fin->read((char*)&modificationTime, sizeof(modificationTime));
	modificationTime = bytes_swap_32(modificationTime);
	mdhdBox->modificationTime = modificationTime;
	std::cout << "mdhdBox.modificationTime:" << mdhdBox->modificationTime << std::endl;
	length = length - 4;

	uint32_t timeScale;
	fin->read((char*)&timeScale, sizeof(timeScale));
	timeScale = bytes_swap_32(timeScale);
	mdhdBox->timeScale = timeScale;
	std::cout << "mdhdBox.timeScale:" << mdhdBox->timeScale << std::endl;
	length = length - 4;

	uint32_t duration;
	fin->read((char*)&duration, sizeof(duration));
	timeScale = bytes_swap_32(duration);
	mdhdBox->duration = duration;
	std::cout << "mdhdBox.duration:" << mdhdBox->duration << std::endl;
	length = length - 4;

	uint16_t language;
	fin->read((char*)&language, sizeof(language));
	language = bytes_swap_16(language);
	mdhdBox->language = language;
	std::cout << "mdhdBox.language:" << mdhdBox->language << std::endl;
	length = length - 2;

	uint16_t preDefined;
	fin->read((char*)&preDefined, sizeof(preDefined));
	preDefined = bytes_swap_16(preDefined);
	mdhdBox->preDefined = preDefined;
	std::cout << "mdhdBox.preDefined:" << mdhdBox->preDefined << std::endl;
	length = length - 2;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}

	return mdhdBox;
}

HdlrBox* ParseHdlrBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	HdlrBox* hdlrBox = (HdlrBox*)malloc(sizeof(HdlrBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	hdlrBox->version = version;
	std::cout << "hdlrBox.version:" << (int)hdlrBox->version << std::endl;
	length = length - 1;

	std::cout << "hdlrBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		hdlrBox->flags[i] = d;
		std::cout << (int)hdlrBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t preDefined;
	fin->read((char*)&preDefined, sizeof(preDefined));
	preDefined = bytes_swap_32(preDefined);
	hdlrBox->preDefined = preDefined;
	std::cout << "hdlrBox.preDefined:" << hdlrBox->preDefined << std::endl;
	length = length - 4;

	std::cout << "hdlrBox.handlerType:";
	for (int i = 0; i < 4; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		hdlrBox->handlerType[i] = d;
		std::cout << hdlrBox->handlerType[i] << ",";
	}
	std::cout << std::endl;
	length = length - 4;

	std::cout << "hdlrBox.reserved:";
	for (int i = 0; i < 12; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		hdlrBox->reserved[i] = d;
		std::cout << (int)hdlrBox->reserved[i] << ",";
	}
	std::cout << std::endl;
	length = length - 12;

	std::cout << "hdlrBox.name:";
	hdlrBox->name = (char*)malloc(length * sizeof(char));
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		hdlrBox->name[i] = d;
		std::cout << hdlrBox->name[i] << ",";
	}
	std::cout << std::endl;

	return hdlrBox;
}

VmhdBox* ParseVmhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	VmhdBox* vmhdBox = (VmhdBox*)malloc(sizeof(VmhdBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	vmhdBox->version = version;
	std::cout << "vmhdBox.version:" << (int)vmhdBox->version << std::endl;
	length = length - 1;

	std::cout << "vmhdBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		vmhdBox->flags[i] = d;
		std::cout << (int)vmhdBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint16_t graphicsMode;
	fin->read((char*)&graphicsMode, sizeof(graphicsMode));
	graphicsMode = bytes_swap_16(graphicsMode);
	vmhdBox->graphicsMode = graphicsMode;
	std::cout << "vmhdBox.graphicsMode:" << vmhdBox->graphicsMode << std::endl;
	length = length - 2;

	std::cout << "vmhdBox.opcolor:";
	for (int i = 0; i < 6; i++) {
		uint16_t d;
		fin->read((char*)&d, sizeof(char));
		vmhdBox->opcolor[i] = bytes_swap_16(d);
		std::cout << (int)vmhdBox->opcolor[i] << ",";
	}
	std::cout << std::endl;
	length = length - 6;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}
	return vmhdBox;
}

SmhdBox* ParseSmhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	SmhdBox* smhdBox = (SmhdBox*)malloc(sizeof(SmhdBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	smhdBox->version = version;
	std::cout << "smhdBox.version:" << (int)smhdBox->version << std::endl;
	length = length - 1;

	std::cout << "smhdBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		smhdBox->flags[i] = d;
		std::cout << (int)smhdBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint16_t balance;
	fin->read((char*)&balance, sizeof(balance));
	balance = bytes_swap_16(balance);
	smhdBox->balance = balance;
	int n = (smhdBox->balance & 0XFF00) >> 8;
	int m = smhdBox->balance & 0X00FF;
	std::cout << "smhdBox.balance:" << n << "." << m << std::endl;
	length = length - 2;

	std::cout << "smhdBox.reserved:";
	for (int i = 0; i < 2; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		smhdBox->reserved[i] = d;
		std::cout << (int)smhdBox->reserved[i] << ",";
	}
	std::cout << std::endl;
	length = length - 2;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}
	return smhdBox;
}

UrlBox* ParseUrlBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	UrlBox* urlBox = (UrlBox*)malloc(sizeof(UrlBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	urlBox->version = version;
	std::cout << "urlBox.version:" << (int)urlBox->version << std::endl;
	length = length - 1;

	std::cout << "urlBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		urlBox->flags[i] = d;
		std::cout << (int)urlBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	if (urlBox->flags[2] == 1) {
		return urlBox;
	}

	urlBox->data = (uint8_t*)malloc(length * sizeof(uint8_t));
	std::cout << "urlBox.data:";
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		urlBox->data[i] = d;
		std::cout << (char)urlBox->data[i] << ",";
	}
	std::cout << std::endl;
	return urlBox;
}

DrefBox* ParseDrefBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	DrefBox* drefBox = (DrefBox*)malloc(sizeof(DrefBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	drefBox->version = version;
	std::cout << "drefBox.version:" << (int)drefBox->version << std::endl;
	length = length - 1;

	std::cout << "drefBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		drefBox->flags[i] = d;
		std::cout << (int)drefBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	drefBox->entryCount = entryCount;
	std::cout << "drefBox.entryCount:" << drefBox->entryCount << std::endl;
	length = length - 4;

	UrlBox** urls = (UrlBox**)malloc(drefBox->entryCount * sizeof(UrlBox*));
	uint32_t i = 0;
	while (i < drefBox->entryCount)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "url ") {
			UrlBox* urlBox = ParseUrlBox(fin, box->size, box->largesize);
			urls[i] = urlBox;
		}
		i++;
	}
	drefBox->urls = urls;

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}
	return drefBox;
}

DinfBox* ParseDinfBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	DinfBox* dinfBox = (DinfBox*)malloc(sizeof(DinfBox));

	BaseBox* box = ParseBaseBox(fin);
	if (size == 1) {
		length = length - box->largesize;
	}
	else {
		length = length - box->size;
	}
	if (box->type == "dref") {
		DrefBox* drefBox = ParseDrefBox(fin, box->size, box->largesize);
		dinfBox->drefBox = drefBox;
	}
	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}
	return dinfBox;
}

EsdStruct* ParseEsdStruct(std::ifstream* fin) {
	EsdStruct* esdStruct = (EsdStruct*)malloc(sizeof(EsdStruct));

	uint8_t tag;
	fin->read((char*)&tag, sizeof(tag));
	esdStruct->tag = tag;
	std::cout << "esdStruct.tag:" << esdStruct->tag << std::endl;

	uint8_t len = 0;
	int count = 4;
	uint8_t pb;
	while (count--) {
		fin->read((char*)&pb, sizeof(pb));
		len = (len << 7) | (pb & 0x7f);
		if (!(pb & 0x80))
			break;
	}
	esdStruct->size = len;
	std::cout << "esdStruct.size:" << esdStruct->size << std::endl;

	esdStruct->data = (uint8_t*)malloc(esdStruct->size * sizeof(uint8_t));
	std::cout << "esdStruct.data:";
	for (int i = 0; i < esdStruct->size; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(uint8_t));
		esdStruct->data[i] = d;
		std::cout << esdStruct->data[i] << ",";
	}
	std::cout << std::endl;

	return esdStruct;
}

EsdsBox* ParseEsdsBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	EsdsBox* esdsBox = (EsdsBox*)malloc(sizeof(EsdsBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	esdsBox->version = version;
	std::cout << "esdsBox.version:" << (int)esdsBox->version << std::endl;
	length = length - 1;

	std::cout << "esdsBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		esdsBox->flags[i] = d;
		std::cout << (int)esdsBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	esdsBox->data = (uint8_t*)malloc(length * sizeof(uint8_t));
	std::cout << "esdsBox.data:";
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(uint8_t));
		esdsBox->data[i] = d;
		std::cout << esdsBox->data[i] << ",";
	}
	std::cout << std::endl;

	//int count = 3;
	//while (count--)
	//{
	//	EsdStruct* esdStruct = ParseEsdStruct(fin);
	//	if (esdStruct->tag == 0x03) {
	//		esdsBox->MP4ESDescrTag = esdStruct;
	//	}
	//	if (esdStruct->tag == 0x04) {
	//		esdsBox->MP4DecConfigDescrTag = esdStruct;
	//	}
	//	if (esdStruct->tag == 0x05) {
	//		esdsBox->MP4DecSpecificDescrTag = esdStruct;
	//	}
	//}

	return esdsBox;
}

Mp4aBox* ParseMp4aBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	Mp4aBox* mp4aBox = (Mp4aBox*)malloc(sizeof(Mp4aBox));

	std::cout << "mp4aBox.reserved:";
	for (int i = 0; i < 6; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(uint8_t));
		mp4aBox->reserved[i] = d;
		std::cout << mp4aBox->reserved[i] << ",";
	}
	std::cout << std::endl;
	length = length - 6;

	uint16_t dataReferenceIndex;
	fin->read((char*)&dataReferenceIndex, sizeof(dataReferenceIndex));
	mp4aBox->dataReferenceIndex = dataReferenceIndex;
	std::cout << "mp4aBox.dataReferenceIndex:" << mp4aBox->dataReferenceIndex << std::endl;
	length = length - 2;

	uint16_t version;
	fin->read((char*)&version, sizeof(version));
	mp4aBox->version = version;
	std::cout << "mp4aBox.version:" << mp4aBox->version << std::endl;
	length = length - 2;

	uint16_t revisionLevel;
	fin->read((char*)&revisionLevel, sizeof(revisionLevel));
	mp4aBox->revisionLevel = revisionLevel;
	std::cout << "mp4aBox.revisionLevel:" << mp4aBox->revisionLevel << std::endl;
	length = length - 2;

	uint32_t vendor;
	fin->read((char*)&vendor, sizeof(vendor));
	mp4aBox->vendor = vendor;
	std::cout << "mp4aBox.vendor:" << mp4aBox->vendor << std::endl;
	length = length - 4;

	uint16_t numberOfChannels;
	fin->read((char*)&numberOfChannels, sizeof(numberOfChannels));
	mp4aBox->numberOfChannels = numberOfChannels;
	std::cout << "mp4aBox.numberOfChannels:" << mp4aBox->numberOfChannels << std::endl;
	length = length - 2;

	uint16_t sampleSize;
	fin->read((char*)&sampleSize, sizeof(sampleSize));
	mp4aBox->sampleSize = sampleSize;
	std::cout << "mp4aBox.sampleSize:" << mp4aBox->sampleSize << std::endl;
	length = length - 2;

	uint16_t compressionId;
	fin->read((char*)&compressionId, sizeof(compressionId));
	mp4aBox->compressionId = compressionId;
	std::cout << "mp4aBox.compressionId:" << mp4aBox->compressionId << std::endl;
	length = length - 2;

	uint16_t reserved2;
	fin->read((char*)&reserved2, sizeof(reserved2));
	mp4aBox->reserved2 = reserved2;
	std::cout << "mp4aBox.reserved2:" << mp4aBox->reserved2 << std::endl;
	length = length - 2;

	uint32_t sampleRate;
	fin->read((char*)&sampleRate, sizeof(sampleRate));
	mp4aBox->sampleRate = sampleRate;
	std::cout << "mp4aBox.sampleRate:" << mp4aBox->sampleRate << std::endl;
	length = length - 4;

	BaseBox* box = ParseBaseBox(fin);
	if (size == 1) {
		length = length - box->largesize;
	}
	else {
		length = length - box->size;
	}
	if (box->type == "esds") {
		EsdsBox* esdsBox = ParseEsdsBox(fin, box->size, box->largesize);
		mp4aBox->esdsBox = esdsBox;
	}

	//如果有，除去多余字节
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
	}
	return mp4aBox;
}

AvcCBox* ParseAvcCBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	AvcCBox* avcCBox = (AvcCBox*)malloc(sizeof(AvcCBox));

	uint8_t configurationVersion;
	fin->read((char*)&configurationVersion, sizeof(configurationVersion));
	avcCBox->configurationVersion = configurationVersion;
	std::cout << "avcCBox.configurationVersion:" << (int)avcCBox->configurationVersion << std::endl;
	length = length - 1;

	uint8_t avcProfileIndication;
	fin->read((char*)&avcProfileIndication, sizeof(avcProfileIndication));
	avcCBox->avcProfileIndication = avcProfileIndication;
	std::cout << "avcCBox.avcProfileIndication:" << avcCBox->avcProfileIndication << std::endl;
	length = length - 1;

	uint8_t profileCompatibility;
	fin->read((char*)&profileCompatibility, sizeof(profileCompatibility));
	avcCBox->profileCompatibility = profileCompatibility;
	std::cout << "avcCBox.profileCompatibility:" << (int)avcCBox->profileCompatibility << std::endl;
	length = length - 1;

	uint8_t avcLevelIndication;
	fin->read((char*)&avcLevelIndication, sizeof(avcLevelIndication));
	avcCBox->avcLevelIndication = avcLevelIndication;
	std::cout << "avcCBox.avcLevelIndication:" << (int)avcCBox->avcLevelIndication << std::endl;
	length = length - 1;

	uint8_t nalUnitSize;
	fin->read((char*)&nalUnitSize, sizeof(nalUnitSize));
	nalUnitSize = nalUnitSize & (0xFF >> 6);
	avcCBox->nalUnitSize = nalUnitSize;
	std::cout << "avcCBox.nalUnitSize:" << (int)avcCBox->nalUnitSize << std::endl;
	length = length - 1;

	uint8_t numberOfSps;
	fin->read((char*)&numberOfSps, sizeof(numberOfSps));
	numberOfSps = numberOfSps & (0xFF >> 5);
	avcCBox->numberOfSps = numberOfSps;
	std::cout << "avcCBox.numberOfSps:" << (int)avcCBox->numberOfSps << std::endl;
	length = length - 1;

	uint16_t spsLength;
	fin->read((char*)&spsLength, sizeof(spsLength));
	spsLength = bytes_swap_16(spsLength);
	avcCBox->spsLength = spsLength;
	std::cout << "avcCBox.spsLength:" << avcCBox->spsLength << std::endl;
	length = length - 2;

	avcCBox->sps = (uint8_t*)malloc(numberOfSps * spsLength * sizeof(uint8_t));
	std::cout << "avcCBox.sps:";
	for (int i = 0; i < numberOfSps * spsLength; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(uint8_t));
		avcCBox->sps[i] = d;
		std::cout << (char)avcCBox->sps[i] << ",";
	}
	std::cout << std::endl;
	length = length - numberOfSps * spsLength;

	uint8_t numberOfPps;
	fin->read((char*)&numberOfPps, sizeof(numberOfPps));
	numberOfPps = numberOfPps & (0xFF >> 5);
	avcCBox->numberOfPps = numberOfPps;
	std::cout << "avcCBox.numberOfPps:" << (int)avcCBox->numberOfPps << std::endl;
	length = length - 1;

	uint16_t ppsLength;
	fin->read((char*)&ppsLength, sizeof(ppsLength));
	ppsLength = bytes_swap_16(ppsLength);
	avcCBox->ppsLength = ppsLength;
	std::cout << "avcCBox.ppsLength:" << avcCBox->ppsLength << std::endl;
	length = length - 2;

	avcCBox->pps = (uint8_t*)malloc(numberOfPps * ppsLength * sizeof(uint8_t));
	std::cout << "avcCBox.pps:";
	for (int i = 0; i < numberOfPps * ppsLength; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(uint8_t));
		avcCBox->pps[i] = d;
		std::cout << (char)avcCBox->pps[i] << ",";
	}
	std::cout << std::endl;
	length = length - numberOfPps * ppsLength;

	return avcCBox;
}

PaspBox* ParsePaspBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	PaspBox* paspBox = (PaspBox*)malloc(sizeof(PaspBox));

	uint32_t hSpacing;
	fin->read((char*)&hSpacing, sizeof(hSpacing));
	paspBox->hSpacing = bytes_swap_32(hSpacing);
	std::cout << "paspBox.hSpacing:" << paspBox->hSpacing << std::endl;
	length = length - 4;

	uint32_t vSpacing;
	fin->read((char*)&vSpacing, sizeof(vSpacing));
	paspBox->vSpacing = bytes_swap_32(vSpacing);
	std::cout << "paspBox.vSpacing:" << paspBox->vSpacing << std::endl;
	length = length - 4;
	return paspBox;
}

ColrBox* ParseColrBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	ColrBox* colrBox = (ColrBox*)malloc(sizeof(ColrBox));

	uint32_t colorParameterType;
	fin->read((char*)&colorParameterType, sizeof(colorParameterType));
	colrBox->colorParameterType = bytes_swap_32(colorParameterType);
	std::cout << "colrBox.colorParameterType:" << colrBox->colorParameterType << std::endl;
	length = length - 4;

	uint16_t primariesIndex;
	fin->read((char*)&primariesIndex, sizeof(primariesIndex));
	colrBox->primariesIndex = bytes_swap_16(primariesIndex);
	std::cout << "colrBox.primariesIndex:" << colrBox->primariesIndex << std::endl;
	length = length - 2;

	uint16_t transferFunctionIndex;
	fin->read((char*)&transferFunctionIndex, sizeof(transferFunctionIndex));
	colrBox->transferFunctionIndex = bytes_swap_16(transferFunctionIndex);
	std::cout << "colrBox.transferFunctionIndex:" << colrBox->transferFunctionIndex << std::endl;
	length = length - 2;

	uint16_t matrixIndex;
	fin->read((char*)&matrixIndex, sizeof(matrixIndex));
	colrBox->matrixIndex = bytes_swap_16(matrixIndex);
	std::cout << "colrBox.matrixIndex:" << colrBox->matrixIndex << std::endl;
	length = length - 2;

	return colrBox;
}

Avc1Box* ParseAvc1Box(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	Avc1Box* avc1Box = (Avc1Box*)malloc(sizeof(Avc1Box));

	std::cout << "avc1Box.reserved:";
	for (int i = 0; i < 6; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		avc1Box->reserved[i] = d;
		std::cout << (char)avc1Box->reserved[i] << ",";
	}
	std::cout << std::endl;
	length = length - 6;

	uint16_t dataReferenceIndex;
	fin->read((char*)&dataReferenceIndex, sizeof(dataReferenceIndex));
	avc1Box->dataReferenceIndex = bytes_swap_16(dataReferenceIndex);
	std::cout << "avc1Box.dataReferenceIndex:" << avc1Box->dataReferenceIndex << std::endl;
	length = length - 2;

	uint16_t version;
	fin->read((char*)&version, sizeof(version));
	avc1Box->version = bytes_swap_16(version);
	std::cout << "avc1Box.version:" << avc1Box->version << std::endl;
	length = length - 2;

	uint16_t revisionLevel;
	fin->read((char*)&revisionLevel, sizeof(revisionLevel));
	avc1Box->revisionLevel = bytes_swap_16(revisionLevel);
	std::cout << "avc1Box.revisionLevel:" << avc1Box->revisionLevel << std::endl;
	length = length - 2;

	uint32_t vendor;
	fin->read((char*)&vendor, sizeof(vendor));
	avc1Box->vendor = bytes_swap_32(vendor);
	std::cout << "avc1Box.vendor:" << avc1Box->vendor << std::endl;
	length = length - 4;

	uint32_t temporalQuality;
	fin->read((char*)&temporalQuality, sizeof(temporalQuality));
	avc1Box->temporalQuality = bytes_swap_32(temporalQuality);
	std::cout << "avc1Box.temporalQuality:" << avc1Box->temporalQuality << std::endl;
	length = length - 4;

	uint32_t spatialQuality;
	fin->read((char*)&spatialQuality, sizeof(spatialQuality));
	avc1Box->spatialQuality = bytes_swap_32(spatialQuality);
	std::cout << "avc1Box.spatialQuality:" << avc1Box->spatialQuality << std::endl;
	length = length - 4;

	uint16_t width;
	fin->read((char*)&width, sizeof(width));
	avc1Box->width = bytes_swap_16(width);
	std::cout << "avc1Box.width:" << avc1Box->width << std::endl;
	length = length - 2;

	uint16_t height;
	fin->read((char*)&height, sizeof(height));
	avc1Box->height = bytes_swap_16(height);
	std::cout << "avc1Box.height:" << avc1Box->height << std::endl;
	length = length - 2;

	uint32_t horizontalResolution;
	fin->read((char*)&horizontalResolution, sizeof(horizontalResolution));
	avc1Box->horizontalResolution = bytes_swap_32(horizontalResolution);
	std::cout << "avc1Box.horizontalResolution:" << avc1Box->horizontalResolution << std::endl;
	length = length - 4;

	uint32_t verticalResolution;
	fin->read((char*)&verticalResolution, sizeof(verticalResolution));
	avc1Box->verticalResolution = bytes_swap_32(verticalResolution);
	std::cout << "avc1Box.verticalResolution:" << avc1Box->verticalResolution << std::endl;
	length = length - 4;

	uint32_t dataSize;
	fin->read((char*)&dataSize, sizeof(dataSize));
	avc1Box->dataSize = bytes_swap_32(dataSize);
	std::cout << "avc1Box.dataSize:" << avc1Box->dataSize << std::endl;
	length = length - 4;

	uint16_t frameCount;
	fin->read((char*)&frameCount, sizeof(frameCount));
	avc1Box->frameCount = bytes_swap_16(frameCount);
	std::cout << "avc1Box.frameCount:" << avc1Box->frameCount << std::endl;
	length = length - 2;

	std::cout << "avc1Box.compressorName:";
	for (int i = 0; i < 32; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		avc1Box->compressorName[i] = d;
		std::cout << (char)avc1Box->compressorName[i] << ",";
	}
	std::cout << std::endl;
	length = length - 32;

	uint16_t depth;
	fin->read((char*)&depth, sizeof(depth));
	avc1Box->depth = bytes_swap_16(depth);
	std::cout << "avc1Box.depth:" << avc1Box->depth << std::endl;
	length = length - 2;

	uint16_t colorTableID;
	fin->read((char*)&colorTableID, sizeof(colorTableID));
	avc1Box->colorTableID = bytes_swap_16(colorTableID);
	std::cout << "avc1Box.colorTableID:" << avc1Box->colorTableID << std::endl;
	length = length - 2;

	while (length > 0)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "avcC") {
			AvcCBox* avcCBox = ParseAvcCBox(fin, box->size, box->largesize);
			avc1Box->avcCBox = avcCBox;
		}
		if (box->type == "pasp") {
			PaspBox* paspBox = ParsePaspBox(fin, box->size, box->largesize);
			avc1Box->paspBox = paspBox;
		}
		if (box->type == "colr") {
			ColrBox* colrBox = ParseColrBox(fin, box->size, box->largesize);
			avc1Box->colrBox = colrBox;
		}
	}

	return avc1Box;
}

StsdBox* ParseStsdBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	StsdBox* stsdBox = (StsdBox*)malloc(sizeof(StsdBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	stsdBox->version = version;
	std::cout << "stsdBox.version:" << (int)stsdBox->version << std::endl;
	length = length - 1;

	std::cout << "stsdBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		stsdBox->flags[i] = d;
		std::cout << (int)stsdBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	stsdBox->entryCount = entryCount;
	std::cout << "stsdBox.entryCount:" << stsdBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return stsdBox;
	}

	stsdBox->avc1List = (Avc1Box**)malloc(entryCount * sizeof(Avc1Box*));
	stsdBox->mp4aList = (Mp4aBox**)malloc(entryCount * sizeof(Mp4aBox*));
	for (int i = 0; i < entryCount; i++) {
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "avc1") {
			Avc1Box* avc1Box = ParseAvc1Box(fin, box->size, box->largesize);
			stsdBox->avc1List[i] = avc1Box;
		}
		if (box->type == "mp4a") {
			Mp4aBox* mp4aBox = ParseMp4aBox(fin, box->size, box->largesize);
			stsdBox->mp4aList[i] = mp4aBox;
		}
	}

	return stsdBox;
}

SttsBox* ParseSttsBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	SttsBox* sttsBox = (SttsBox*)malloc(sizeof(SttsBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	sttsBox->version = version;
	std::cout << "sttsBox.version:" << (int)sttsBox->version << std::endl;
	length = length - 1;

	std::cout << "sttsBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		sttsBox->flags[i] = d;
		std::cout << (int)sttsBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	sttsBox->entryCount = entryCount;
	std::cout << "sttsBox.entryCount:" << sttsBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return sttsBox;
	}

	sttsBox->sampleCount = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	sttsBox->sampleDuration = (uint32_t*)malloc(entryCount * sizeof(uint32_t));

	for (int i = 0; i < entryCount; i++) {
		uint32_t sampleCount;
		fin->read((char*)&sampleCount, sizeof(sampleCount));
		sttsBox->sampleCount[i] = bytes_swap_32(sampleCount);

		uint32_t sampleDuration;
		fin->read((char*)&sampleDuration, sizeof(sampleDuration));
		sttsBox->sampleDuration[i] = bytes_swap_32(sampleDuration);
	}

	std::cout << "sttsBox.sampleCount:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << sttsBox->sampleCount[i] << ",";
	}
	std::cout << std::endl;


	std::cout << "sttsBox.sampleDuration:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << sttsBox->sampleDuration[i] << ",";
	}
	std::cout << std::endl;
	return sttsBox;
}

StssBox* ParseStssBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	StssBox* stssBox = (StssBox*)malloc(sizeof(StssBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	stssBox->version = version;
	std::cout << "stssBox.version:" << (int)stssBox->version << std::endl;
	length = length - 1;

	std::cout << "stssBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		stssBox->flags[i] = d;
		std::cout << (int)stssBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	stssBox->entryCount = entryCount;
	std::cout << "stssBox.entryCount:" << stssBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return stssBox;
	}

	stssBox->sampleNumber = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	std::cout << "stssBox.sampleNumber:";
	for (int i = 0; i < entryCount; i++) {
		uint32_t d;
		fin->read((char*)&d, sizeof(uint32_t));
		stssBox->sampleNumber[i] = bytes_swap_32(d);
		std::cout << stssBox->sampleNumber[i] << ",";
	}
	std::cout << std::endl;

	return stssBox;
}

CttsBox* ParseCttsBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	CttsBox* cttsBox = (CttsBox*)malloc(sizeof(CttsBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	cttsBox->version = version;
	std::cout << "cttsBox.version:" << (int)cttsBox->version << std::endl;
	length = length - 1;

	std::cout << "cttsBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		cttsBox->flags[i] = d;
		std::cout << (int)cttsBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	cttsBox->entryCount = entryCount;
	std::cout << "cttsBox.entryCount:" << cttsBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return cttsBox;
	}

	cttsBox->sampleCount = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	cttsBox->sampleOffset = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	for (int i = 0; i < entryCount; i++) {
		uint32_t sampleCount;
		fin->read((char*)&sampleCount, sizeof(sampleCount));
		cttsBox->sampleCount[i] = bytes_swap_32(sampleCount);

		uint32_t sampleOffset;
		fin->read((char*)&sampleOffset, sizeof(sampleOffset));
		cttsBox->sampleOffset[i] = bytes_swap_32(sampleOffset);
	}

	std::cout << "cttsBox.sampleCount:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << cttsBox->sampleCount[i] << ",";
	}
	std::cout << std::endl;

	std::cout << "cttsBox.sampleOffset:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << cttsBox->sampleOffset[i] << ",";
	}
	std::cout << std::endl;
	return cttsBox;
}

StszBox* ParseStszBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	StszBox* stszBox = (StszBox*)malloc(sizeof(StszBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	stszBox->version = version;
	std::cout << "stszBox.version:" << (int)stszBox->version << std::endl;
	length = length - 1;

	std::cout << "stszBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		stszBox->flags[i] = d;
		std::cout << (int)stszBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t sampleSize;
	fin->read((char*)&sampleSize, sizeof(sampleSize));
	sampleSize = bytes_swap_32(sampleSize);
	stszBox->sampleSize = sampleSize;
	std::cout << "stszBox.sampleSize:" << stszBox->sampleSize << std::endl;
	length = length - 4;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	stszBox->entryCount = entryCount;
	std::cout << "stszBox.entryCount:" << stszBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return stszBox;
	}

	stszBox->entrySize = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	std::cout << "stszBox.sampleSize:";
	for (int i = 0; i < entryCount; i++) {
		uint32_t d;
		fin->read((char*)&d, sizeof(uint32_t));
		stszBox->entrySize[i] = bytes_swap_32(d);
		std::cout << stszBox->entrySize[i] << ",";
	}
	std::cout << std::endl;

	return stszBox;
}

StscBox* ParseStscBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	StscBox* stscBox = (StscBox*)malloc(sizeof(StscBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	stscBox->version = version;
	std::cout << "stscBox.version:" << (int)stscBox->version << std::endl;
	length = length - 1;

	std::cout << "stscBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		stscBox->flags[i] = d;
		std::cout << (int)stscBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	stscBox->entryCount = entryCount;
	std::cout << "stscBox.entryCount:" << stscBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return stscBox;
	}

	stscBox->firstChunk = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	stscBox->samplesPerChunk = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	stscBox->sampleDescIdx = (uint32_t*)malloc(entryCount * sizeof(uint32_t));

	for (int i = 0; i < entryCount; i++) {
		uint32_t firstChunk;
		fin->read((char*)&firstChunk, sizeof(firstChunk));
		stscBox->firstChunk[i] = bytes_swap_32(firstChunk);

		uint32_t samplesPerChunk;
		fin->read((char*)&samplesPerChunk, sizeof(samplesPerChunk));
		stscBox->samplesPerChunk[i] = bytes_swap_32(samplesPerChunk);

		uint32_t sampleDescIdx;
		fin->read((char*)&sampleDescIdx, sizeof(sampleDescIdx));
		stscBox->sampleDescIdx[i] = bytes_swap_32(sampleDescIdx);
	}

	std::cout << "stscBox.firstChunk:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << stscBox->firstChunk[i] << ",";
	}
	std::cout << std::endl;


	std::cout << "stscBox.samplesPerChunk:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << stscBox->samplesPerChunk[i] << ",";
	}
	std::cout << std::endl;


	std::cout << "stscBox.sampleDescIdx:";
	for (int i = 0; i < entryCount; i++) {
		std::cout << stscBox->sampleDescIdx[i] << ",";
	}
	std::cout << std::endl;
	return stscBox;
}

StcoBox* ParseStcoBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	StcoBox* stcoBox = (StcoBox*)malloc(sizeof(StcoBox));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	stcoBox->version = version;
	std::cout << "stcoBox.version:" << (int)stcoBox->version << std::endl;
	length = length - 1;

	std::cout << "stcoBox.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		stcoBox->flags[i] = d;
		std::cout << (int)stcoBox->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	stcoBox->entryCount = entryCount;
	std::cout << "stcoBox.entryCount:" << stcoBox->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return stcoBox;
	}

	stcoBox->sampleSize = (uint32_t*)malloc(entryCount * sizeof(uint32_t));
	std::cout << "stcoBox.sampleSize:";
	for (int i = 0; i < entryCount; i++) {
		uint32_t d;
		fin->read((char*)&d, sizeof(uint32_t));
		stcoBox->sampleSize[i] = bytes_swap_32(d);
		std::cout << stcoBox->sampleSize[i] << ",";
	}
	std::cout << std::endl;

	return stcoBox;
}

Co64Box* ParseCo64Box(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	Co64Box* co64Box = (Co64Box*)malloc(sizeof(Co64Box));

	uint8_t version;
	fin->read((char*)&version, sizeof(version));
	co64Box->version = version;
	std::cout << "co64Box.version:" << (int)co64Box->version << std::endl;
	length = length - 1;

	std::cout << "co64Box.flag:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		co64Box->flags[i] = d;
		std::cout << (int)co64Box->flags[i] << ",";
	}
	std::cout << std::endl;
	length = length - 3;

	uint32_t entryCount;
	fin->read((char*)&entryCount, sizeof(entryCount));
	entryCount = bytes_swap_32(entryCount);
	co64Box->entryCount = entryCount;
	std::cout << "co64Box.entryCount:" << co64Box->entryCount << std::endl;
	length = length - 4;

	if (entryCount == 0) {
		return co64Box;
	}

	co64Box->sampleSize = (uint64_t*)malloc(entryCount * sizeof(uint64_t));
	std::cout << "co64Box.sampleSize:";
	for (int i = 0; i < entryCount; i++) {
		uint64_t d;
		fin->read((char*)&d, sizeof(uint64_t));
		co64Box->sampleSize[i] = bytes_swap_64(d);
		std::cout << co64Box->sampleSize[i] << ",";
	}
	std::cout << std::endl;

	return co64Box;
}

StblBox* ParseStblBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	StblBox* stblBox = (StblBox*)malloc(sizeof(StblBox));

	while (length > 0)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "stsd") {
			StsdBox* stsdBox = ParseStsdBox(fin, box->size, box->largesize);
			stblBox->stsdBox = stsdBox;
		}
		if (box->type == "stts") {
			SttsBox* sttsBox = ParseSttsBox(fin, box->size, box->largesize);
			stblBox->sttsBox = sttsBox;
		}
		if (box->type == "stss") {
			StssBox* stssBox = ParseStssBox(fin, box->size, box->largesize);
			stblBox->stssBox = stssBox;
		}
		if (box->type == "ctts") {
			CttsBox* cttsBox = ParseCttsBox(fin, box->size, box->largesize);
			stblBox->cttsBox = cttsBox;
		}
		if (box->type == "stsz") {
			StszBox* stszBox = ParseStszBox(fin, box->size, box->largesize);
			stblBox->stszBox = stszBox;
		}
		if (box->type == "stsc") {
			StscBox* stscBox = ParseStscBox(fin, box->size, box->largesize);
			stblBox->stscBox = stscBox;
		}
		if (box->type == "stco") {
			StcoBox* stcoBox = ParseStcoBox(fin, box->size, box->largesize);
			stblBox->stcoBox = stcoBox;
		}
		if (box->type == "co64") {
			Co64Box* co64Box = ParseCo64Box(fin, box->size, box->largesize);
			stblBox->co64Box = co64Box;
		}
	}
	return stblBox;
}

MinfBox* ParseMinfBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MinfBox* minfBox = (MinfBox*)malloc(sizeof(MinfBox));

	while (length > 0)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "vmhd") {
			VmhdBox* vmhdBox = ParseVmhdBox(fin, box->size, box->largesize);
			minfBox->vmhdBox = vmhdBox;
		}
		if (box->type == "smhd") {
			SmhdBox* smhdBox = ParseSmhdBox(fin, box->size, box->largesize);
			minfBox->smhdBox = smhdBox;
		}
		if (box->type == "dinf") {
			DinfBox* dinfBox = ParseDinfBox(fin, box->size, box->largesize);
			minfBox->dinfBox = dinfBox;
		}
		if (box->type == "stbl") {
			StblBox* stblBox = ParseStblBox(fin, box->size, box->largesize);
			minfBox->stblBox = stblBox;
		}
	}
	return minfBox;
}

MdiaBox* ParseMdiaBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MdiaBox* mdiaBox = (MdiaBox*)malloc(sizeof(MdiaBox));

	while (length > 0)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "mdhd") {
			MdhdBox* mdhdBox = ParseMdhdBox(fin, box->size, box->largesize);
			mdiaBox->mdhdBox = mdhdBox;
		}
		if (box->type == "hdlr") {
			HdlrBox* hdlrBox = ParseHdlrBox(fin, box->size, box->largesize);
			mdiaBox->hdlrBox = hdlrBox;
		}
		if (box->type == "minf") {
			MinfBox* minfBox = ParseMinfBox(fin, box->size, box->largesize);
			mdiaBox->minfBox = minfBox;
		}
	}
	return mdiaBox;
}

TrakBox* ParseTrakBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	TrakBox* trakBox = (TrakBox*)malloc(sizeof(TrakBox));

	while (length > 0)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "tkhd") {
			TkhdBox* tkhdBox = ParseTkhdBox(fin, box->size, box->largesize);
			trakBox->tkhdBox = tkhdBox;
		}
		if (box->type == "edts") {
			EdtsBox* edtsBox = ParseEdtsBox(fin, box->size, box->largesize);
			trakBox->edtsBox = edtsBox;
		}
		if (box->type == "mdia") {
			MdiaBox* mdiaBox = ParseMdiaBox(fin, box->size, box->largesize);
			trakBox->mdiaBox = mdiaBox;
		}
	}
	return trakBox;
}

MoovBox* ParseMoovBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MoovBox* moovBox = (MoovBox*)malloc(sizeof(MoovBox));

	moovBox->traks = (TrakBox**)malloc(3 * sizeof(TrakBox*));
	while (length > 0)
	{
		BaseBox* box = ParseBaseBox(fin);
		if (size == 1) {
			length = length - box->largesize;
		}
		else {
			length = length - box->size;
		}
		if (box->type == "mvhd") {
			MvhdBox* mvhdBox = ParseMvhdBox(fin, box->size, box->largesize);
			moovBox->mvhdBox = mvhdBox;
		}
		if (box->type == "udta") {
			UdtaBox* udtaBox = ParseUdtaBox(fin, box->size, box->largesize);
			moovBox->udtaBox = udtaBox;
		}
		if (box->type == "gnre") {
			GnreBox* gnreBox = ParseGnreBox(fin, box->size, box->largesize);
			moovBox->gnreBox = gnreBox;
		}
		if (box->type == "meta") {
			MetaBox* metaBox = ParseMetaBox(fin, box->size, box->largesize);
			moovBox->metaBox = metaBox;
		}
		if (box->type == "trak") {
			moovBox->trakNum++;
			TrakBox* trakBox = ParseTrakBox(fin, box->size, box->largesize);
			if (moovBox->trakNum < 3) {
				moovBox->traks[moovBox->trakNum - 1] = trakBox;
			}
		}
	}
	return moovBox;
}

FreeBox* ParseFreeBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	FreeBox* freeBox = (FreeBox*)malloc(sizeof(FreeBox));

	freeBox->data = (uint8_t*)malloc(length * sizeof(uint8_t));
	std::cout << "freeBox.data:";
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		freeBox->data[i] = d;
		std::cout << (int)freeBox->data[i] << ",";
	}
	std::cout << std::endl;
	return freeBox;
}

MdatBox* ParseMdatBox(std::ifstream* fin, uint32_t size, uint64_t largesize) {
	uint64_t length = size;
	if (size == 1) {
		length = largesize;
		length = length - 8;
	}
	length = length - 8;

	MdatBox* mdatBox = (MdatBox*)malloc(sizeof(MdatBox));

	/*mdatBox->data = (uint8_t*)malloc(length * sizeof(uint8_t));
	std::cout << "mdatBox.data:";
	for (int i = 0; i < length; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		mdatBox->data[i] = d;
		std::cout << (int)mdatBox->data[i] << ",";
	}
	std::cout << std::endl;*/
	return mdatBox;
}

Mp4Box* ParseMp4Box(std::ifstream* fin) {
	Mp4Box* mp4Box = (Mp4Box*)malloc(sizeof(Mp4Box));

	while (!fin->eof())
	{
		BaseBox* box = ParseBaseBox(fin);
		if (box->type == "ftyp") {
			FtypBox* ftypBox = ParseFtypBox(fin, box->size, box->largesize);
			mp4Box->ftypBox = ftypBox;
		}
		if (box->type == "moov") {
			MoovBox* moovBox = ParseMoovBox(fin, box->size, box->largesize);
			mp4Box->moovBox = moovBox;
		}
		if (box->type == "free") {
			FreeBox* freeBox = ParseFreeBox(fin, box->size, box->largesize);
			mp4Box->freeBox = freeBox;
		}
		if (box->type == "mdat") {
			break;
		}
	}
	return mp4Box;
}

void readfile(const char* filename) {
	std::ifstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);//用二进制打开，不然某些符号会出问题
	if (!fin.is_open()) {
		std::cout << "fail" << std::endl;
		return;
	}
	Mp4Box* mp4Box = ParseMp4Box(&fin);
	fin.close();
}