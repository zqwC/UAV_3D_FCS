#include "stdafx.h"
#include "../stdafx.h"
#include "FCToPCProtocolStruct.hpp"

extern BOOL g_is26ByteMessHeader;



template <typename T>
static bool TSPI::StrHexToIntDec(T &iDec, const std::string &strHex)
{
	const std::string::size_type strHexLen = strHex.size();
	iDec = 0;
	T count = 1;
	T num[16] = { 0 };

	//字符串为空或者超出了预期字符数表示转换失败，返回false
	if (0 == strHexLen || sizeof(T)* 2 < strHexLen)
	{
		return false;
	}

	std::string::size_type i;

	//检查字符的有效性，即是否在[0,9] [a,f] [A,F]之间
	for (i = 0; i < strHexLen; i++)
	{
		if (!(('0' <= strHex[i] && '9' >= strHex[i])
			|| ('a' <= strHex[i] && 'f' >= strHex[i])
			|| ('A' <= strHex[i] && 'F' >= strHex[i])))
		{
			return false;
		}
	}

	for (i = strHexLen; i > 0; i--)
	{
		if ((strHex[i - 1] >= '0') && (strHex[i - 1] <= '9'))
		{
			num[i - 1] = strHex[i - 1] - 48;  //字符0的ASCII值为48   
		}
		else if ((strHex[i - 1] >= 'a') && (strHex[i - 1] <= 'f'))
		{
			num[i - 1] = strHex[i - 1] - 'a' + 10;
		}
		else if ((strHex[i - 1] >= 'A') && (strHex[i - 1] <= 'F'))
		{
			num[i - 1] = strHex[i - 1] - 'A' + 10;
		}
		else
		{
			num[i - 1] = 0;
		}

		iDec = iDec + num[i - 1] * count;
		count = count * 16;  //十六进制(如果是八进制就在这里乘以8)       
	}

	return true;
}

static void DecNumToHexString(int num, std::string & str, int bytes, int dir = 1)
{
	str.clear();
	int index = 0;
	int i = 0;
	unsigned int n = num;
	std::string tstr;
	n %= (1 << (bytes * 8));
	while (index < 2 * bytes)
	{
		char c[2] = { '\0' };
		sprintf_s<2>(c, "%x", n % 16);
		tstr = c + tstr;
		if (index % 2 == 1)
		{
			//一个字节
			if (dir == 1)
			{
				//大端
				str = tstr + str;
			}
			else
			{
				//小端
				str = str + tstr;
			}
			tstr.clear();
		}
		n /= 16;
		index++;
	}
}


static bool UcharToStrHex(std::string &hexStr, const unsigned char uchar)
{
	char buffer[4] = { '\0' };

	sprintf_s(buffer, 4, "%02X", static_cast<boost::uint8_t>(uchar));
	hexStr = buffer;

	return true;
}

void TSPI::ConvertToHexString(std::string& str)
{
	str.clear();
	string tstr, rstr, head;
	 
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	m_MessageHeader.hour = systime.wHour;
	m_MessageHeader.minute = systime.wMinute;
	m_MessageHeader.second = systime.wSecond;
	m_MessageHeader.millSecond = systime.wMilliseconds;
	GetHeaderStr(m_MessageHeader, head);

	rstr += head;
	rstr += "7022";
	DecNumToHexString(m_ID, tstr, sizeof(m_ID), -1);
	rstr += tstr;
	DecNumToHexString(m_DID, tstr, sizeof(m_DID), -1);
	rstr += tstr;
	DecNumToHexString(m_LLOTD, tstr, sizeof(m_LLOTD), -1);
	rstr += tstr;
	DecNumToHexString(m_HLOTD, tstr, sizeof(m_HLOTD), -1);
	rstr += tstr;
	DecNumToHexString(m_LLATD, tstr, sizeof(m_LLATD), -1);
	rstr += tstr;
	DecNumToHexString(m_HLATD, tstr, sizeof(m_HLATD), -1);
	rstr += tstr;
	DecNumToHexString(m_SPH, tstr, sizeof(m_SPH), -1);
	rstr += tstr;
	DecNumToHexString(m_PRH, tstr, sizeof(m_PRH), -1);
	rstr += tstr;
	DecNumToHexString(m_RH, tstr, sizeof(m_RH), -1);
	rstr += tstr;
	DecNumToHexString(m_VS, tstr, sizeof(m_VS), -1);
	rstr += tstr;
	DecNumToHexString(m_IVS, tstr, sizeof(m_IVS), -1);
	rstr += tstr;
	DecNumToHexString(m_TH, tstr, sizeof(m_TH), -1);
	rstr += tstr;
	DecNumToHexString(m_ANGP, tstr, sizeof(m_ANGP), -1);
	rstr += tstr;
	DecNumToHexString(m_ANGR, tstr, sizeof(m_ANGR), -1);
	rstr += tstr;
	DecNumToHexString(m_ANGPS, tstr, sizeof(m_ANGPS), -1);
	rstr += tstr;
	DecNumToHexString(m_ANGRS, tstr, sizeof(m_ANGRS), -1);
	rstr += tstr;
	str = rstr;
}

void TSPI::GetHeaderStr(const MessageHeader &headerInfo, std::string &headerStr)
{
	headerStr.clear();  //清空一下

	std::string str_16;
	std::string str_20;
	std::string str_21;
	std::string str_22;
	std::string str_23;
	std::string str_24;
	std::string str_25;

	boost::uint8_t ui_20 = ((headerInfo.minute << 6) & 0xC0) +
		(((headerInfo.hour & 0X1F) << 1) & 0x3E) + (headerInfo.date & 0x01);
	boost::uint8_t ui_21 = ((headerInfo.second << 4) & 0xF0) +
		((headerInfo.minute >> 2) & 0x0F);
	boost::uint8_t ui_22 = ((headerInfo.millSecond << 2) & 0xFC) +
		((headerInfo.second >> 4) & 0x03);
	boost::uint8_t ui_23 = (headerInfo.millSecond >> 6) & 0x0F;
	boost::uint8_t ui_24 = headerInfo.PID & 0x00FF;
	boost::uint8_t ui_25 = (headerInfo.PID >> 8) & 0x00FF;

	UcharToStrHex(str_16, headerInfo.COMM_SOURCE_ADDR);
	UcharToStrHex(str_20, ui_20);
	UcharToStrHex(str_21, ui_21);
	UcharToStrHex(str_22, ui_22);
	UcharToStrHex(str_23, ui_23);
	UcharToStrHex(str_24, ui_24);
	UcharToStrHex(str_25, ui_25);

	if (g_is26ByteMessHeader)
	{
		headerStr = "64000000000000000000000000000000" + str_16 + "000000" +
			str_20 + str_21 + str_22 + str_23 + str_24 + str_25;
	}
	else  //26字节的情况
	{
		headerStr = "64000000000000000000000000000000" + str_16 + "000000" +
			str_20 + str_21 + str_22 + str_23;
	}
}



bool TSPI::HexStrToAsciiStr(std::string &asciiStr, const std::string &hexStr)
{
	BYTE byteTmp = 0;
	std::string strTmp;
	asciiStr = "";  //重置

	for (decltype(hexStr.size()) i = 0; i < hexStr.size() / 2; i++)
	{
		strTmp = hexStr[i * 2];
		strTmp += hexStr[i * 2 + 1];

		if (!StrHexToIntDec<BYTE>(byteTmp, strTmp))
			return false;

		asciiStr += byteTmp;
	}

	return true;
}