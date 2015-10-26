#include "mgen.h"

//constructor
MGen::MGen( MapType & hmap ,
            std::string directory ,
            std::string filename ,
            std::string prefix
          )
    :hmap(hmap),filename(prefix+filename),directory(directory)
{
  m_prefix = prefix ;
  m_indent = "  " ;
}

//destructor
MGen::~MGen()
{
}

//METHODS

//Outputs the hmap into the Model.cpp
void MGen::generateClass()
{
    std::ofstream headerstream;
    QString headerFileName = QDir(this->directory.c_str()).filePath((this->filename+".h").c_str());
    headerstream.open(headerFileName.toStdString().c_str(),std::ios::out);

    qDebug()<<headerFileName;

    try{
        if(headerstream.is_open()) {
            genHeader(headerstream);

        }
    }catch(...) {
        std::cout<<"Output to model cpp file failed"<<std::endl;

    }

    headerstream.close();

    std::ofstream sourcestream;
    QString sourceFileName = QDir(this->directory.c_str()).filePath((this->filename+".cpp").c_str());
    sourcestream.open(sourceFileName.toStdString().c_str(),std::ios::out);
    

    try{
        if(sourcestream.is_open()) {
            genSource(sourcestream);

        }
    }catch(...) {
        std::cout<<"Output to model cpp file failed"<<std::endl;

    }

    sourcestream.close();

}

void MGen::genHeader(std::ofstream & headerstream) {
    headerstream<<"#ifndef "+this->filename+"_H"<<std::endl;
    headerstream<<"#define "+this->filename+"_H"<<std::endl<<std::endl;

    //generate the includes
    genIncludes(headerstream);

    //print class keyword and the class name
    headerstream << "class "<<this->filename<<" {"<<std::endl;
    //generate the field
    genField(headerstream);

    //make getter and setters public
    headerstream <<std::endl<< "public: "<<std::endl<<std::endl;
    headerstream << m_indent << this->filename << "();"<<std::endl;

    genGetSet(headerstream,true);

    headerstream<<"};"<<std::endl<<std::endl;
    headerstream<<"#endif"<<std::endl;

}

void MGen::genSource(std::ofstream & sourcestream)
{
    sourcestream<<"#include \""+this->filename+".h\""<<std::endl<<std::endl;
    genConstructor(sourcestream);
    genGetSet(sourcestream,false);

}

void MGen::genConstructor(std::ofstream &sourcestream)
{
    //get iterator from begin
    MapType::const_iterator it=hmap.begin();
    sourcestream << m_indent <<this->filename + "::" + this->filename + "()" << std::endl ;
    sourcestream << m_indent << "{" << std::endl ;
    //while the point did not hit the end of the map
    while(it != hmap.end())
    {
        if( (it->second).compare( "QString" ) == 0 )
        {
          sourcestream << m_indent <<m_indent <<it->first.second.toStdString() << " = \"\" ;" << std::endl ;
        }
        else if( (it->second).compare( "double" ) == 0 )
        {
          sourcestream << m_indent <<m_indent <<it->first.second.toStdString() << " = 0.0 ;" << std::endl ;
        }
        else if( (it->second).compare( "int" ) == 0 )
        {
          sourcestream << m_indent <<m_indent <<it->first.second.toStdString() << " = 0 ;" << std::endl ;
        }
        else if( (it->second).compare( "bool" ) == 0 )
        {
          sourcestream << m_indent <<m_indent <<it->first.second.toStdString() << " = false ;" << std::endl ;
        }
        else
        {
          sourcestream << m_indent <<m_indent << "// " << it->first.second.toStdString() << " : " << it->second.toStdString() << " does not require any initialization" << std::endl ;
        }
        it++;
    }
    sourcestream << "}" << std::endl << std::endl ;
}

void MGen::AdditionalIncludeLibraries( std::ofstream & headerstream , QString headerName )
{
    if( usedHeader.find( headerName ) == usedHeader.end() ) //try to find the header first
    {
         headerstream << "#include <" << headerName.toStdString() << ">" << std::endl ; // put it into the header
         usedHeader.insert( headerName ) ; //put into set
    }
}

