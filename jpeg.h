#pragma once
#include "tool.h"

void readJPEGfile(const char* filename);

struct BaseSegment
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//������
};

BaseSegment* ParseBaseSegment(std::ifstream* fin);

//SOI�ļ�ͷ
struct SoiHead
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�D8
};

//APP0ͼ��ʶ����Ϣ
struct App
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�E0
	uint16_t length;//�γ���
	uint8_t form[5];//������ʽ
	uint8_t mainVersion;//���汾��
	uint8_t minorVersion;//�ΰ汾��
	uint8_t density;//�ܶȵ�λ��0=�޵�λ��1=����/Ӣ�磬2=����/����
	uint16_t xP;//X�����ܶȣ�ˮƽ������ܶ�
	uint16_t yP;//Y�����ܶȣ���ֱ������ܶ�
	uint8_t xsp;//����ͼX���أ�ˮƽ������Ŀ
	uint8_t ysp;//����ͼY���أ���ֱ������Ŀ
	uint8_t* rgbP;//3*n������ͼX���غ�����ͼY���ؾ�����0�������ݣ�n��������ͼ��������=����ͼX����*����ͼY����
};

App* ParseApp(std::ifstream* fin);

//DQT����������
struct DQT
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�DB
	uint16_t length;//�γ���
	uint8_t no;//4λ��QT��
	uint8_t precision;//4λ��QT���ȣ�0=һ�ֽڣ�����=2�ֽ�
	uint8_t* qt;//QT������64*QT���ȵ��ֽ���
};

DQT* ParseDQT(std::ifstream* fin);

//SOF0���
struct SOFComponent
{
	uint8_t id;//���id��1=Y��2=Cb��3=Cr��4=I��5=Q
	uint8_t factor;//����ϵ��
	uint8_t qtNo;//�������
};

SOFComponent* ParseSOFComponent(std::ifstream* fin);

//SOF0ͼ�������Ϣ
struct SOF
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�C0
	uint16_t length;//�γ���
	uint8_t precision;//��������
	uint16_t heigh;//ͼƬ�߶�
	uint16_t width;//ͼƬ���
	uint8_t no;//���������1=�Ҷ�ͼ��3=YCbCr/YIQ��ɫͼ��4=CMYK��ɫͼ
	SOFComponent* coms;
};

SOF* ParseSOF(std::ifstream* fin);

//DHT����Huffman��
struct DHT
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�C4
	uint16_t length;//�γ���
	uint8_t no;//4λ��HT��
	uint8_t htType;//1λ��HT���ͣ�0=DC��1=AC��
	uint8_t data[16];//HTλ��
	uint8_t* vt;//HTֵ���ֽ���Ϊ��ͷ16�����ĺ�
};

DHT* ParseDHT(std::ifstream* fin);

//���ݷ�ʽ�����������DHT�й�����ұ�
uint8_t* GetDHT(uint8_t data[16], uint8_t* vt);

//SOS���
struct SOSComponent
{
	uint8_t id;//���id��1=Y��2=Cb��3=Cr��4=I��5=Q
	uint8_t acNo;//4λ��AC���
	uint8_t dcNo;//4λ��DC���
};

SOSComponent* ParseSOSComponent(std::ifstream* fin);

//SOSɨ���п�ʼ
struct SOS
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�DA
	uint16_t length;//�γ���
	uint8_t no;//�������
	SOSComponent* coms;
	uint8_t remain[3];
};

SOS* ParseSOS(std::ifstream* fin);

//EOI�ļ�β
struct EOI
{
	uint8_t flag;//�α�ʶ��FF
	uint8_t type;//�����ͣ�D9
};
