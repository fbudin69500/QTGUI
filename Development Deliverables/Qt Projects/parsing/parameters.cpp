#include <QCoreApplication>
#include <QString>

bool checkBox;
QString textEdit;

void setcheckBox(bool toSet)
{
	checkBox = toSet;
}
void getcheckBox()
{
	return checkBox;
}
void settextEdit(QString toSet)
{
	textEdit = toSet;
}
void gettextEdit()
{
	return textEdit;
}