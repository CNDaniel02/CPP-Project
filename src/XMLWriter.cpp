#include "XMLWriter.h"
#include <sstream>
#include <stack>
#include <iostream>



struct CXMLWriter::SImplementation {
	std::shared_ptr<CDataSink> Sink;
	std::stack<std::string>OpenElements;//track elements
	explicit SImplementation(std::shared_ptr<CDataSink> Sink) : Sink(std::move(Sink)) {}



	bool WriteStartElement(const SXMLEntity& entity) {
		std::ostringstream stream;
		stream << '<' << entity.DNameData;
		for (const auto& attribute : entity.DAttributes) {
			stream << ' ' << attribute.first << "=\"" << attribute.second << '"';
		}
		stream << '>';
		OpenElements.push(entity.DNameData);//remember elements name;
		auto str = stream.str();
		return Sink->Write(std::vector<char>(str.begin(), str.end()));
		
		
	}

	bool WriteEndElement(const SXMLEntity& entity) {
		std::ostringstream stream;
		if (!OpenElements.empty() && OpenElements.top() == entity.DNameData) {
			OpenElements.pop(); //pop element from stack
		}
		stream << "</" << entity.DNameData << '>';
		auto str = stream.str();
		return Sink->Write(std::vector<char>(str.begin(), str.end()));
		
	}

	bool WriteCharData(const SXMLEntity& entity) {
		return Sink->Write(std::vector<char>(entity.DNameData.begin(), entity.DNameData.end()));
		
	}

	bool WriteCompleteElement(const SXMLEntity& entity) {
		if (!WriteStartElement(entity)) {
			return false;
		}
		if (!entity.DNameData.empty()) {
			if (!WriteCharData(entity)) {
				return false;
			}
		}
		return WriteEndElement(entity);
		
	}


};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) :DImplementation(new SImplementation(std::move(sink))) {

}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush() {
	bool success = true;
	while (!DImplementation->OpenElements.empty()) {
		std::string elementName = DImplementation->OpenElements.top();
		DImplementation->OpenElements.pop();
		std::ostringstream stream;
		stream << "</" << elementName << '>';
		auto str = stream.str();
		success &= DImplementation->Sink->Write(std::vector<char>(str.begin(), str.end()));
	}
	
	return success;
}

bool CXMLWriter::WriteEntity(const SXMLEntity& entity) {
	switch (entity.DType) {
	case SXMLEntity::EType::StartElement:
		return DImplementation->WriteStartElement(entity);
	case SXMLEntity::EType::EndElement:
		return DImplementation->WriteEndElement(entity);
	case SXMLEntity::EType::CharData:
		return DImplementation->WriteCharData(entity);
	case SXMLEntity::EType::CompleteElement:
		return DImplementation->WriteCompleteElement(entity);
	default:
		return false; 
	}
	
	
}


