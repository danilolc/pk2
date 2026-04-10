//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * JSON utils by SaturninTheAlien.
 */
#include "PJson.hpp"
#include <sstream>

namespace PJson{

void jsonReadString(const nlohmann::json& j, const std::string& name, std::string& target){
	if(j.contains(name)){
		target = j[name].get<std::string>();
	}
}

void jsonReadInt(const nlohmann::json& j, const std::string& name, int& target){
	if(j.contains(name)){
		target = j[name].get<int>();
	}
}

void jsonReadDouble(const nlohmann::json& j, const std::string& name, double& target ){
	if(j.contains(name)){
		target = j[name].get<double>();
	}
}

void jsonReadBool(const nlohmann::json& j, const std::string& name, bool& target){
	if(j.contains(name)){
		target = j[name].get<bool>();
	}
}

void jsonReadU32(const nlohmann::json& j, const std::string& name, u32& target){
	if(j.contains(name)){
		target = j[name].get<u32>();
	}
}

void jsonReadEnumU8(const nlohmann::json& j, const std::string& name, u8& target){
	if(j.contains(name)){
		int res = j[name].get<int>();
		target= (u8) res;
	}
}

void jsonReadEnumU8(const nlohmann::json& j, const std::string& name, u8& target,
	const std::map<std::string, u8>& namedValues){
	
	if(j.contains(name)){
		const nlohmann::json& field = j[name];
		if(field.is_number_integer()){
			int res = field.get<int>();
			target = (u8) res;
		}
		else if(field.is_string()){
			std::string field_str = field.get<std::string>();
			auto it = namedValues.find(field_str);
			if(it!=namedValues.end()){
				target = it->second;
			}
			else{
				std::ostringstream os;
				os<<"Unknown named enum value: \""
				<<field_str<<"\" of field \""<<name<<"\"";
				std::string s=os.str();
				throw std::runtime_error(s.c_str());
			}
		}
	}
}

}

void to_json(nlohmann::json& j, const Point2D&p){
	j["x"] = p.x;
	j["y"] = p.y;
}

void from_json(const nlohmann::json& j, Point2D&p){
	j.at("x").get_to(p.x);
	j.at("y").get_to(p.y);
}