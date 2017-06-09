#ifndef TOC_H_INCLUDE
#define TOC_H_INCLUDE

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdint> // for uint64_t

class ObjAtt
{
public:
	std::string m_att_name;
	std::string m_att_type;
};

class ObjType
{
public:
	std::string m_obj_typename;
	std::vector< ObjAtt > m_atts;
};

class ObjData
{
public:
	ObjData(const ObjType &obj, std::vector<std::string> &row);
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
