#include "DSVReader.h"
#include <sstream>

struct CDSVReader::SImplementation {
	std::shared_ptr<CDataSource> dsvsrc;
	char fenge; //delimiter
	
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource>dsvsrc, char fenge) //constructor
	:DImplementation(new SImplementation{ dsvsrc, fenge }) {}

CDSVReader::~CDSVReader() = default;//destructor

bool CDSVReader::End() const {
	return DImplementation->dsvsrc->End();//检查有没有到末尾
}

bool CDSVReader::ReadRow(std::vector<std::string>& row) {
	if (End()) {
		return false;
	}
	std::vector<char> vec;
	char ch;
	std::string line;
	while (!DImplementation->dsvsrc->End() && DImplementation->dsvsrc->Get(ch)) {//读直到换行符号，并且把读到的加成line
		if (ch == '\n') {
			break;
		}
		line += ch;
	}

	if (line.empty()) {//如果空行，不操作
		return false;
	}
	std::stringstream stream(line);
	std::string b;
	
	
	while (std::getline(stream, b, DImplementation->fenge)) {//extract blocks
		row.push_back(b);
	}

	if (!line.empty() && line.back() == DImplementation->fenge) {//如果最后一个字符是分割字符，添加一个空字符在最后
		row.emplace_back("");
	}
	
	return true;

	
}

