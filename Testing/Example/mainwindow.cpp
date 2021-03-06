#include "mainwindow.h"

#include <QDebug>
#include <iostream>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("0Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("1Header")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("2Header")));
    ui->tableView->setModel(model);
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

    std::map<std::pair<unsigned long,QString>,bool> itemList2 ;
    for( unsigned long i = 0 ; i < ui->listWidget_2->count() ; i++ )
    {
      QListWidgetItem *currentItem = ui->listWidget_2->item(i) ;
      std::pair<unsigned long,QString> attribute ;
      attribute = std::make_pair(i,currentItem->text());
      itemList2[ attribute ] = static_cast<bool>(currentItem->checkState() ) ;
    }
    m.setlistWidget_2(itemList2) ;
    std::vector<std::vector<QString> > my_table ;
    if( ui->tableView->model() )
    {
      my_table.resize(ui->tableView->model()->rowCount());
      for( unsigned long i = 0 ; i < ui->tableView->model()->rowCount() ; i++ )
      {
        std::vector<QString> my_list(ui->tableView->model()->columnCount()) ;
        for( unsigned long j = 0 ; j < ui->tableView->model()->columnCount() ; j++ )
        {
          QModelIndex index = ui->tableView->model()->index(i,j,QModelIndex());
          my_list[ j ] = ui->tableView->model()->data(index).toString();
        }
        my_table[ i ] = my_list ;
      }
    }
    m.settableView(my_table);
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

    std::map<std::pair<unsigned long,QString>,bool> itemList2 ;
    itemList2 = m.getlistWidget_2() ;
    ui->listWidget_2->clear() ;
    for( std::map<std::pair<unsigned long,QString>,bool>::iterator it = itemList2.begin() ; it != itemList2.end() ; it++ )
    {
      QListWidgetItem *item = new QListWidgetItem( it->first.second , ui->listWidget_2 ) ;
      item->setCheckState( it->second != 0 ? Qt::Checked : Qt::Unchecked ) ;
    }

    std::vector<std::vector<QString> > my_table  = m.gettableView();
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->tableView->model());
    if(model)
    {
    model->clear();
    unsigned long row = my_table.size() ;
    if( row )
    {
      unsigned long column = my_table[0].size() ;
      if( column )
      {
        model->setRowCount(row);
        model->setColumnCount(column);
        for( unsigned long i = 0 ; i < column ; i++ )
        {
          model->setHorizontalHeaderItem(i, new QStandardItem(QString::number(i)+QString("Header")));
          for( unsigned j = 0 ; j < row ; j++ )
          {
            QModelIndex index = ui->tableView->model()->index(j,i,QModelIndex());
            ui->tableView->model()->setData( index , QVariant(my_table[ j ][ i ]) ) ;
          }
        }
      }
      }
    }
}
