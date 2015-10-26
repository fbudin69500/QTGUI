
#include "lgen.h"
#include <iostream>

//constructor
LGen::LGen( std::map< std::pair< QString , QString > , QString > & hmap ,
            std::string modelclass ,
            std::string directory ,
            std::string filename ,
            std::string prefix
          )
{
    m_map = hmap ;
    m_modelclass = prefix + modelclass ;
    m_directory = directory ;
    m_filename = prefix + filename ;
    m_prefix = prefix ;
    m_indent = "  " ;
}

void LGen::generateClass()
{
    QString headerFileName = QDir( m_directory.c_str() ).filePath( ( m_filename + ".h" ).c_str() ) ;
    QFile loaderh( headerFileName ) ;
    if( loaderh.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream out( &loaderh ) ;
        out << "#ifndef " << m_filename.c_str() << "_H\n" ;
        out << "#define " << m_filename.c_str() << "_H\n\n" ;

        out << "#include <QCoreApplication>\n" ;
        out << "#include <QString>\n" ;
        out << "#include <QDebug>\n" ;
        out << "#include <map>\n" ;
        out << "#include <list>\n" ;
        out << "#include <QFile>\n" ;
        out << "#include <QStringList>\n" ;
        out << "#include <iostream>\n" ;
        out << "#include \"" << m_modelclass.c_str() << ".h\"\n" ;
        out << "#include <QXmlStreamReader>\n" ;

        out << "\n\n" ;
        out << "class " ;
        out << m_filename.c_str() ;
        out << "{" ;
        out << "\n\n" ;


        out << m_indent << "QString tmpClassName;\n" ;
        out << m_indent << "QString tmpClassType;\n" ;

        out << "public:\n" ;

        out << "\n" << m_indent << m_modelclass.c_str() << " load(" << m_modelclass.c_str() << "& model , std::string filename );\n" ;

        out << "\n};\n" ;
        out << "#endif" ;
    }
    else
    {
        qDebug() << "Loader failed to open" ;
    }
    loaderh.close() ;

    //source file
    QString sourceFileName = QDir( m_directory.c_str() ).filePath(( m_filename + ".cpp" ).c_str() ) ;
    QFile loaders( sourceFileName ) ;
    if( loaders.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream out( &loaders ) ;

        out << "#include \"" << m_filename.c_str() << ".h\"" ;

        out << "\n\n" << m_indent << m_modelclass.c_str() << " " << m_filename.c_str() << "::load(" << m_modelclass.c_str() << "& model , std::string filename)\n" << m_indent << "{\n" ;

        out << m_indent << m_indent << "std::map<std::pair<QString, QString>, QString> xmlTokens;\n" ;
        out << m_indent << m_indent << "typedef std::map<std::pair<QString, QString>, QString> Dict;\n" ;
        out << m_indent << m_indent << "typedef Dict::const_iterator It;\n\n" ;



        out << m_indent << m_indent << "QFile* data = new QFile(filename.c_str());\n" ;
        out << m_indent << m_indent << "if (!data->open(QIODevice::ReadOnly | QIODevice::Text)) \n" << m_indent << m_indent << "{\n" ;
        out << m_indent << m_indent << m_indent << "qDebug() << \"Not read only\";\n" ;
        out << m_indent << m_indent << "}\n\n" ;
        out << m_indent << m_indent << "QXmlStreamReader readXML(data);\n" ;
        out << m_indent << m_indent << "std::map<QString , QList< std::pair< QString , QString > > > attributes ;\n" ;
        out << m_indent << m_indent << "while(!readXML.atEnd() && !readXML.hasError()) \n" << m_indent << m_indent << "{\n" ;
        out << m_indent << m_indent << m_indent << "QXmlStreamReader::TokenType token = readXML.readNext();\n" ;
        out << m_indent << m_indent << m_indent << "if(token == QXmlStreamReader::StartDocument) \n" << m_indent << m_indent << m_indent << "{\n" ;
        out << m_indent << m_indent << m_indent << m_indent << "continue;\n" ;
        out << m_indent << m_indent << m_indent << "}\n" ;
        out << m_indent << m_indent << m_indent << "else if (readXML.isEndElement() != true) \n" << m_indent <<m_indent << m_indent << "{\n" ;
        out << m_indent << m_indent << m_indent << m_indent << "if(readXML.attributes().size()>2)\n" ;
        out << m_indent << m_indent << m_indent << m_indent << "{\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << "QList< std::pair< QString , QString > > currentAttributes;\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << "for( int i = 0 ; i < readXML.attributes().size() ; i++ )\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair< QString , QString > xmlAttribute ;\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "xmlAttribute = std::make_pair( readXML.attributes()[i].name().toString() , readXML.attributes()[i].value().toString() ) ;\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.append( xmlAttribute ) ;\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << "attributes[ readXML.attributes().value(\"name\").toString() ] = currentAttributes ;\n" ;
        out << m_indent << m_indent << m_indent << m_indent << m_indent << "xmlTokens[std::make_pair(readXML.attributes().value(\"type\").toString(), readXML.attributes().value(\"name\").toString())] = readXML.attributes().value(\"value\").toString();\n" ;
        out << m_indent << m_indent << m_indent << m_indent << "}\n" ;
        out << m_indent << m_indent << m_indent << "}\n" ;
        out << m_indent << m_indent << "}\n\n" ;

        for( MapType::const_iterator it = m_map.begin() ; it != m_map.end() ; ++it )
        {
            out << m_indent << m_indent << "tmpClassType = \"" << it->second << "\";\n" ;
            out << m_indent << m_indent << "tmpClassName = \"" << it->first.second << "\";\n\n" ;
            out << m_indent << m_indent << "for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) \n" <<m_indent << m_indent << "{\n" ;

            out << m_indent << m_indent << m_indent << "if((it -> first.second).compare(tmpClassName) == 0) \n" << m_indent << m_indent << m_indent << "{\n" ;

            if( (it->second).compare( "QString" ) == 0 )
            {
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "model.set" << it->first.second ;
                out << "((QString)(it->second));\n" ;
            }
            else if( (it->second).compare( "bool" ) == 0 )
            {
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "int boolValue = ((QString)(it->second)).toInt();\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "model.set" << it->first.second ;
                out << "(static_cast<bool>(boolValue));\n" ;
            }
            else if( (it->second).compare( "double" ) == 0 )
            {
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "double doubleValue = ((QString)(it -> second)).toDouble();\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "model.set" << it->first.second ;
                out << "(doubleValue);\n" ;
            }
            else if( (it->second).compare( "int" ) == 0 )
            {
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "int intValue = ((QString)(it->second)).toInt();\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "model.set" << it->first.second ;
                out << "(intValue);\n" ;
            }
            else if( (it->second).compare( "std::map<std::pair<unsigned long,QString>,bool>" ) == 0 )
            {
                QString mapName = it->first.second + "Map" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "std::map<std::pair<unsigned long,QString>,bool> " << mapName << " ;\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "QList< std::pair< QString , QString > > currentAttributes = attributes[ \"" << it->first.second << "\" ];\n" ;
                //there should be at least 4 elements to load anything: type, name, itemName and itemIsChecked(). Additionally, there has to be an even number of elements
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "if( (currentAttributes.size()-2) % 3 != 0 || currentAttributes.size() < 5 )\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::cerr << \"Wrong number of attributes for \"<< it->first.second.toStdString() << \":\" << currentAttributes.size() << std::endl ;\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "continue ;\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the type
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the 2nd item should be the name
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "int itemNb = 0 ;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "while( currentAttributes.size() > 1 )\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<QString , QString> attributeName = currentAttributes[ 0 ];\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "QString name = QString(\"item\") + QString::number(itemNb) + \"Name\";\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<QString , QString> attributeIsChecked = currentAttributes[ 1 ];\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "QString isChecked = QString(\"item\") + QString::number(itemNb) + \"isChecked\";\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<QString , QString> attributePosition = currentAttributes[ 2 ];\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "QString position = QString(\"item\") + QString::number(itemNb) + \"Position\";\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "if( !attributeName.first.compare(name) && !attributeIsChecked.first.compare(isChecked) && !attributePosition.first.compare(position))\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<unsigned long,QString> attribute ;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "attribute = std::make_pair( attributePosition.second.toULong() , attributeName.second );\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << mapName << "[ attribute ] = ( attributeIsChecked.second.toInt() != 0 ? true : false ) ;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the itemXName
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the itemXisChecked
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the itemXPosition
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "itemNb++;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "model.set" << it->first.second ;
                out << "(" << mapName << ") ;\n " ;
            }
           /* else if( (it->second).compare( "std::vector<std::vector<QString> >" ) == 0 )
            {
                QString tableName = it->first.second + "Table" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "std::vector<std::vector<QString> >" << tableName << " ;\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "QList< std::pair< QString , QString > > currentAttributes = attributes[ \"" << it->first.second << "\" ];\n" ;
                //there should be at least 4 elements to load anything: type, name, itemName and itemIsChecked(). Additionally, there has to be an even number of elements
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "if( (currentAttributes.size()-2) % 3 != 0 || currentAttributes.size() < 5 )\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::cerr << \"Wrong number of attributes for \"<< it->first.second.toStdString() << \":\" << currentAttributes.size() << std::endl ;\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "continue ;\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the type
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the 2nd item should be the name
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "int itemNb = 0 ;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "while( currentAttributes.size() > 1 )\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<QString , QString> attributeName = currentAttributes[ 0 ];\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "QString name = QString(\"item\") + QString::number(itemNb) + \"Name\";\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<QString , QString> attributeIsChecked = currentAttributes[ 1 ];\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "QString isChecked = QString(\"item\") + QString::number(itemNb) + \"isChecked\";\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<QString , QString> attributePosition = currentAttributes[ 2 ];\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "QString position = QString(\"item\") + QString::number(itemNb) + \"Position\";\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "if( !attributeName.first.compare(name) && !attributeIsChecked.first.compare(isChecked) && !attributePosition.first.compare(position))\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "{\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "std::pair<unsigned long,QString> attribute ;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "attribute = std::make_pair( attributePosition.second.toULong() , attributeName.second );\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << mapName << "[ attribute ] = ( attributeIsChecked.second.toInt() != 0 ? true : false ) ;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the itemXName
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the itemXisChecked
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "currentAttributes.removeFirst();\n";//the first item should be the itemXPosition
                out << m_indent << m_indent << m_indent << m_indent << m_indent << m_indent << "itemNb++;\n";
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "}\n" ;
                out << m_indent << m_indent << m_indent << m_indent << m_indent << "model.set" << it->first.second ;
                out << "(" << mapName << ") ;\n " ;
            }*/
            out << m_indent << m_indent << m_indent << "}\n"<<m_indent<<m_indent<<"}" ;
            out << "\n\n" ;
        }

        out << m_indent << m_indent << "return model;" ;
        out << "\n" << m_indent << "}" ;
    }
    loaders.close() ;
}




