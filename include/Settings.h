#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>

class Settings {
private:
	std::string filename;
	std::map<std::string, std::string> data;
	
protected:
	virtual void read();
	virtual void write() const;
public:
	Settings(const std::string& filename);
	Settings(const Settings&);
	virtual ~Settings();
	virtual Settings& operator=(const Settings&);
	
	virtual const std::string& get(const std::string& key, const std::string& defaultValue="");
	virtual void set(const std::string& key, const std::string& value);
};

extern Settings settings;

#endif