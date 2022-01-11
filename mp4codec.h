#pragma once
#include "tool.h"

void readfile(const char* filename);

struct BaseBox
{
	uint32_t size;
	std::string type;
	uint64_t largesize;//当size==1时生效
};

BaseBox* ParseBaseBox(std::ifstream* fin);

struct FtypBox {
	uint8_t* majorBrand;//标识符，如mp42
	uint32_t minorVersion;//major brand 的次版本标识
	uint8_t* compatibleBrands;//一个list，一直到box的结尾
};

FtypBox* ParseFtypBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct MvhdBox
{
	uint8_t version;//版本
	uint8_t flag[3];//扩展的movie header标志
	uint32_t createdTime;//从UTC时间的1904年1月1日0点至今的秒数,不过这里的时间并不影响播放器识别并播放影片。
	uint32_t updatedTime;//Movie atom的修订时间。基准时间是1904-1-1 0:00 AM
	//文档媒体在1秒时间内的刻度值，可以理解为1秒长度的时间单元数.
	//即将1s平均分为1/44100份，每份1/44100s。timescale时间刻度贯穿在整个文档中，所有对于时间的描述都要以其为参照，
	//例如解码时间DTS，展示时间PTS等最重要的时间描述。
	uint32_t timeScale;
	//媒体可播放时长.这个数值的单位与实际时间的对应关系就要通过上面的timescale参数。
	//duration / timescale = 可播放时长（s）。这里算出该视频能播放50247540 / 44100 = 1139s, 即19分钟左右。
	uint32_t duration;
	uint32_t speed;//推荐播放速率，高16位和低16位分别为小数点整数部分和小数部分，即[16.16] 格式，该值为1.0（0x00010000）表示正常前向播放
	uint16_t voice;//与rate类似，[8.8] 格式，1.0（0x0100）表示最大音量
	uint8_t remain[10];//保留
	uint8_t data[36]; //该矩阵定义了此movie中两个坐标空间的映射关系
	uint32_t previewTime;//开始预览此movie的时间
	uint32_t preDuration;//预览时长
	uint32_t posterTime;//宣传时间
	uint32_t selectionTime;//当前选择的开始时间
	uint32_t selectionDuration;//一刻度时间内当前选择的持续时间
	uint32_t currentTime;//当前时间
	uint32_t nextTrackId;//下一个trackID
};

MvhdBox* ParseMvhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct UdtaBox
{
	uint8_t data[30];//用户自定义数据
};

UdtaBox* ParseUdtaBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct GnreBox
{
	uint8_t data[30];
};

GnreBox* ParseGnreBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct MetaBox
{
	uint8_t* data;
};

MetaBox* ParseMetaBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct TkhdBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	/*按位或操作结果值，预定义如下：
	0x000001 track_enabled，否则该track不被播放；
	0x000002 track_in_movie，表示该track在播放中被引用；
	0x000004 track_in_preview，表示该track在预览时被引用。
	一般该值为7，如果一个媒体所有track均未设置track_in_movie和track_in_preview，
	将被理解为所有track均设置了这两项；对于hint track，该值为0*/
	uint8_t flags[3];
	uint32_t createdTime;//创建时间（相对于UTC时间1904-01-01零点的秒数）
	uint32_t modificationTime;//修改时间
	uint32_t trackId;//id号，不能重复且不能为0
	uint32_t reserved1;//保留位
	uint32_t duration;//track的时间长度
	uint64_t reserved2;//保留位
	uint16_t layer;//视频层，默认为0，值小的在上层
	uint16_t alternateGroup;//track分组信息，默认为0表示该track未与其他track有群组关系
	uint16_t volume;//[8.8] 格式，如果为音频track，1.0（0x0100）表示最大音量；否则为0
	uint16_t reserved3;//保留位
	uint8_t matrix[36];//视频变换矩阵
	uint32_t width;//宽
	uint32_t height;//高，均为 [16.16] 格式值，与sample描述中的实际画面大小比值，用于播放时的展示宽高

};

