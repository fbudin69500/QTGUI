#include "sgen.h"


//constructor
SGen::SGen( std::map < std::pair < QString , QString > , QString > & hmap ,
            std::string modelclass ,
            std::string directory ,
            std::string filename ,
            std::string prefix
          )
    :hmap(hmap),directory(directory), filename(prefix+filename),modelclass(prefix+modelclass)
{
  m_prefix = prefix ;

}

//destructor
SGen::~SGen(){


}

//METHODS

//Outputs the hmap into the Saver.h
void SGen::generateClass() {
    std::ofstream savehstream;
    QString headerFileName = QDir(this->directory.c_str()).filePath((this->filename+".h").c_str());
    savehstream.open(headerFileName.toStdString().c_str(),std::ios::out);
    qDebug()<<headerFileName;

    try{
        if(savehstream.is_open()) {
            genMethodLine(savehstream,"#ifndef "+this->filename+"_H",0);
            genMethodLine(savehstream,"#define "+this->filename+"_H",0);

            //generate the includes
            genIncludes(savehstream);

            //print class keyword and the class name
            savehstream << "class "<<filename<<"{ "<<std::endl;



            //generate the field
            genField(savehstream);



            //make getter and setters public
            savehstream <<std::endl<< "public: "<<std::endl<<std::endl;

            //generate save method
            genMethod(savehstream, true);


            savehstream<<"};"<<std::endl;

            genMethodLine(savehstream,"#endif",0);

        }
    }catch(...) {
        std::cout<<"Output to model cpp file failed"<<std::endl;

    }

    savehstream.close();


    std::ofstream savesstream;
    QString cppFileName = QDir(this->directory.c_str()).filePath((this->filename+".cpp").c_str());
    savesstream.open(cppFileName.toStdString().c_str(),std::ios::out);

    try{
        if(savesstream.is_open()) {
            genMethodLine(savesstream,"#include \""+this->filename+".h\"",0);

            //generate save method
            genMethod(savesstream, false);

        }
    }catch(...) {
        std::cout<<"Output to model cpp file failed"<<std::endl;

    }

    savesstream.close();
}



void SGen::genIncludes(std::ofstream & savehstream) {
    savehstream<<INCLUDEHEADER<<" <QXmlStreamWriter>"<<std::endl;
    savehstream<<INCLUDEHEADER<<" <QFile>"<<std::endl<<std::endl;
    savehstream<<INCLUDEHEADER<<" <QString>"<<std::endl<<std::endl;
    savehstream<<INCLUDEHEADER<<" <string>"<<std::endl<<std::endl;
    savehstream<<INCLUDEHEADER<<" \""+this->modelclass+".h\""<<std::endl<<std::endl;
    savehstream<<INCLUDEHEADER<<" <QDebug>"<<std::endl<<std::endl;

}


//generates fields
void SGen::genField(std::ofstream & savehstream){



}



//generate the save method in the saver
void SGen::genMethod(std::ofstream & savestream, bool isHeader) {
    //get iterator from begin
    MapType::const_iterator it=hmap.begin();

    //used to determine amount of tab needed to properly indent this line
    int tab_index=1;

    //generate method signature
    genMethodLine(savestream,"void "+(isHeader? ("save("+this->modelclass+"& m , std::string filename );")
                                             :(filename+"::save("+this->modelclass+"& m , std::string filename){")),tab_index);


    //do not generate the rest if not header
    if(isHeader) {
        genSavetoXMLMethod(savestream,it, tab_index, isHeader);
        return;

    }

    tab_index++;

    //generate the file
    genMethodLine(savestream,"//First, address the appropriate instantiate",tab_index);
    genMethodLine( savestream,"QFile file(filename.c_str());",tab_index);

    //open the file
    genMethodLine(savestream,"if(!file.open(QIODevice::WriteOnly)){",tab_index);
    //write lines in error handling
    genMethodLine(savestream,"//handle file open errors here ",tab_index+1);
    genMethodLine(savestream," qDebug()<<file.exists();",tab_index+1);
    genMethodLine(savestream,"}\n",tab_index);

    //generate the xml writer
    genMethodLine(savestream,"QXmlStreamWriter writer(&file);\n",tab_index);

    //call method that actually writes each field in model to xml file
    genMethodLine(savestream,"//write each field stored in model into xml",tab_index);
    genMethodLine(savestream,"saveToXML(m,writer);\n",tab_index);

    genMethodLine(savestream,"}\n\n",--tab_index);

    genSavetoXMLMethod(savestream,it, tab_index, isHeader);
}

