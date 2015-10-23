#include "mainwindow.h"

#include <QDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// set model
void MainWindow::setModel(Model_QTGUIExample m){
    this->m = m;
}

// get model
Model_QTGUIExample MainWindow::getModel(){
    return m;
}

// Save Listener
// - Updates the model with data from GUI
void MainWindow::on_pushButton_clicked()
{

    bool sb1 = ui->swagbox->isChecked();
    m.setswagbox(sb1);

    bool sb2 = ui->swagboxII->isChecked();
    m.setswagboxII(sb2);

    QString txt1 = ui->textEdit->toPlainText();
    m.settextEdit(txt1);

    QString txt2 = ui->textEdit_2->toPlainText();
    m.settextEdit_2(txt2);

    m.setpre_checkBoxsdf(ui->pre_checkBoxsdf->isChecked());

    m.setcomboBox(ui->comboBox->currentText());

    m.setdoubleSpinBox(ui->doubleSpinBox->value());

    m.sethorizontalScrollBar(ui->horizontalScrollBar->value());

    m.setverticalScrollBar(ui->verticalScrollBar->value());

    m.setlineEdit(ui->lineEdit->text());

    m.setradioButton(ui->radioButton->isChecked());

    m.setradioButton_2(ui->radioButton_2->isChecked());

    m.setpost_radioButton_3(ui->post_radioButton_3->isChecked());

    m.setspinBox(ui->spinBox->value());

    std::map<std::pair<unsigned long,QString>,bool> itemList ;
    for( unsigned long i = 0 ; i < ui->listWidget->count() ; i++ )
    {
      QListWidgetItem *currentItem = ui->listWidget->item(i) ;
      std::pair<unsigned long,QString> attribute ;
      attribute = std::make_pair(i,currentItem->text());
      itemList[ attribute ] = static_cast<bool>(currentItem->checkState() ) ;
    }
    m.setlistWidget(itemList) ;
    Save_QTGUIExample save;
    std::string filename = "data1.xml";
    save.save(m,filename);

}

// Load Listener
// - Updates the GUI with data from the model
void MainWindow::on_pushButton_2_clicked()
{
    Load_QTGUIExample loader;
    std::string filename = "data1.xml";
    loader.load(m,filename);


    ui->pre_checkBoxsdf->setChecked(m.getpre_checkBoxsdf());
    int comboBoxIndex = ui->comboBox->findText(m.getcomboBox() ) ;
    if( comboBoxIndex != -1 )
    {
      ui->comboBox->setCurrentIndex(comboBoxIndex);
    }
    ui->doubleSpinBox->setValue(m.getdoubleSpinBox());

    ui->horizontalScrollBar->setValue(m.gethorizontalScrollBar());

    ui->verticalScrollBar->setValue(m.getverticalScrollBar());

    ui->lineEdit->setText(m.getlineEdit());

    ui->radioButton->setChecked( m.getradioButton());

    ui->radioButton_2->setChecked(m.getradioButton_2());

    ui->post_radioButton_3->setChecked( m.getpost_radioButton_3());

    ui->spinBox->setValue(m.getspinBox());



    bool sb1 = m.getswagbox();
    ui->swagbox->setChecked(sb1);

    bool sb2 = m.getswagboxII();
    ui->swagboxII->setChecked(sb2);

    QString txt1 = m.gettextEdit();
    ui->textEdit->setText(txt1);

    QString txt2 = m.gettextEdit_2();
    ui->textEdit_2->setText(txt2);


    std::map<std::pair<unsigned long,QString>,bool> itemList ;
    itemList = m.getlistWidget() ;
    ui->listWidget->clear() ;
    for( std::map<std::pair<unsigned long,QString>,bool>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
    {
      QListWidgetItem *item = new QListWidgetItem( it->first.second , ui->listWidget ) ;
      item->setCheckState( it->second != 0 ? Qt::Checked : Qt::Unchecked ) ;
    }
}