TkhdBox* ParseTkhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct ElstBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint32_t* segmentDuration;//duration of this edit segment in units of the movie’s timescale.
	/**
	starting time within the media of this edit segment (in media
	timescale units)。值为-1表示是空edit。Track中的最后一个edit永远不能为空。
	Any difference between the movie’s duration and the track’s duration is expressed as an implicit empty edit.
	**/
	uint32_t* mediaTime;
	/**
	relative rate at which to play the media corresponding to this edit segment。不能是0或负数。
	**/
	uint32_t* mediaRate;
};

ElstBox* ParseElstBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct EdtsBox
{
	ElstBox* elstBox;
};

EdtsBox* ParseEdtsBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct MdhdBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t createdTime;//创建时间（相对于UTC时间1904-01-01零点的秒数）
	uint32_t modificationTime;//修改时间
	uint32_t timeScale;//文件媒体在1秒时间内的刻度值，可以理解为1秒长度的时间单元数
	uint32_t duration;//track的时间长度
	uint16_t language;//媒体语言码。最高位为0，后面15位为3个字符（见ISO 639-2/T标准中定义）
	uint16_t preDefined;//???不知道

};

MdhdBox* ParseMdhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
“hdlr”解释了媒体的播放过程信息，该box也可以被包含在meta box（meta）中。
**/
struct HdlrBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t preDefined;//???不知道
	/**
	在media box中，该值为4个字符：
	“vide”— video track
	“soun”— audio track
	“hint”— hint track
	**/
	char handlerType[4];
	uint8_t reserved[12];
	char* name;//track type name，以‘\0’结尾的字符串

};

HdlrBox* ParseHdlrBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct VmhdBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint16_t graphicsMode;//视频合成模式，为0时拷贝原始图像，否则与opcolor进行合成
	uint16_t opcolor[3];//｛red，green，blue｝

};

VmhdBox* ParseVmhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct SmhdBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint16_t balance;//立体声平衡，[8.8] 格式值，一般为0，-1.0表示全部左声道，1.0表示全部右声道
	uint8_t reserved[2];

};

SmhdBox* ParseSmhdBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
* “url”或“urn”都是box，“url”的内容为字符串（location string），
“urn”的内容为一对字符串（name string and location string）。
当“url”或“urn”的box flag为1时，字符串均为空。
**/
struct UrlBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint8_t* data;//地址
};

UrlBox* ParseUrlBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct DrefBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//“url”或“urn”表的元素个数
	UrlBox** urls;
};

DrefBox* ParseDrefBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
*  “dinf”解释如何定位媒体信息，是一个container box。
“dinf”一般包含一个“dref”，即data reference box；
“dref”下会包含若干个“url”或“urn”，这些box组成一个表，用来定位track数据。
简单的说，track可以被分成若干段，每一段都可以根据“url”或“urn”指向的地址来获取数据，
sample描述中会用这些片段的序号将这些片段组成一个完整的track。
一般情况下，当数据被完全包含在文件中时，“url”或“urn”中的定位字符串是空的。
**/
struct DinfBox
{
	DrefBox* drefBox;
};

DinfBox* ParseDinfBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
ESDs中可以分为三层，每层为包含关系，分别为MP4ESDescr，MP4DecConfigDescr，MP4DecSpecificDescr，每层的结构都类似如下：
typedef esdsStruct{
	uint8_t tag;
	<不定长，最长4字节> size;
	uint8_t[size] data;
}esdsStruct；

各层的起始标签如下：
	MP4ESDescrTag                   0x03
	MP4DecConfigDescrTag            0x04
	MP4DecSpecificDescrTag          0x05
**/
struct EsdStruct
{
	uint8_t tag;
	uint32_t size;
	uint8_t* data;
};

EsdStruct* ParseEsdStruct(std::ifstream* fin);

struct EsdsBox
{
	uint8_t version;
	uint8_t flags[3];
	uint8_t* data;
	EsdStruct* MP4ESDescrTag;
	EsdStruct* MP4DecConfigDescrTag;
	EsdStruct* MP4DecSpecificDescrTag;
};

