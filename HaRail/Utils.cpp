#include "Utils.h"

int Utils::parseTime(const string& time) 
{
	int result = 0;
	switch (time.length())
	{
	case 8:
	{
			  if (time.substr(5, 1) != ":") {
				  throw HaException("bad time format");
			  }
			  string sec_str(time.substr(6, 2));
			  int seconds;
			  try {
				  seconds = str2int(sec_str);
			  }
			  catch (boost::bad_lexical_cast) {
				  throw HaException("bad time format");
			  }
			  result += seconds;
	}
		// FALLTHROUGH
	case 5:
	{
			  if (time.substr(2, 1) != ":") {
				  throw HaException("bad time format");
			  }
			  string hour_str(time.substr(0, 2));
			  string min_str(time.substr(3, 2));
			  int hours, minutes;
			  try {
				  hours = str2int(hour_str);
				  minutes = str2int(min_str);
			  }
			  catch (boost::bad_lexical_cast) {
				  throw HaException("bad time format");
			  }
			  result += hours * 3600 + minutes * 60;
			  break;
	}

	default:
		throw HaException("bad time format");
	}

	return result;
}

string Utils::padWithZeroes(const string& data, unsigned int target_len)
{
	string result(data);
	while (result.length() < target_len) {
		result = "0" + result;
	}
	return data;
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
		throw HaException("Could not read file");
	}
	unsigned int size = (unsigned int)ifs.tellg();
	if (size == UINT_MAX) {
		exit(0);
	}
	char *buf = new char[size + 1];
	if (!buf) {
		throw HaException("Not enough memory");
	}
	ifs.seekg(0, ios_base::beg);
	ifs.read(buf, size);
	buf[size] = 0;
	*out_buf = buf;
}

string Utils::getCurrentDate()
{
	boost::posix_time::ptime pt = boost::posix_time::second_clock::local_time();
	boost::gregorian::date d = pt.date();
	string date = string(padWithZeroes(int2str(d.day().as_number()), 2) + padWithZeroes(int2str(d.month().as_number()), 2) + padWithZeroes(int2str(d.year() % 100), 2));
	return date;
}
