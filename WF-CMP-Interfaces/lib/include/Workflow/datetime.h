// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <chrono>
#include <string>

namespace Philips { namespace Workflow {

	//This struct represents the Date in terms of Day, Month and Year 
	struct Date
	{
		int day;
		int month;
		int year;
	};

	//! This class represents a Time in seconds since 00:00:00 Jan 1 1970 UTC. 
	class DateTime : public std::chrono::system_clock::time_point //TICS !OOP#013: DateTime wrapper over system_clock::time_point
	{
	public:

		//! Creates a default Datetime object with minimum time point value
		DateTime();

		//! Creates a Datetime object with the given time point value
		explicit DateTime(const std::chrono::system_clock::time_point &from);

		//! Creates a Database object with the given Datetime value
		DateTime(const DateTime& from) = default;

		//! Creates a Database object with the given Datetime value
		DateTime(const DateTime&& from) noexcept;

		//! Default destructor for Datetime object
		~DateTime() = default;

		DateTime& operator=	(const DateTime& from) = default;
		DateTime& operator=	(DateTime&& from) noexcept = default;

		bool operator==	(const DateTime &) const;
		bool operator!=	(const DateTime &) const;
		bool operator<	(const DateTime &) const;
		bool operator<=	(const DateTime &) const;
		bool operator>	(const DateTime &) const;
		bool operator>=	(const DateTime &) const;

		//! Converts the stored DateTime to Date structure and returns it.
		//! \param local Local is made false if UTC date is required. Else, by default, local date is returned.
		//! \return Date Structure with values of  day, month and year.
		Date toDate(bool local = true) const;

		//! Converts the stored DateTime to string of required format and returns it.
		//! \param format Specifies the format for the datetime to be returned. The accepted characters are:
		//!					y: Year, M: month, d: day, H: hour(24 hour format), h: hour(12 hour format), m: minutes, s: seconds, t: AM or PM
		//! \param local Local is made false if UTC date is required. Else, by default, local date is returned.
		//! \return wstring String with complete datetime format filled with date and time values.
		//! \code
		//! auto dateTimeString = dateTime.toString(L"yyyy/MM/dd hh:mm:ss tt", false);
		//! \endcode
		std::string toString(const std::string &format, bool local = true) const;

		//! Converts the given DateTime string to DateTime format and returns it.
		//! \param format Specifies the format for the datetime to be returned. The accepted characters are:
		//!					y: Year, M: month, d: day, H: hour(24 hour format), h: hour(12 hour format), m: minutes, s: seconds, t: AM or PM
		//! \param value Specifies the value of each section corresponding to the format
		//! \param local Local is made false if UTC date is required. Else, by default, local date is returned.
		//! \return DateTime DateTime object with the datetime value parsed from string.
		//! \code
		//! auto dateTime = DateTime::fromString(L"yyyy/MM/dd hh:mm:ss tt", L"2020/02/02 02:02:02 PM", false);
		//! \endcode
		static DateTime fromString(const std::string &format, const std::string &value, bool local = true);

		//! Creates a DateTime object with the current time point.
		//! \return DateTime Datetime object containing the current time point.
		static DateTime	now();
	};
	
}}