EsdsBox* ParseEsdsBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct Mp4aBox
{
	uint8_t reserved[6];
	uint16_t dataReferenceIndex;
	uint16_t version;
	uint16_t revisionLevel;
	uint32_t vendor;
	uint16_t numberOfChannels;
	uint16_t sampleSize;
	uint16_t compressionId;
	uint16_t reserved2;
	uint32_t sampleRate;
	EsdsBox* esdsBox;
};

Mp4aBox* ParseMp4aBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
该Box则包含了真实的SPS PPS等信息，包含着音视频编解码参数.
AVC解码器配置记录
MPEG-4规范ISO / IEC 14496-15中定义的H.264视频的AVCDecoderConfigurationRecord，并且受ISO / IEC 14496-15中规定的MPEG-4文件中存储限制的约束。
**/
struct AvcCBox
{
	uint8_t configurationVersion;//保留位默认0x01
	uint8_t avcProfileIndication;//保留位默认0x00,H.264编码配置：0x64即100表示Baseline,0x4D即77表示Main
	uint8_t profileCompatibility;
	uint8_t avcLevelIndication;//H.264编码级别
	uint8_t nalUnitSize;
	uint8_t reserved2;//3 bit,默认填1
	uint8_t numberOfSps;//5 bit,sps的个数，一般情况是1
	uint16_t spsLength;//sps 长度
	uint8_t* sps;//sps实际内容
	uint8_t reserved3;//3 bit,默认填0
	uint8_t numberOfPps;//5 bit,pps的个数，一般情况是1
	uint16_t ppsLength;//pps 长度
	uint8_t* pps;//pps实际内容
};

AvcCBox* ParseAvcCBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
此扩展名指定在视频样本中找到的像素的高宽比。
当使用非正方形像素时，这是MPEG-4和未压缩的Y´CbCr视频格式的必需扩展。
当使用正方形像素时，它是可选的。
没有指定hSpacing和vSpacing参数的度量单位，因为只有比率才重要。但是，高度和宽度的度量单位必须相同。
**/
struct PaspBox
{
	uint32_t hSpacing;//指定像素的水平间距
	uint32_t vSpacing;//指定像素（例如视频图片行）的垂直间距。
};

PaspBox* ParsePaspBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct ColrBox
{
	uint32_t colorParameterType;
	uint16_t primariesIndex;
	uint16_t transferFunctionIndex;
	uint16_t matrixIndex;
};

ColrBox* ParseColrBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct Avc1Box
{
	uint8_t reserved[6];
	uint16_t dataReferenceIndex;
	uint16_t version;//版本,一个16位整数，指示压缩数据的版本号。除非压缩程序更改了其数据格式，否则将其设置为0。
	uint16_t revisionLevel;//修订级别,必须设置为0的16位整数。
	uint32_t vendor;//一个32位整数，指定生成压缩数据的压缩程序的开发者。通常此字段包含' appl'表示Apple，Inc.
	uint32_t temporalQuality;//一个32位整数，包含从0到1023的值，指示时间压缩的程度。
	uint32_t spatialQuality;//一个32位整数，包含从0到1024的值，指示空间压缩的程度。
	uint16_t width;//一个16位整数，以像素为单位指定源图像的宽度。
	uint16_t height;//一个16位整数，以像素为单位指定源图像的高度。
	uint32_t horizontalResolution;//一个32位定点数，其中包含图像的水平分辨率，以每英寸像素为单位。
	uint32_t verticalResolution;//一个32位定点数，其中包含图像的垂直分辨率，以每英寸像素为单位。
	uint32_t dataSize;//必须设置为0的32位整数。
	uint16_t frameCount;//一个16位整数，指示每个样本中存储了多少帧压缩数据。通常设置为1。
	uint8_t compressorName[32];//一个32字节的Pascal字符串，其中包含创建映像的压缩器的名称，例如"jpeg"。
	uint16_t depth;//一个16位整数，指示压缩图像的像素深度。1、2、4、8、16、24和32的值表示彩色图像的深度。仅当图像包含Alpha通道时才应使用值32。值34、36和40分别表示灰度图像的2位，4位和8位灰度。
	/**
	一个16位整数，标识要使用的颜色表。如果此字段设置为–1，则默认色表应用于指定的深度。
	对于每个像素16位以下的所有深度，这表示指定深度的标准Macintosh色表。16、24和32的深度没有颜色表。
	如果颜色表ID设置为0，则颜色表包含在样本描述本身中。颜色表紧随样本描述中的颜色表ID字段。
	**/
	uint16_t colorTableID;
	AvcCBox* avcCBox;
	PaspBox* paspBox;
	ColrBox* colrBox;

};

