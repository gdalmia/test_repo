// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/datetime.h"
#include <cctype>

#include <Windows.h>
#undef min
#undef max


namespace Philips { namespace Workflow {

	constexpr int fileTimeClock = 10000000;
	constexpr long long fileTimeToEpoch = 11644473600LL;

	static bool parseValue(const std::string& format, const std::string& value, char key, int& result)
	{
		std::string stringResult = "0";
		auto pos = format.find(key);

		while (pos != std::string::npos)
		{
			if (isdigit(value[pos]))
				stringResult += value[pos];
			else
				return false;

			if (pos == format.size() - 1)
				break;

			pos = format.find(key, pos + 1);
		}

		result = stoi(stringResult);

		return true;
	}

	static unsigned char toLower(unsigned char value)
	{
		if (std::isupper(value))
			return static_cast<unsigned char>(std::tolower(value));
		return value;
	}

	static bool parseAmPm(const std::string& format, const std::string& value, char key, int& hour)
	{
		const auto pos = format.find_first_of(key);

		if (pos == std::string::npos)
			return false;
		
		const int result = toLower(value[pos]);;

		switch (result)
		{
		case 'a':
			return true;
		case 'p':
			hour += 12;
			return true;
		default:
			return false;
		}
	}

	static bool parseHours(const std::string& format, const std::string& value, int& hour)
	{
		if (format.find('H') != std::string::npos)
			return parseValue(format, value, 'H', hour);

		return parseValue(format, value, 'h', hour) && parseAmPm(format, value, 't', hour);
	}

	static std::chrono::time_point<std::chrono::system_clock> fromSystemTime(const SYSTEMTIME& systemTime, bool local)
	{
		FILETIME fileTime;
		if (SystemTimeToFileTime(&systemTime, &fileTime))
		{
			if (local)
			{
				FILETIME utcTime;
				if (LocalFileTimeToFileTime(&fileTime, &utcTime))
					fileTime = utcTime;
			}

			LARGE_INTEGER li;
			li.LowPart = fileTime.dwLowDateTime;
			li.HighPart = fileTime.dwHighDateTime;
			return std::chrono::system_clock::from_time_t(li.QuadPart / fileTimeClock - fileTimeToEpoch);
		}

		return std::chrono::system_clock::time_point::min();
	}

	static DateTime fromDateTime(int year, int month, int day, int hour, int minute, int second, bool local)
	{
		SYSTEMTIME systemTime;
		memset(&systemTime, 0, sizeof(systemTime));
		systemTime.wYear = WORD(year);
		systemTime.wMonth = WORD(month);
		systemTime.wDay = WORD(day);
		systemTime.wHour = WORD(hour);
		systemTime.wMinute = WORD(minute);
		systemTime.wSecond = WORD(second);

		return DateTime(fromSystemTime(systemTime, local));
	}

	bool toSystemTime(const std::chrono::system_clock::time_point& timePoint, SYSTEMTIME& systemTime, bool local)
	{
		LARGE_INTEGER uli;
		uli.QuadPart = (std::chrono::system_clock::to_time_t(timePoint) + fileTimeToEpoch) * fileTimeClock;

		FILETIME fileTime;
		fileTime.dwLowDateTime = uli.LowPart;
		fileTime.dwHighDateTime = uli.HighPart;

		if (local)
		{
			FILETIME localTime;
			if (FileTimeToLocalFileTime(&fileTime, &localTime))
				fileTime = localTime;
		}

		return FileTimeToSystemTime(&fileTime, &systemTime) != FALSE;
	}

	void formatValue(const std::string& format, int data, char key, std::string& result)
	{
		auto dataString = std::to_string(data);
		auto dataIterator = dataString.size();
		auto pos = format.rfind(key);

		while (pos != std::string::npos)
		{
			if (dataIterator > 0)
			{
				result[pos] = dataString[dataIterator - 1];
				dataIterator--;
			}
			else
			{
				result[pos] = '0';
			}
			if (pos == 0)
				break;
			pos = format.rfind(key, pos - 1);
		}
	}

