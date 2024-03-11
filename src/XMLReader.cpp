#include "XMLReader.h"
#include"XMLEntity.h"
#include <expat.h>
#include <iostream>
#include <queue>

struct CXMLReader::SImplementation{
    std::shared_ptr< CDataSource > DDataSource;
    XML_Parser DXMLParser;
    std::queue< SXMLEntity > DEntityQueue;

    void StartElementHandler(const std::string &name, const std::vector<std::string> &attrs){
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::StartElement;
        for (size_t Index=0; Index<attrs.size(); Index+=2){
            TempEntity.SetAttribute(attrs[Index], attrs[Index+1]);
        }//set attributes
        DEntityQueue.push(TempEntity);//push to queue
    };

    void EndElementHandler(const std::string &name){
        SXMLEntity TempEntity;
        TempEntity.DNameData=name;
        TempEntity.DType=SXMLEntity::EType::EndElement;
        DEntityQueue.push(TempEntity);
    };//create end element entity and push to quene

    void CharacterDataHandler(const std::string &cdata){
        if (!cdata.empty() && cdata.find_first_not_of(" \t\n\r") != std::string::npos){
            SXMLEntity TempEntity;
            TempEntity.DNameData = cdata;
            TempEntity.DType = SXMLEntity::EType::CharData;
            DEntityQueue.push(TempEntity);
        }//create character data entity and push to queue
    };

    static void StartElementHandlerCallback(void *context, const XML_Char *name, const XML_Char **atts){
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        std::vector<std::string> Attributes;
        auto AttrPtr = atts;
        while (*AttrPtr){
            Attributes.push_back(*AttrPtr);
            AttrPtr++;
        }//push attributes to vector
        ReaderObject->StartElementHandler(name, Attributes);
    };//call start element handler

    static void EndElementHandlerCallback(void *context, const XML_Char *name){
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        ReaderObject->EndElementHandler(name);
    };//call end element handler

    static void CharacterDataHandlerCallback(void *context, const XML_Char *s, int len){
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        ReaderObject->CharacterDataHandler(std::string(s,len));
    };//call character data handler

    SImplementation(std::shared_ptr< CDataSource > src){
        DDataSource = src;
        DXMLParser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(DXMLParser, StartElementHandlerCallback);
        XML_SetEndElementHandler(DXMLParser, EndElementHandlerCallback);
        XML_SetCharacterDataHandler(DXMLParser, CharacterDataHandlerCallback);
        XML_SetUserData(DXMLParser, this);
    };

    // Returns true if all rows have been read from the DSV
    bool End() const{
        return (DEntityQueue.empty() && DDataSource->End());
    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        //Reader from source if necessary
        //pass to XML_Parse function
        //Return entity
        while (DEntityQueue.empty()){
            std::vector<char> DataBuffer;
            if (DDataSource->Read(DataBuffer, 512)){
                XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), DataBuffer.size() < 512);
            }
            else{
                XML_Parse(DXMLParser, DataBuffer.data(), 0, true);
            }
        }
        if (DEntityQueue.empty()){
            return false;
        }//if queue is empty, return false
        if (skipcdata == true){
            for (size_t i=0; i<DEntityQueue.size(); i++){
                if (DEntityQueue.front().DType == SXMLEntity::EType::CharData){
                    DEntityQueue.pop();
                }
            }
        }//if skipcdata is true, skip character data
        entity = DEntityQueue.front();
        DEntityQueue.pop();
        return true;
    }
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique< SImplementation >(src);
};

CXMLReader::~CXMLReader(){
};

bool CXMLReader::End() const{
    return DImplementation->End();
};

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return DImplementation->ReadEntity(entity, skipcdata);
};
