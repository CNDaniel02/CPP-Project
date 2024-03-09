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
	return DImplementation->dsvsrc->End();//�����û�е�ĩβ
}

bool CDSVReader::ReadRow(std::vector<std::string>& row) {
	if (End()) {
		return false;
	}
	std::vector<char> vec;
	char ch;
	std::string line;
	while (!DImplementation->dsvsrc->End() && DImplementation->dsvsrc->Get(ch)) {//��ֱ�����з��ţ����ҰѶ����ļӳ�line
		if (ch == '\n') {
			break;
		}
		line += ch;
	}

	if (line.empty()) {//������У�������
		return false;
	}
	std::stringstream stream(line);
	std::string b;
	
	
	while (std::getline(stream, b, DImplementation->fenge)) {//extract blocks
		row.push_back(b);
	}

	if (!line.empty() && line.back() == DImplementation->fenge) {//������һ���ַ��Ƿָ��ַ������һ�����ַ������
		row.emplace_back("");
	}
	
	return true;

	
}