Avc1Box* ParseAvc1Box(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
 box header和version字段后会有一个entry count字段，根据entry的个数，每个entry会有type信息，如“vide”、“sund”等，
 根据type不同sample description会提供不同的信息，例如对于video track，会有“VisualSampleEntry”类型信息，
 对于audio track会有“AudioSampleEntry”类型信息。视频的编码类型、宽高、长度，音频的声道、采样等信息都会出现在这个box中。
**/
struct StsdBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	Avc1Box** avc1List;
	Mp4aBox** mp4aList;
};

StsdBox* ParseStsdBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
 “stts”存储了sample的duration，描述了sample时序的映射方法，我们通过它可以找到任何时间的sample。
 “stts”可以包含一个压缩的表来映射时间和sample序号，用其他的表来提供每个sample的长度和指针。
 表中每个条目提供了在同一个时间偏移量里面连续的sample序号，以及samples的偏移量。
 递增这些偏移量，就可以建立一个完整的time to sample表。
**/
struct SttsBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint32_t* sampleCount;//有相同duration的连续sample的数目
	uint32_t* sampleDuration;//每个sample的duration,以timescale为单位的时间长度
};

SttsBox* ParseSttsBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
“stss”确定media中的关键帧。对于压缩媒体数据，关键帧是一系列压缩序列的开始帧，其解压缩时不依赖以前的帧，
而后续帧的解压缩将依赖于这个关键帧。“stss”可以非常紧凑的标记媒体内的随机存取点，它包含一个sample序号表，
表内的每一项严格按照sample的序号排列，说明了媒体中的哪一个sample是关键帧。
如果此表不存在，说明每一个sample都是一个关键帧，是一个随机存取点。
**/
struct StssBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint32_t* sampleNumber;//媒体流中同步sample的序号。
};

StssBox* ParseStssBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct CttsBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint32_t* sampleCount;
	uint32_t* sampleOffset;
};

CttsBox* ParseCttsBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
“stsz” 定义了每个sample的大小，包含了媒体中全部sample的数目和一张给出每个sample大小的表。这个box相对来说体积是比较大的。
**/
struct StszBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t sampleSize;//指定默认的sample字节大小，如果所有sample的大小不一样，这个字段为0。
	uint32_t entryCount;//条目数量
	uint32_t* entrySize;//每个sample的字节大小。
};

StszBox* ParseStszBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
用chunk组织sample可以方便优化数据获取，一个thunk包含一个或多个sample。
“stsc”中用一个表描述了sample与chunk的映射关系，查看这张表就可以找到包含指定sample的thunk，从而找到这个sample。
**/
struct StscBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint32_t* firstChunk;//一组chunk的第一个chunk的序号。chunk的编号从1开始。
	uint32_t* samplesPerChunk;//每个chunk有多少个sample。
	uint32_t* sampleDescIdx;//stsd 中sample desc信息的索引。
};

StscBox* ParseStscBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
“stco”定义了每个thunk在媒体流中的位置。位置有两种可能，32位的和64位的，后者对非常大的电影很有用。
在一个表中只会有一种可能，这个位置是在整个文件中的，而不是在任何box中的，这样做就可以直接在文件中找到媒体数据，而不用解释box。
需要注意的是一旦前面的box有了任何改变，这张表都要重新建立，因为位置信息已经改变了。
**/
struct StcoBox
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint32_t* sampleSize;//chunk在文件中的位置。
};

StcoBox* ParseStcoBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct Co64Box
{
	uint8_t version;//box版本，0或1，一般为0。（以下字节数均按version=0）
	uint8_t flags[3];
	uint32_t entryCount;//条目数量
	uint64_t* sampleSize;//chunk在文件中的位置。
};

