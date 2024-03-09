
#include<sstream>
#include "DSVWriter.h"
#include <iomanip>

struct CDSVWriter::SImplementation {
	std::shared_ptr<CDataSink> sink;//shared pointer to data sink
	char fenge;//delimiter
	bool incluall;//flag
};


//constructor
CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char fenge, bool incluall)
	:DImplementation(new SImplementation{ sink,fenge,incluall }) {}

CDSVWriter::~CDSVWriter() = default;//destructor




bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {//writes a row to the sink
	std::ostringstream strstream;
	for (ssize_t i = 0; i < row.size();i++) {//循环一个row里的每个element
		if (i > 0) {//如果不是第一个element prepend fenge
			strstream << DImplementation->fenge;
		}
		if (DImplementation->incluall || row[i].find(DImplementation->fenge) != std::string::npos) {
			//enclose the field in quotes if inclu is true or field contains fenge
			strstream << '"' << row[i] << '"';
		}
		else {
			//else 直接写
			strstream << row[i];
		}
	}
	strstream << '\n';//在最后添加换行符
	std::string data = strstream.str();
	//write row and return success
	return DImplementation->sink->Write(std::vector<char>(data.begin(), data.end()));
	
	


}


