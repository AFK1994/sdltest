#include "jpeg.h"

BaseSegment* ParseBaseSegment(std::ifstream* fin) {
	BaseSegment* baseSegment = (BaseSegment*)malloc(sizeof(BaseSegment));

	uint8_t flag = 0;
	fin->read((char*)&flag, sizeof(flag));
	baseSegment->flag = flag;

	uint8_t type = 0;
	fin->read((char*)&type, sizeof(type));
	baseSegment->type = type;
	return baseSegment;
}

App* ParseApp(std::ifstream* fin) {
	App* app = (App*)malloc(sizeof(App));
	uint16_t length = 0;
	fin->read((char*)&length, sizeof(length));
	length=bytes_swap_16(length);
	app->length = length;
	std::cout <<  "App.length:" << (int)app->length << std::endl;

	std::cout << "App.form:";
	for (int i = 0; i < 5; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		app->form[i] = d;
		std::cout << app->form[i] << ",";
	}
	std::cout << std::endl;

	uint8_t mainVersion = 0;
	fin->read((char*)&mainVersion, sizeof(mainVersion));
	app->mainVersion = mainVersion;
	std::cout <<  "App.mainVersion:" << (int) app->mainVersion << std::endl;

	uint8_t minorVersion = 0;
	fin->read((char*)&minorVersion, sizeof(minorVersion));
	app->minorVersion = minorVersion;
	std::cout <<  "App.minorVersion:" << (int)app->minorVersion << std::endl;

	uint8_t density = 0;
	fin->read((char*)&density, sizeof(density));
	app->density = density;
	std::cout <<  "App.density:" << (int)app->density << std::endl;

	uint16_t xP = 0;
	fin->read((char*)&xP, sizeof(xP));
	xP = bytes_swap_16(xP);
	app->xP = xP;
	std::cout <<  "App.xP:" << (int)app->xP << std::endl;

	uint16_t yP = 0;
	fin->read((char*)&yP, sizeof(yP));
	yP = bytes_swap_16(yP);
	app->yP = yP;
	std::cout <<  "App.yP:" << (int)app->yP << std::endl;

	uint8_t xsp = 0;
	fin->read((char*)&xsp, sizeof(xsp));
	app->xsp = xsp;
	std::cout <<  "App.xsp:" << (int)app->xsp << std::endl;

	uint8_t ysp = 0;
	fin->read((char*)&ysp, sizeof(ysp));
	app->ysp = ysp;
	std::cout << "App.ysp:" << (int)app->ysp << std::endl;

	if (xsp > 0 && ysp > 0) {
		std::cout << "App0.rgbP:";
		app->rgbP = (uint8_t*)malloc(xsp * ysp * sizeof(uint8_t));
		for (int i = 0; i < 3 * xsp * ysp; i++) {
			uint8_t d;
			fin->read((char*)&d, sizeof(char));
			app->rgbP[i] = d;
			std::cout << (int)app->rgbP[i] << ",";
		}
		std::cout << std::endl;
	}
	return app;
}

DQT* ParseDQT(std::ifstream* fin) {
	DQT* dQT = (DQT*)malloc(sizeof(DQT));

	uint16_t length = 0;
	fin->read((char*)&length, sizeof(length));
	length = bytes_swap_16(length);
	dQT->length = length;
	std::cout <<  "DQT.length:" << (int)dQT->length << std::endl;

	uint8_t no = 0;
	fin->read((char*)&no, sizeof(no));
	dQT->no = (no & 0x0F) ;
	std::cout <<  "DQT.no:" << (int)dQT->no << std::endl;

	dQT->precision = (no & 0xF0) >> 4;
	std::cout <<  "DQT.precision:" << (int)dQT->precision << std::endl;

	int size = 0;
	if (dQT->precision==0) {
		size = 1;
	}
	else {
		size = 2;
	}
	std::cout << "DQT.qt:";
	dQT->qt = (uint8_t*)malloc(64 * size * sizeof(uint8_t));
	for (int i = 0; i < 64 * size; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		dQT->qt[i] = d;
		std::cout << (int)dQT->qt[i] << ",";
	}
	std::cout << std::endl;
	return dQT;
}

SOFComponent* ParseSOFComponent(std::ifstream* fin) {
	SOFComponent* sOFComponent = (SOFComponent*)malloc(sizeof(SOFComponent));

	uint8_t id = 0;
	fin->read((char*)&id, sizeof(id));
	sOFComponent->id = id;
	std::cout <<  "SOFComponent.id:" << (int)sOFComponent->id << std::endl;

	uint8_t factor = 0;
	fin->read((char*)&factor, sizeof(factor));
	sOFComponent->factor = factor;
	std::cout <<  "SOFComponent.factor:" << (int)sOFComponent->factor << std::endl;

	uint8_t qtNo = 0;
	fin->read((char*)&qtNo, sizeof(qtNo));
	sOFComponent->qtNo = qtNo;
	std::cout << "SOFComponent.qtNo:" << (int)sOFComponent->qtNo << std::endl;
	return sOFComponent;
}

