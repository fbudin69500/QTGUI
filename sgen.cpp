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
        else if( !it->second.compare( "std::map<std::pair<unsigned long,QString>,bool>" ) )
        {
            genMethodLine(savestream , "{" , tab_index ) ;
            genMethodLine(savestream , "std::map<std::pair<unsigned long,QString>,bool> map = m.get"+std::string(it->first.second.toStdString())+"();" , tab_index + 1 ) ;
            genMethodLine(savestream , "std::map<std::pair<unsigned long,QString>,bool>::iterator it = map.begin();" , tab_index + 1 ) ;
            genMethodLine(savestream , "for( int count = 0 ; it != map.end() ; count++ , it++ )" , tab_index + 1 ) ;
            genMethodLine(savestream , "{" , tab_index + 1 ) ;
            genMethodLine(savestream , "std::string item = \"item\" + QString::number(count).toStdString() ;" , tab_index + 2 ) ;
            genMethodLine(savestream , "std::string itemName = item+\"Name\" ;" , tab_index + 2 ) ;
            genMethodLine(savestream , "writer.writeAttribute(itemName.c_str(),it->first.second);" , tab_index + 2 ) ;
            genMethodLine(savestream , "std::string itemState = item+\"isChecked\" ;" , tab_index + 2 ) ;
            genMethodLine(savestream , "writer.writeAttribute(itemState.c_str(),QString::number(it->second));" , tab_index + 2 ) ;
            genMethodLine(savestream , "std::string itemPosition = item+\"Position\" ;" , tab_index + 2 ) ;
            genMethodLine(savestream , "writer.writeAttribute(itemPosition.c_str(),QString::number(it->first.first));" , tab_index + 2 ) ;
            genMethodLine(savestream , "}" , tab_index + 1 ) ;
            genMethodLine(savestream , "}" , tab_index ) ;
        }
        else if( !it->second.compare( "std::vector<std::vector<QString> >" ) )
        {
            genMethodLine(savestream , "{" , tab_index ) ;
            genMethodLine(savestream , "std::vector<std::vector<QString> > table = m.get"+std::string(it->first.second.toStdString())+"();" , tab_index + 1 ) ;
            genMethodLine(savestream , "if( !table.empty() )" , tab_index + 1 ) ;
            genMethodLine(savestream , "{" , tab_index + 1 ) ;
            genMethodLine(savestream , "writer.writeAttribute(\"row\", QString::number(table.size()) );" , tab_index + 2 ) ;
            genMethodLine(savestream , "writer.writeAttribute(\"column\", QString::number(table[0].size()) );" , tab_index + 2 ) ;
            genMethodLine(savestream , "}" , tab_index + 1 ) ;
            genMethodLine(savestream , "std::vector<std::vector<QString> >::iterator it_table_1 = table.begin();" , tab_index + 1 ) ;
            genMethodLine(savestream , "for( size_t count = 0 ; it_table_1 != table.end() ; count++ , it_table_1++ )" , tab_index + 1 ) ;
            genMethodLine(savestream , "{" , tab_index + 1 ) ;
            genMethodLine(savestream , "std::vector<QString> list_2 = *it_table_1;" , tab_index + 2 ) ;
            genMethodLine(savestream , "std::vector<QString>::iterator it_table_2 = list_2.begin();" , tab_index + 2 ) ;
            genMethodLine(savestream , "for( std::vector<QString>::size_type count2 = 0 ; it_table_2 != list_2.end() ; count2++ , it_table_2++ )" , tab_index + 2 ) ;
            genMethodLine(savestream , "{" , tab_index + 2 ) ;

            genMethodLine(savestream , "std::string item = \"item_\" + QString::number(count).toStdString() + \"_\" + QString::number(count2).toStdString() ;" , tab_index + 3 ) ;
            genMethodLine(savestream , "writer.writeAttribute(item.c_str(),*it_table_2);" , tab_index + 3 ) ;
            genMethodLine(savestream , "}" , tab_index + 2 ) ;
            genMethodLine(savestream , "}" , tab_index + 1 ) ;
            genMethodLine(savestream , "}" , tab_index ) ;
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


