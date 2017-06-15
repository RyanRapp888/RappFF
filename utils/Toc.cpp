#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdint> // for uint64_t
#include "toc.h"

Toc::Toc(const std::string &filename)
{
	m_filename = filename;
	m_buffer = NULL;
	m_bufferStartPos = 0;
	m_bufferEndPos = 0;
	m_curPos = 0;
	m_relPos = 0;
	m_fileSize = 0;
	m_ErrorMessage = "";
	m_Error = false;
	m_currentLine = 0;
	ReadFileChunk(0);
	LoadObjTypes();

}


// Note: this function does not rewind,
// it should remain private
void Toc::LoadObjTypes()
{
	bool types_start_found(false);
	bool types_end_found(false);

	std::vector<std::string> row;
	char newlinechar = '\n';
	char spacechar = ' ';
	bool done_with_toc(false);

	while (!done_with_toc && ReadToCharLoadSplitsNotSized(newlinechar, spacechar, row))
	{
		if (row.size() == 1 && row[0] == "starttypes")
		{
			types_start_found = true;
			continue;
		}
		else if (row.size() == 1 && row[0] == "endtypes")
		{
			done_with_toc = true;
			break;
		}
		else if (types_start_found && row.size() >= 4)
		{
			// minimum case "<objtype> : <obj_att_name> <obj_att_type> : ..." == 5 tokens
			ObjType obj_elect;
			int cur_mode(0); // 0 - reading objtype, 1 - reading attname, 2 - reading attype
			ObjAtt att_elect;

			for (auto curtoken : row)
			{
				if (curtoken == ":")
				{
					att_elect.m_att_name = "";
					att_elect.m_att_type = "";
					cur_mode = 1;
				}
				else
				{
					if (cur_mode == 0)
					{
						obj_elect.m_obj_typename = curtoken;
						cur_mode++;
					}
					else if (cur_mode == 1)
					{
						att_elect.m_att_name = curtoken;
						cur_mode++;
					}
					else
					{
						att_elect.m_att_type = curtoken;
						if (obj_elect.m_obj_typename != "" &&
							att_elect.m_att_name != "" &&
							att_elect.m_att_type != "")
						{
							obj_elect.m_atts.emplace_back(att_elect);
						}
					}
				}
			}
			if (obj_elect.m_atts.size() > 0) m_objtypes.emplace_back(obj_elect);
		}
		row.clear(); // not necessary, but just for safety
	}
}

ObjData::ObjData(const ObjType &objtyp, std::vector<std::string> &row)
{
	int rowsize = static_cast<int>(row.size());
	int cur_token_idx(0);
	
	if (rowsize == objtyp.m_atts.size() + 1)
	{
		if (row[0] == objtyp.m_obj_typename)
		{
			cur_token_idx = 1;
			for (const auto curatt : objtyp.m_atts)
			{
				if (curatt.m_att_type == "str")
				{
					m_str_data[curatt.m_att_name] = row[cur_token_idx];
					cur_token_idx++;
				}
				else if (curatt.m_att_type == "dbl")
				{
					std::istringstream tmp(row[cur_token_idx]);
					double tmp_dbl;
					tmp >> tmp_dbl;
					m_dbl_data[curatt.m_att_name] = tmp_dbl;
					cur_token_idx++;
				}
				else if (curatt.m_att_type == "int")
				{
					std::istringstream tmp(row[cur_token_idx]);
					int tmp_int;
					tmp >> tmp_int;
					m_int_data[curatt.m_att_name] = tmp_int;
					cur_token_idx++;
				}
			}
		}
	}
}

void Toc::GetObjData(const std::string &datatype, std::vector<ObjData> &results)
{
	results.clear();
	for (const auto &dat : m_objdata)
	{
		if (dat.m_obj_typename == datatype)
		{
			results.emplace_back(dat);
		}
	}
}
// Note: this function does not rewind,
// it should remain private
void Toc::LoadObjects()
{
	bool startdata_found(false);
	bool enddata_found(false);

	std::vector<std::string> row;
	char newlinechar = '\n';
	char spacechar = ' ';
	bool donewithdata(false);

	while (!donewithdata && ReadToCharLoadSplitsNotSized(newlinechar, spacechar, row))
	{
		if (row.size() == 1 && row[0] == "startdata")
		{
			startdata_found = true;
			continue;
		}
		else if (row.size() == 1 && row[0] == "enddata")
		{
			donewithdata = true;
			break;
		}
		else if (startdata_found && row.size() > 1)
		{
			std::string token = row[0];
			int found_idx(-1);

			for (int aa = 0; aa < m_objtypes.size(); aa++)
			{
				if (token == m_objtypes[aa].m_obj_typename)
				{
					found_idx = aa;
					break;
				}
			}

			if (found_idx >= 0)
			{
				ObjData tempobj(m_objtypes[found_idx], row);
				tempobj.m_obj_typename = token;
				m_objdata.emplace_back(tempobj);
			}
		}
	}
}

