#include "parse.h"

Parser::Parser( std::string uifileuri , std::string prefix )
{
  m_uifileuri = uifileuri ;
  m_prefix = prefix.c_str() ;
}

int Parser::parseXML( std::map< std::pair< QString , QString > , QString> & m )
    {
        /* We'll parse the example.xml */
        QFile* file = new QFile(m_uifileuri.data());

        /* If we can't open it, let's show an error message. */
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString errMsg = file->errorString();
            QFile::FileError err = QFile::NoError; //in Qt5, replace QFile by  QFileDevice
            err = file->error();
            qDebug() << err;
            return 1;
        }

        /* QXmlStreamReader takes any QIODevice. */
        QXmlStreamReader xml(file);

        /* We'll parse the XML until we reach end of it.*/
        while(!xml.atEnd() && !xml.hasError())
        {

            QXmlStreamReader::TokenType token = xml.readNext();     // Read next element

            if(token == QXmlStreamReader::StartDocument)
            {   //If token is just StartDocument, we'll go to next
                continue;
            }

            else if (xml.qualifiedName() == "widget" && xml.isEndElement() != true)
            {
                QString widgetType = xml.attributes().value("", "class").toString() ;
                QString widgetName = xml.attributes().value("", "name").toString() ;
                if( !widgetName.startsWith( m_prefix ) )
                {
                  continue ;
                }
                if (widgetType.compare("QTextEdit") == 0
                    || widgetType.compare("QComboBox") == 0
                    || widgetType.compare("QLineEdit") == 0)
                {
                    m[std::make_pair(widgetType, widgetName)] = "QString";
                }
                else if (widgetType.compare("QCheckBox") == 0
                         || widgetType.compare("QRadioButton") == 0)
                {
                    m[std::make_pair(widgetType, widgetName)] = "bool";
                }
                else if (widgetType.compare("QDoubleSpinBox") == 0)
                {
                    m[std::make_pair(widgetType, widgetName)] = "double";
                }
                else if (widgetType.compare("QSpinBox") == 0
                         || widgetType.compare("QScrollBar") == 0)
                {
                    m[std::make_pair(widgetType, widgetName)] = "int";
                }
                else if (widgetType.compare("QListWidget") == 0
			 || widgetType.compare("QTableView") == 0)
                {
                    m[std::make_pair(widgetType, widgetName)] = "std::map<std::pair<unsigned long,QString>,bool>";
                }
                // Store values into the dictionary
    //            m[std::make_pair(widgetType, xml.attributes().at(1).value().toString())] = "";

                // Print values to the console

            }

        }
        /* Error handling. */
        if(xml.hasError())
        {

        }
        /* Removes any device() or data from the reader
         * and resets its internal state to the initial state. */
        xml.clear();

        file->close();
        delete file;

        return 0;
    }
