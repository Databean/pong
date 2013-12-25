#include "Settings.h"

#include <fstream>
#include <string>
#include <map>

using std::fstream;
using std::string;
using std::map;
using std::pair;
using std::endl;
using std::getline;

Settings settings("settings.txt");

Settings::Settings(const std::string& filename) : filename(filename), data() {
	read();
}

Settings::Settings(const Settings& s) : filename(s.filename), data(s.data) {
	
}

Settings::~Settings() {
	write();
}

Settings& Settings::operator=(const Settings& s) {
	filename = s.filename;
	data = s.data;
	return *this;
}

void Settings::read() {
	fstream file(filename, fstream::in);
	if(file) {
		data.clear();
		while(!file.eof()) {
			string key;
			string value;
			getline(file, key, '=');
			if(key != "") {
				getline(file, value);
				data[key] = value;
			}
		}
	}
}

void Settings::write() const {
	fstream file(filename, fstream::out | fstream::trunc);
	for(const pair<string, string>& keyValue : data) {
		file << keyValue.first << "=" << keyValue.second << endl;
	}
	file.close();
}

const string& Settings::get(const string& key, const string& defaultValue) {
	//read();
	if(data.count(key)) {
		return data[key];
	} else {
		return defaultValue;
	}
}

void Settings::set(const string& key, const string& value) {
	data[key] = value;
	write();
}