void Toc::PrintSummary() const
{

}

bool Toc::ReadFileChunk(uint64_t startpos)
{
	std::ifstream input_filestr;
	if (m_buffer != NULL)
	{
		delete m_buffer;
	}
	m_buffer = NULL;
	input_filestr.open(m_filename.c_str(), std::ios::binary);

	// indicate an error if the file didn't open properly.
	if (!input_filestr.is_open())
	{
		m_bufferStartPos = 0;
		m_bufferEndPos = 0;
		m_curPos = 0;
		m_relPos = 0;
		m_fileSize = 0;
		m_Error = true;
		m_ErrorMessage = "Unable to open file (" + m_filename + ")";
		return !m_Error;
	}

	input_filestr.seekg(startpos, std::ios::end);

	uint64_t buff_size_bytes;
	buff_size_bytes = input_filestr.tellg();

	if (buff_size_bytes == 0)
	{
		m_bufferStartPos = 0;
		m_bufferEndPos = 0;
		m_curPos = 0;
		m_relPos = 0;
		m_fileSize = 0;
		m_Error = true;
		m_ErrorMessage = "Unable to read data from (" + m_filename + ")";
		return !m_Error;
	}

	if (startpos == 0)
	{
		m_fileSize = buff_size_bytes;
	}

	input_filestr.seekg(startpos, std::ios::beg);

	// Attempt to allocate enough bytes to buffer to the end of the file.
	// If we can't allocate the block, ask for half until we succeed.
	while (m_buffer == NULL)
	{
		m_buffer = new char[buff_size_bytes];
		if (m_buffer == NULL)
		{
			buff_size_bytes / 2;
		}
	}

	// Read in the amount that we succeeded to buffer
	input_filestr.read(m_buffer, buff_size_bytes);
	if (m_buffer == NULL)
	{
		m_ErrorMessage = "Unable to read data from (" + m_filename + ")";
		m_Error = true;
		return !m_Error;
	}
	m_curPos = startpos;
	m_bufferStartPos = startpos;
	m_bufferEndPos = m_bufferStartPos + buff_size_bytes;
	m_relPos = 0;
	return !m_Error;
}

bool Toc::IsOpen() const
{
	//we don't keep the file open, so "open" just
	// means that we have something in the buffer
	return m_fileSize > 0 && m_buffer != NULL;
}

bool Toc::AtEof() const
{
	if (m_curPos >= m_fileSize)
	{
		return true;
	}

	return false;
}

bool Toc::ReadToCharLoadSplitsNotSized(char endChar, char delim, std::vector<std::string> &row)
{
	row.clear();
	bool found_endChar = false;
	std::string curword("");
	bool in_a_quoted_string = false;

	while (m_curPos < m_fileSize)
	{
		if (m_curPos >= m_bufferEndPos)
		{
			// We hit the end of our current chunk, read a new chunk of the file
			// starting at our current position.
			ReadFileChunk(m_curPos);
		}

		found_endChar = false;
		if (m_buffer[m_relPos] != endChar)
		{
			if (m_buffer[m_relPos] != delim || in_a_quoted_string)
			{
				int asciival = m_buffer[m_relPos];
				if (!(asciival == 13 || asciival == 34))
				{
					//ignore carriage returns, don't add double quotes
					curword += m_buffer[m_relPos];
				}

				if (asciival == 34) in_a_quoted_string = !in_a_quoted_string;
			}
			else
			{
				row.push_back(curword);
				curword = "";
			}
		}
		else
		{
			found_endChar = true;
			m_currentLine++;
			row.push_back(curword);
			curword = "";
		}

		m_relPos++;
		m_curPos++;

		if (found_endChar)
		{
			break;
		}
	}

	// We hit the end of file before finding endChar
	// so we wipe out the result string we'd been building up
	if (!found_endChar)
	{
		//if curtoken is zero, we're dealing with a blank line at the end of the file,
		//just pretend it isn't there
		if (row.empty())
		{
			return false;
		}
		row.push_back(curword);
	}
	return true;
}

bool Toc::HasError() const
{
	return m_Error;
}

const std::string& Toc::GetErrorMessage() const
{
	return m_ErrorMessage;
}

std::string Toc::GetFileName() const
{
	return m_filename;
}

void Toc::ClearErrors()
{
	m_Error = false;
	m_ErrorMessage = "";
	return;
}

/*
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "usage: toc <filename>\n";
		return 1;
	}

	Toc t1("examp.toc");
	t1.LoadObjects();
	if (t1.HasError())
	{
		std::string err = t1.GetErrorMessage();
		std::cout << "ERROR: " << err << std::endl;
	}
}
*/