Co64Box* ParseCo64Box(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
“stbl”几乎是普通的MP4文件中最复杂的一个box了，首先需要回忆一下sample的概念。
sample是媒体数据存储的单位，存储在media的chunk中，chunk和sample的长度均可互不相同，
“stbl”包含了关于track中sample所有时间和位置的信息，以及sample的编解码等信息。
利用这个表，可以解释sample的时序、类型、大小以及在各自存储容器中的位置。
“stbl”是一个container box，其子box包括：sample description box（stsd）、time to sample box（stts）、
sample size box（stsz或stz2）、sample to chunk box（stsc）、chunk offset box（stco或co64）、
composition time to sample box（ctts）、sync sample box（stss）等。
“stsd”必不可少，且至少包含一个条目，该box包含了data reference box进行sample数据检索的信息。
没有“stsd”就无法计算media sample的存储位置。“stsd”包含了编码的信息，其存储的信息随媒体类型不同而不同。
**/
struct StblBox
{
	StsdBox* stsdBox;
	SttsBox* sttsBox;
	StssBox* stssBox;
	CttsBox* cttsBox;
	StszBox* stszBox;
	StscBox* stscBox;
	StcoBox* stcoBox;
	Co64Box* co64Box;
};

StblBox* ParseStblBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
 “minf”存储了解释track媒体数据的handler-specific信息，media handler用这些信息将媒体时间映射到媒体数据并进行处理。
 “minf”中的信息格式和内容与媒体类型以及解释媒体数据的media handler密切相关，其他media handler不知道如何解释这些信息。
 “minf”是一个container box，其实际内容由子box说明。
一般情况下，“minf”包含一个header box，一个“dinf”和一个“stbl”，
其中，header box根据track type（即media handler type）分为“vmhd”、“smhd”、“hmhd”和“nmhd”，
“dinf”为data information box，“stbl”为sample table box。
**/
struct MinfBox
{
	VmhdBox* vmhdBox;
	SmhdBox* smhdBox;
	DinfBox* dinfBox;
	StblBox* stblBox;
};

MinfBox* ParseMinfBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
总体来说，“mdia”定义了track媒体类型以及sample数据，描述sample信息。
一般“mdia”包含一个“mdhd”，一个“hdlr”和一个“minf”，
其中“mdhd”为media header box，“hdlr”为handler reference box，
“minf”为media information box。
**/
struct MdiaBox
{
	MdhdBox* mdhdBox;
	HdlrBox* hdlrBox;
	MinfBox* minfBox;
};

MdiaBox* ParseMdiaBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct TrakBox
{
	TkhdBox* tkhdBox;
	MdiaBox* mdiaBox;
	EdtsBox* edtsBox;
};

TrakBox* ParseTrakBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct MoovBox {
	std::string majorBrand;
	uint32_t minorVersion;
	std::string compatibleBrands;
	MvhdBox* mvhdBox;
	UdtaBox* udtaBox;
	GnreBox* gnreBox;
	MetaBox* metaBox;
	uint32_t trakNum;//自定义，TrakBox的数量
	TrakBox** traks;
};

MoovBox* ParseMoovBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
 “free”中的内容是无关紧要的，可以被忽略。该box被删除后，不会对播放产生任何影响。
**/
struct FreeBox
{
	uint8_t* data;
};

FreeBox* ParseFreeBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

/**
该box包含于文件层，可以有多个，也可以没有（当媒体数据全部为外部文件引用时），用来存储媒体数据。
数据直接跟在box type字段后面，具体数据结构的意义需要参考metadata（主要在sample table中描述）。
**/
struct MdatBox
{
	uint8_t* data;
};

MdatBox* ParseMdatBox(std::ifstream* fin, uint32_t size, uint64_t largesize);

struct Mp4Box
{
	FtypBox* ftypBox;
	MoovBox* moovBox;
	FreeBox* freeBox;
	MdatBox* mdatBox;
};

Mp4Box* ParseMp4Box(std::ifstream* fin, uint32_t size, uint64_t largesize);