//generate each line of method, indented. Also, puts ; at end. Default is one tab
void SGen::genMethodLine(std::ofstream & savestream,std::string s,int tab_amount) {

    for(;tab_amount>0;tab_amount--)
        savestream<<"\t";

    savestream<<s<<std::endl;
}

void SGen::genSavetoXMLMethod(std::ofstream & savestream,MapType::const_iterator & it, int tab_index, bool isHeader) {


    //first generate the method signature
    genMethodLine(savestream,"void "+(isHeader? ("saveToXML("+this->modelclass+"& m,QXmlStreamWriter& writer);"
                                       ):(filename+std::string("::saveToXML("+this->modelclass+"& m,QXmlStreamWriter& writer){\n"))),tab_index);

    if(isHeader) {
            return;

    }

    tab_index++;
    //generate xml headers
    genMethodLine(savestream,"writer.setAutoFormatting(true);\n",tab_index);

    //start document
    genMethodLine(savestream,"writer.writeStartDocument();\n\n",tab_index);

    //parent namespace and
    std::string saverElement = std::string("writer.writeStartElement(\"" ) + m_prefix.c_str() + "Parameters\"" + ");" ;
    genMethodLine(savestream,saverElement,tab_index);


    bool void_pointer=false;
    //recusively generate the tag for data
    while(it != hmap.end()) {
        //generate
        genMethodLine(savestream,"writer.writeEmptyElement(\"Parameter\");",tab_index);
        //Type attribute
        genMethodLine(savestream,"writer.writeAttribute(\"type\",\""+std::string(it->second.toStdString())+"\");",tab_index);
        //convert qstring to string
        genMethodLine(savestream,"writer.writeAttribute(\"name\",\""+std::string(it->first.second.toStdString())+"\");",tab_index);
        //value attribute generation
        //saves data, falls under different cases
        if( !it->second.compare( "QString" ) )
        {
            //can just output the qstring
            genMethodLine(savestream,"writer.writeAttribute(\"value\",m.get"+std::string(it->first.second.toStdString())+"());",tab_index);
        }
        else if( !it->second.compare( "int" )
                || !it->second.compare( "double" )
                || !it->second.compare( "bool" )
               )
        {
            //can just output the qstring
            genMethodLine(savestream,"writer.writeAttribute(\"value\",QString::number(m.get"+std::string(it->first.second.toStdString())+"()));",tab_index);
        }
        else if( !it->second.compare( "std::map<QString,bool>" ) )
        {
            genMethodLine(savestream,"std::map<QString,bool> map_"+ std::string(it->first.second.toStdString()) + " = m.get"+std::string(it->first.second.toStdString())+"();",tab_index);
            genMethodLine(savestream,"std::map<QString,bool>::iterator it_"+std::string(it->first.second.toStdString()) + " = map_"+std::string(it->first.second.toStdString())+".begin();",tab_index);
            genMethodLine(savestream,"for( int count = 0 ; it_"+std::string(it->first.second.toStdString())+" != map_"+std::string(it->first.second.toStdString())+".end() ; count++ , it_"+std::string(it->first.second.toStdString())+"++ )",tab_index);
            genMethodLine(savestream,"{",tab_index);
            genMethodLine(savestream, "std::string item = \"item\" + QString::number(count).toStdString() ;" , tab_index + 1 ) ;
            genMethodLine(savestream, "std::string itemName = item+\"Name\" ;" , tab_index + 1 ) ;
            genMethodLine(savestream,"writer.writeAttribute(itemName.c_str(),it_"+std::string(it->first.second.toStdString())+"->first);",tab_index+1);
            genMethodLine(savestream, "std::string itemState = item+\"isChecked\" ;" , tab_index + 1 ) ;
            genMethodLine(savestream,"writer.writeAttribute(itemState.c_str(),QString::number(it_"+std::string(it->first.second.toStdString())+"->second));",tab_index+1);
            genMethodLine(savestream,"}",tab_index);
        }
        it++;
    }
    genMethodLine(savestream,"writer.writeEndElement();\n",tab_index);
    //end document
    genMethodLine(savestream,"writer.writeEndDocument();\n",tab_index);
    tab_index--;
    //close the method
    genMethodLine(savestream,"}",tab_index);

}