SOF* ParseSOF(std::ifstream* fin) {
	SOF* sOF = (SOF*)malloc(sizeof(SOF));

	uint16_t length = 0;
	fin->read((char*)&length, sizeof(length));
	length = bytes_swap_16(length);
	sOF->length = length;
	std::cout << "SOF.length:" << (int)sOF->length << std::endl;

	uint8_t precision = 0;
	fin->read((char*)&precision, sizeof(precision));
	sOF->precision = precision;
	std::cout <<  "SOF.precision:" << (int)sOF->precision << std::endl;

	uint16_t heigh = 0;
	fin->read((char*)&heigh, sizeof(heigh));
	heigh = bytes_swap_16(heigh);
	sOF->heigh = heigh;
	std::cout <<  "SOF.heigh:" << (int)sOF->heigh << std::endl;

	uint16_t width = 0;
	fin->read((char*)&width, sizeof(width));
	width = bytes_swap_16(width);
	sOF->width = width;
	std::cout <<  "SOF.width:" << (int)sOF->width << std::endl;

	uint8_t no = 0;
	fin->read((char*)&no, sizeof(no));
	sOF->no = no;
	std::cout <<  "SOF.no:" << (int)sOF->no << std::endl;

	sOF->coms = (SOFComponent*)malloc(sOF->no * sizeof(SOFComponent));
	for (int i = 0; i < sOF->no; i++) {
		SOFComponent* com = ParseSOFComponent(fin);
		sOF->coms[i] = *com;
	}
	return sOF;
}

DHT* ParseDHT(std::ifstream* fin) {
	DHT* dHT = (DHT*)malloc(sizeof(DHT));

	uint16_t length = 0;
	fin->read((char*)&length, sizeof(length));
	length = bytes_swap_16(length);
	dHT->length = length;
	std::cout <<  "DHT.length:" << (int)dHT->length << std::endl;

	uint8_t no = 0;
	fin->read((char*)&no, sizeof(no));
	dHT->no = (no & 0x0F);
	std::cout <<  "DHT.no:" << (int)dHT->no << std::endl;

	dHT->htType = (no & 0xF0) >> 4;
	std::cout <<  "DHT.htType:" << (int)dHT->htType << std::endl;

	std::cout <<  "DHT.data:";
	uint8_t size = 0;
	for (int i = 0; i < 16; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		dHT->data[i] = d;
		size = size + d;
		std::cout << (int)dHT->data[i] << ",";
	}
	std::cout << std::endl;

	std::cout << "DHT.vt:";
	dHT->vt = (uint8_t*)malloc(size * sizeof(uint8_t));
	for (int i = 0; i < size; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		dHT->vt[i] = d;
		std::cout << (int)dHT->vt[i] << ",";
	}
	std::cout << std::endl;
	return dHT;
}

SOSComponent* ParseSOSComponent(std::ifstream* fin) {
	SOSComponent* sOSComponent = (SOSComponent*)malloc(sizeof(SOSComponent));

	uint8_t id = 0;
	fin->read((char*)&id, sizeof(id));
	sOSComponent->id = id;
	std::cout <<  "SOSComponent.id:" << (int)sOSComponent->id << std::endl;

	uint8_t acNo = 0;
	fin->read((char*)&acNo, sizeof(acNo));
	sOSComponent->acNo = (acNo & 0x0F);
	std::cout <<  "SOSComponent.acNo:" << (int)sOSComponent->acNo << std::endl;

	sOSComponent->dcNo = (acNo & 0xF0) >> 4;
	std::cout <<  "SOSComponent.dcNo:" << (int)sOSComponent->dcNo << std::endl;
	return sOSComponent;
}


SOS* ParseSOS(std::ifstream* fin) {
	SOS* sOS = (SOS*)malloc(sizeof(SOS));

	uint16_t length = 0;
	fin->read((char*)&length, sizeof(length));
	length = bytes_swap_16(length);
	sOS->length = length;
	std::cout <<  "SOS.length:" << (int)sOS->length << std::endl;

	uint8_t no = 0;
	fin->read((char*)&no, sizeof(no));
	sOS->no = no;
	std::cout <<  "SOS.no:" << (int)sOS->no << std::endl;

	sOS->coms = (SOSComponent*)malloc(sOS->no * sizeof(SOSComponent));
	for (int i = 0; i < sOS->no; i++) {
		SOSComponent* com = ParseSOSComponent(fin);
		sOS->coms[i] = *com;
	}

	std::cout << "SOS.remain:";
	for (int i = 0; i < 3; i++) {
		uint8_t d;
		fin->read((char*)&d, sizeof(char));
		sOS->remain[i] = d;
		std::cout << (int)sOS->remain[i] << ",";
	}
	std::cout << std::endl;
	return sOS;
}

void readJPEGfile(const char* filename) {
	std::ifstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);//用二进制打开，不然某些符号会出问题
	if (!fin.is_open()) {
		std::cout << "fail" << std::endl;
		return;
	}

	BaseSegment* baseSegment;
	bool f = true;
	while (f)
	{
		baseSegment = ParseBaseSegment(&fin);

		switch (baseSegment->type)
		{
		case 0xD8:
			break;
		case 0xE0:
			ParseApp(&fin);
			break;
		case 0xDB:
			ParseDQT(&fin);
			break;
		case 0xC0:
			ParseSOF(&fin);
			break;
		case 0xC4:
			ParseDHT(&fin);
			break;
		case 0xDA:
			ParseSOS(&fin);
			break;
		case 0xD9:
			f = false;
			std::cout << "over" << std::endl;
			break;
		default:
			uint16_t length = 0;
			fin.read((char*)&length, sizeof(length));
			length = bytes_swap_16(length);
			//如果有，除去多余字节
			for (int i = 0; i < length-2; i++) {
				uint8_t d;
				fin.read((char*)&d, sizeof(char));
			}
			break;
		}
	}

	fin.close();
}