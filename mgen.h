#ifndef MGEN_H
#define MGEN_H

//define file name is not already done
#ifndef MODELFILENAME
#define MODELFILENAME "../COMP523ProjectQTParamCp/model.h"
#endif



//Define the header of the class
#define CLASSHEADER \
    "class Model {"

//Termination character
#define TERMINATECHAR\
    "}"

//Head for the setters
#define SETTERHEAD\
    "void set"


//Includes
#include <QDir>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <QString>
#include <QDebug>

//Class used to strip data from the dictionary and streams into a cpp file current in
//the same directory
class MGen{
    const std::map<std::pair<QString,QString>,QString> & hmap;

    //headers that have been used
    std::set<QString> usedHeader;

    //name of the header file to be generated
    std::string filename;
    //directory the file should be stored in
    std::string directory;

public:

    //constructor
    MGen( std::map< std::pair< QString , QString > , QString > & hmap ,
          std::string directory = "" ,
          std::string filename = "model"
        ) ;

    //destructor
    ~MGen();

    //METHODS

    //called to generate the whole class
    void generateClass();

private:

    //generate the apprpriate includes, used by genHeader
    void genIncludes(std::ofstream& headerstream);

    //generate the associated header file
    void genHeader(std::ofstream& headerstream);

    //generate the source file
    void genSource(std::ofstream& sourcestream);


    //generates fields
    void genField(std::ofstream & headerstream);

    void genGetSet(std::ofstream & stream,bool headeronly);

    void genGet(std::map<std::pair<QString,QString>,QString>::const_iterator & it, std::ofstream & stream,bool headeronly);

    void genSet(std::map<std::pair<QString,QString>,QString>::const_iterator & it, std::ofstream & stream,bool headeronly);



};


#endif // MGEN_H