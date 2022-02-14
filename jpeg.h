#pragma once
#include "tool.h"

void readJPEGfile(const char* filename);

struct BaseSegment
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型
};

BaseSegment* ParseBaseSegment(std::ifstream* fin);

//SOI文件头
struct SoiHead
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，D8
};

//APP0图像识别信息
struct App
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，E0
	uint16_t length;//段长度
	uint8_t form[5];//交换格式
	uint8_t mainVersion;//主版本号
	uint8_t minorVersion;//次版本号
	uint8_t density;//密度单位，0=无单位，1=点数/英寸，2=点数/厘米
	uint16_t xP;//X像素密度，水平方向的密度
	uint16_t yP;//Y像素密度，垂直方向的密度
	uint8_t xsp;//缩略图X像素，水平像素数目
	uint8_t ysp;//缩略图Y像素，垂直像素数目
	uint8_t* rgbP;//3*n，缩略图X像素和缩略图Y像素均大于0才有数据，n等于缩略图像素总数=缩略图X像素*缩略图Y像素
};

App* ParseApp(std::ifstream* fin);

//DQT定义量化表
struct DQT
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，DB
	uint16_t length;//段长度
	uint8_t no;//4位，QT号
	uint8_t precision;//4位，QT精度，0=一字节，其他=2字节
	uint8_t* qt;//QT量化表，64*QT精度的字节数
};

DQT* ParseDQT(std::ifstream* fin);

//SOF0组件
struct SOFComponent
{
	uint8_t id;//组件id，1=Y，2=Cb，3=Cr，4=I，5=Q
	uint8_t factor;//采样系数
	uint8_t qtNo;//量化表号
};

SOFComponent* ParseSOFComponent(std::ifstream* fin);

//SOF0图像基本信息
struct SOF
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，C0
	uint16_t length;//段长度
	uint8_t precision;//样本精度
	uint16_t heigh;//图片高度
	uint16_t width;//图片宽度
	uint8_t no;//组件数量，1=灰度图，3=YCbCr/YIQ彩色图，4=CMYK彩色图
	SOFComponent* coms;
};

SOF* ParseSOF(std::ifstream* fin);

//DHT定义Huffman表
struct DHT
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，C4
	uint16_t length;//段长度
	uint8_t no;//4位，HT号
	uint8_t htType;//1位，HT类型，0=DC表，1=AC表
	uint8_t data[16];//HT位表
	uint8_t* vt;//HT值表，字节数为表头16个数的和
};

DHT* ParseDHT(std::ifstream* fin);

//根据范式哈夫曼编码从DHT中构造查找表
uint8_t* GetDHT(uint8_t data[16], uint8_t* vt);

//SOS组件
struct SOSComponent
{
	uint8_t id;//组件id，1=Y，2=Cb，3=Cr，4=I，5=Q
	uint8_t acNo;//4位，AC表号
	uint8_t dcNo;//4位，DC表号
};

SOSComponent* ParseSOSComponent(std::ifstream* fin);

//SOS扫描行开始
struct SOS
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，DA
	uint16_t length;//段长度
	uint8_t no;//组件数量
	SOSComponent* coms;
	uint8_t remain[3];
};

SOS* ParseSOS(std::ifstream* fin);

//EOI文件尾
struct EOI
{
	uint8_t flag;//段标识，FF
	uint8_t type;//段类型，D9
};