	void formatAmPm(const std::string& format, int& hour, std::string& result)
	{
		auto pos = format.find('t');
		if (pos != std::string::npos)
		{
			if (hour > 12)
			{
				hour = hour - 12;
				result[pos] = 'P';
			}
			else
			{
				result[pos] = 'A';
			}
		}
		pos = format.find('t', pos + 1);
		if (pos != std::string::npos)
		{
			result[pos] = 'M';
		}
	}

	void formatHours(const std::string& format, int hour, std::string& result)
	{
		if (format.find('H') != std::string::npos)
		{
			formatValue(format, hour, 'H', result);
		}
		else
		{
			formatAmPm(format, hour, result);
			formatValue(format, hour, 'h', result);
		}
	}

	std::string systemTimeToString(const SYSTEMTIME& systemTime, const std::string& format)
	{
		std::string result = format;
		formatValue(format, systemTime.wYear, 'y', result);
		formatValue(format, systemTime.wMonth, 'M', result);
		formatValue(format, systemTime.wDay, 'd', result);
		formatHours(format, systemTime.wHour, result);
		formatValue(format, systemTime.wMinute, 'm', result);
		formatValue(format, systemTime.wSecond, 's', result);
		
		return result;
	}

	Date systemTimeToDate(const SYSTEMTIME& systemTime)
	{
		return Date{ systemTime.wDay, systemTime.wMonth, systemTime.wYear };		
	}

	DateTime::DateTime() : time_point(min())
	{		
	}

	DateTime::DateTime(const DateTime&& from) noexcept : time_point(from)
	{
	}

	DateTime::DateTime(const std::chrono::system_clock::time_point& from) : time_point(from)
	{
	}

	bool DateTime::operator==(const DateTime& comp) const
	{
		return this->time_since_epoch().count() == comp.time_since_epoch().count();
	}

	bool DateTime::operator!=(const DateTime& comp) const
	{
		return this->time_since_epoch().count() != comp.time_since_epoch().count();
	}

	bool DateTime::operator<(const DateTime& comp) const
	{
		return this->time_since_epoch().count() < comp.time_since_epoch().count();
	}

	bool DateTime::operator<=(const DateTime& comp) const
	{
		return this->time_since_epoch().count() <= comp.time_since_epoch().count();
	}

	bool DateTime::operator>(const DateTime& comp) const
	{
		return this->time_since_epoch().count() > comp.time_since_epoch().count();
	}

	bool DateTime::operator>=(const DateTime& comp) const
	{
		return this->time_since_epoch().count() >= comp.time_since_epoch().count();
	}
		
	Date DateTime::toDate(bool local) const
	{
		SYSTEMTIME systemTime;
		if (toSystemTime(*this, systemTime, local))
		{
			return systemTimeToDate(systemTime);
		}
		return {};
	}

	std::string DateTime::toString(const std::string& format, bool local) const
	{
		SYSTEMTIME systemTime;
		if (toSystemTime(*this, systemTime, local))
		{
			return systemTimeToString(systemTime, format);
		}
		return std::string();
	}

	DateTime DateTime::now()
	{
		return DateTime(std::chrono::system_clock::now());
	}

	DateTime DateTime::fromString(const std::string& format, const std::string& value, bool local)
	{
		if(format.size() != value.size())
			return DateTime(std::chrono::system_clock::time_point::min());
		
		int year, month, day;
		if (parseValue(format, value, 'y', year) &&
			parseValue(format, value, 'M', month) &&
			parseValue(format, value, 'd', day))
		{
			int hour = 0, minute = 0, second = 0;
			if (parseHours(format, value, hour) &&
				parseValue(format, value, 'm', minute) &&
				parseValue(format, value, 's', second))
			{
				return fromDateTime(year, month, day, hour, minute, second, local);
			}

			return fromDateTime(year, month, day, 0, 0, 0, local);
		}

		return DateTime(std::chrono::system_clock::time_point::min());
	}
	
}}
