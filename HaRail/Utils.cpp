﻿/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Utils.h"

namespace HaRail {
	int Utils::parseTime(const string& time)
	{
		int result = 0;
		switch (time.length())
		{
		case 8:
		{
				  if (time.substr(5, 1) != ":") {
					  throw HaException("bad time format", HaException::CONVERSION_ERROR);
				  }
				  string sec_str(time.substr(6, 2));
				  int seconds;
				  try {
					  seconds = str2int(sec_str);
				  }
				  catch (boost::bad_lexical_cast) {
					  throw HaException("bad time format", HaException::CONVERSION_ERROR);
				  }
				  result += seconds;
		}
			// FALLTHROUGH
		case 5:
		{
				  if (time.substr(2, 1) != ":") {
					  throw HaException("bad time format", HaException::CONVERSION_ERROR);
				  }
				  string hour_str(time.substr(0, 2));
				  string min_str(time.substr(3, 2));
				  int hours, minutes;
				  try {
					  hours = str2int(hour_str);
					  minutes = str2int(min_str);
				  }
				  catch (boost::bad_lexical_cast) {
					  throw HaException("bad time format", HaException::CONVERSION_ERROR);
				  }
				  result += hours * 3600 + minutes * 60;
				  break;
		}

		default:
			throw HaException("bad time format", HaException::CONVERSION_ERROR);
		}

		return result;
	}

	string Utils::padWithZeroes(const string& data, unsigned int target_len)
	{
		string result(data);
		while (result.length() < target_len) {
			result = "0" + result;
		}
		return result;
	}

	string Utils::makeTime(int time, bool short_form)
	{
		string hours = padWithZeroes(int2str(time / 3600), 2);
		time %= 3600;
		string mins = padWithZeroes(int2str(time / 60), 2);
		if (short_form) {
			return hours + ":" + mins;
		}
		else {
			string secs = padWithZeroes(int2str(time % 60), 2);
			return hours + ":" + mins + ":" + secs;
		}
	}

	void Utils::readFile(const string& path, char **out_buf)
	{
		ifstream ifs(path, ios::in | ios::binary | ios::ate);
		if (!ifs.good()) {
			throw HaException("Could not read file", HaException::FILE_NOT_FOUND_ERROR);
		}
		unsigned int size = (unsigned int)ifs.tellg();
		if (size == UINT_MAX) {
			exit(0);
		}
		char *buf = new char[size + 1];
		if (!buf) {
			throw HaException("Not enough memory", HaException::MEMORY_ERROR);
		}
		ifs.seekg(0, ios_base::beg);
		ifs.read(buf, size);
		buf[size] = 0;
		*out_buf = buf;
	}

	void Utils::readFilePart(const string& path, char **out_buf, unsigned int start, unsigned int length)
	{
		ifstream ifs(path, ios::in | ios::binary);
		if (!ifs.good()) {
			throw HaException("Could not read file", HaException::FILE_NOT_FOUND_ERROR);
		}
		if (length == UINT_MAX) {
			exit(0);
		}
		char *buf = new char[length + 1];
		if (!buf) {
			throw HaException("Not enough memory", HaException::MEMORY_ERROR);
		}
		ifs.seekg(start, ios_base::beg);
		ifs.read(buf, length);
		buf[length] = 0;
		*out_buf = buf;
	}

	string Utils::getCurrentDate()
	{
		boost::posix_time::ptime pt = boost::posix_time::second_clock::local_time();
		boost::gregorian::date d = pt.date();
		string date = string(padWithZeroes(int2str(d.day().as_number()), 2) + padWithZeroes(int2str(d.month().as_number()), 2) + padWithZeroes(int2str(d.year() % 100), 2));
		return date;
	}

	void Utils::checkDate(const string& date)
	{
		if (date.length() != 6) {
			throw HaException("Invalid date", HaException::CONVERSION_ERROR);
		}
	}

	int Utils::getDayOfWeek(const string& date)
	{
		checkDate(date);
		boost::gregorian::date d(str2int(date.substr(4, 2)) + 2000, str2int(date.substr(2, 2)), str2int(date.substr(0, 2)));
		return d.day_of_week();
	}

	string Utils::getReverseDate(const string& date)
	{
		checkDate(date);
		return int2str(str2int(date.substr(4, 2)) + 2000) + date.substr(2, 2) + date.substr(0, 2);
	}
}
