#ifndef TOC_H_INCLUDE
#define TOC_H_INCLUDE

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdint> // for uint64_t

class ObjData;
class ObjType;

class ObjAtt
{
	friend class ObjData;
	friend class ObjType;
public:
	ObjAtt() {}
	ObjAtt(const std::string &name, const std::string &type) : m_att_name(name), m_att_type(type) {}
	std::string GetAttName() const { return m_att_name; }
	std::string GetAttType() const { return m_att_type; }
	void SetAttName(const std::string &dat) { m_att_name = dat; }
	void SetAttType(const std::string &dat) { m_att_type = dat; }
	void Clear() { m_att_name = ""; m_att_type = ""; }

private:
	std::string m_att_name;
	std::string m_att_type;
};

class ObjType
{
	friend class ObjData;
public:
	ObjType() {}
	ObjType(const std::string &obj_tname) : m_obj_typename(obj_tname) {}
	void AddAttribute(const ObjAtt &dat) { m_atts.emplace_back(dat); }
	void AddAttribute(const std::string &name, const std::string &type) { m_atts.emplace_back(name,type); }
	std::string GetObjTypename() const {	return m_obj_typename;	}
	int GetNAttributes() const { return m_atts.size(); }
	void SetObjTypename(const std::string &dat) { m_obj_typename = dat; }
	std::string AsString()
	{
		// character: char_name str : char_type str : charlocx int : charlocy int
		std::string result(m_obj_typename);
		result += " : ";
		for (auto cur_att : m_atts)
		{
			result += cur_att.GetAttName();
			result += " : ";
			result += cur_att.GetAttType();
		}
		result += "\n";
		return result;
	}

	private:
	std::string m_obj_typename;
	std::vector< ObjAtt > m_atts;
};

class ObjData
{
public:
	ObjData(const ObjType &obj, std::vector<std::string> &row);
	std::string m_obj_typename;
	std::map< std::string, double > m_dbl_data;
	std::map < std::string, int > m_int_data;
	std::map< std::string, std::string > m_str_data;
};

class Toc
{
public:
	Toc(const std::string &filename);
	void LoadObjects();
	void PrintSummary() const;
	bool HasError() const;
	void GetObjData(const std::string &datatype, std::vector<ObjData> &results);
	const std::string& GetErrorMessage() const;
	std::string GetFileName() const;

private:
	bool AtEof() const;

	bool ReadToCharLoadSplitsNotSized(char endChar, char delim,
		std::vector<std::string> &row);
	void ClearErrors();

	bool IsOpen() const;
	bool ReadFileChunk(uint64_t startpos);

	std::string m_filename;
	std::vector< ObjType> m_objtypes;
	std::vector< ObjData> m_objdata;
	void LoadObjTypes();

	char *m_buffer;
	uint64_t m_bufferStartPos;
	uint64_t m_bufferEndPos;
	uint64_t m_curPos;
	uint64_t m_relPos;
	uint64_t m_fileSize;
	bool m_small_cache;
	uint64_t m_currentLine;
	mutable bool m_Error;
	mutable std::string m_ErrorMessage;
};
#endif
