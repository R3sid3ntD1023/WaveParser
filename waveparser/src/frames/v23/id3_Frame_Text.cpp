#include "frames/v23/id3_Frame_Text.h"

namespace waveparser
{
	std::string Utf16ToUtf8(const std::u16string &utf16)
	{
		std::string utf8;
		utf8.reserve(utf16.size() * 3); // worst case

		for (char16_t ch : utf16)
		{
			if (ch <= 0x7F)
			{
				utf8.push_back(char(ch));
			}
			else if (ch <= 0x7FF)
			{
				utf8.push_back(char(0xC0) | (ch >> 6));
				utf8.push_back(char(0x80) | (ch & 0x3F));
			}
			else
			{
				utf8.push_back(char(0xE0) | (ch >> 12));
				utf8.push_back(char(0x80) | ((ch >> 6) & 0x3F));
				utf8.push_back(char(0x80) | (ch & 0x3F));
			}
		}

		return utf8;
	}

	std::string ConvertToUtf8(ETextEncoding encoding, const std::vector<char> &data)
	{

		switch (encoding)
		{
		case ETextEncoding::ISO_8859_1:
		{
			std::string out;
			out.reserve(data.size() * 2);
			for (auto &ch : data)
			{
				if (ch < 0x80)
				{
					out.push_back(ch);
				}
				else
				{
					out.push_back(0xC0 | (ch >> 6));
					out.push_back(0x80 | (ch & 0x3F));
				}
			}

			return out;
		}

		case ETextEncoding::ISO_IEC_10646_1_1993: // UTF-16 with BOM
		case ETextEncoding::UTF_16BE:
		{
			std::u16string utf16;
			utf16.resize(data.size() / 2);
			for (size_t i = 0; i < utf16.size(); i++)
			{
				uint16_t ch = uint8_t(data[i * 2] << 8) | uint8_t(data[i * 2 + 1]);
				utf16[i] = ch;
			}

			// if BOM present. swap if needed
			if (encoding == ETextEncoding::ISO_IEC_10646_1_1993 && !utf16.empty())
			{
				if (utf16[0] == 0xFEFF)
				{
					utf16.erase(utf16.begin()); // remove BOM
				}
				else if (utf16[0] == 0xFFFE)
				{
					// swap endian
					for (auto &ch : utf16)
					{
						ch = (ch >> 8) | (ch << 8);
					}
					utf16.erase(utf16.begin());
				}
			}

			return Utf16ToUtf8(utf16);
		}
		case ETextEncoding::UTF_8:
		{
			return std::string(data.begin(), data.end());
		}
		default:
			return {};
		}
	}

	void ID3TextFrame::ProcessData(std::ifstream &stream, const ID3FrameHeader &header)
	{
		auto size = header.Size;

		ETextEncoding encoding;
		stream.read(reinterpret_cast<char *>(&encoding), 1);

		size -= 1;

		std::vector<char> raw(size);
		stream.read(raw.data(), size);

		Text = ConvertToUtf8(encoding, raw);
	}

	MultiString ParseMultiString(std::ifstream &stream, ETextEncoding encoding, uint32_t size)
	{
		std::vector<char> raw(size);
		stream.read(raw.data(), size);

		size_t terminatorSize = (encoding == ISO_8859_1 || encoding == UTF_8) ? 1 : 2;

		size_t termPos = std::string::npos;
		for (size_t i = 0; i + terminatorSize <= raw.size(); ++i)
		{
			bool isTerm = true;
			for (size_t j = 0; j < terminatorSize; ++j)
			{
				if (raw[i + j] != 0)
				{
					isTerm = false;
					break;
				}
			}

			if (isTerm)
			{
				termPos = i;
				break;
			}
		}

		if (termPos == std::string::npos)
		{
			// no terminator found - treat as single string
			return {ConvertToUtf8(encoding, raw), ""};
		}

		std::vector<char> descRaw(raw.begin(), raw.begin() + termPos);
		std::vector<char> valueRaw(raw.begin() + termPos + terminatorSize, raw.end());

		return {ConvertToUtf8(encoding, descRaw), ConvertToUtf8(encoding, valueRaw)};
	}

} // namespace waveparser