void MGen::genIncludes(std::ofstream & headerstream)
{
    //get iterator from begin
    MapType::const_iterator it=hmap.begin();
    while(it != hmap.end())
    {
        //compare the value type to the header QString for now, see if both are same string, aka when compare returns 0
        if( (it->second).compare( "QString" ) == 0 )
        {
          AdditionalIncludeLibraries( headerstream , "QString" ) ;
        }
        else if( (it->second).compare( "std::map<std::pair<unsigned long,QString>,bool>" ) == 0 )
        {
          AdditionalIncludeLibraries( headerstream , "QString" ) ;
          AdditionalIncludeLibraries( headerstream , "map" ) ;
          AdditionalIncludeLibraries( headerstream , "utility" ) ;
        }
        else if( (it->second).compare( "std::vector<std::vector<QString> >" ) == 0 )
        {
          AdditionalIncludeLibraries( headerstream , "QString" ) ;
          AdditionalIncludeLibraries( headerstream , "vector" ) ;
          AdditionalIncludeLibraries( headerstream , "stdexcept" ) ;
        }
        it++;
    }
    headerstream << std::endl ;
}


//generates fields
void MGen::genField(std::ofstream & headerstream)
{
    //get iterator from begin
    MapType::const_iterator it=hmap.begin();


    while(it != hmap.end())
    {

        headerstream<<m_indent<<it->second.toStdString()<<" "
                  <<it->first.second.toStdString()<<";"<<std::endl;



        it++;
    }


}


void MGen::genGetSet(std::ofstream & headerstream,bool headeronly)
{
    //get iterator from begin
    MapType::const_iterator it=hmap.begin();

    //while the point did not hit the end of the map
    while(it != hmap.end()) {

        //getter
        genGet(it,headerstream,headeronly);



        //setter
        genSet(it,headerstream,headeronly);

        it++;
    }

}

void MGen::genGet(MapType::const_iterator & it, std::ofstream & stream,bool headeronly)
{
    //generate Type getVarName();
   stream<<m_indent<<it->second.toStdString()<<" "<<(headeronly?"":this->filename+"::")
              <<"get"
             <<it->first.second.toStdString()<<"()";

    //Now depending on whether we are generating header or not
    if(headeronly)
    {
        stream<<";"<<std::endl;
        //exit, cause no need to generate the rest unless we are messing with source
        return;
    }
    else
    {
        stream<<"{"<<std::endl;

    }

    //generate body of getter
    stream<<m_indent<<m_indent<<"return "<<
                 it->first.second.toStdString()<<";"<<std::endl;

    //end it
    stream<<m_indent<<"}"<<std::endl;

}

void MGen::genSet(MapType::const_iterator & it, std::ofstream & stream, bool headeronly)
{
    //generate Type setVarName(Type a);
    stream<<m_indent<<"void "<<(headeronly?"":this->filename+"::")
              <<"set"
             <<it->first.second.toStdString()<<"("
            <<it->second.toStdString()
           <<" a"
          <<")";

    //Now depending on whether we are generating header or not
    if(headeronly)
    {
        stream<<";"<<std::endl<<std::endl;
        return;

    }
    else
    {
        stream<<"{"<<std::endl;

    }
    //generate body of setter
    // If the input is a tableview, then it has to have the same number of elements on each line
    if( (it->second).compare( "std::vector<std::vector<QString> >" ) == 0 )
    {
          stream<<m_indent<<m_indent<<"if( !a.empty() )"<<std::endl;
          stream<<m_indent<<m_indent<<"{"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<"std::vector<std::vector<QString> >::iterator it = a.begin(); "<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<"std::vector<std::vector<QString> >::size_type first_size = a[0].size();"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<"for( ; it != a.end() ; ++it )"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<"{"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<m_indent<<"if( it->size() != first_size )"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<m_indent<<"{"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<m_indent<<m_indent<<"throw std::runtime_error(\"Failed: All rows do not have the same size\");"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<m_indent<<"}"<<std::endl;
          stream<<m_indent<<m_indent<<m_indent<<"}"<<std::endl;
          stream<<m_indent<<m_indent<<"}"<<std::endl;
    }
    stream<<m_indent<<m_indent<<
                 it->first.second.toStdString()<<"=a;"
              <<std::endl;

    //end it
    stream<<m_indent<<"}"<<std::endl;

}








