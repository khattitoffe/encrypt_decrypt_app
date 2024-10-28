#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<fstream>
#include <string>
#include <cctype>
#include <iostream>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QString filepath="";
QString destination="";
QString decryptName="";


void MainWindow::on_clickToSelectFile_clicked()
{
    filepath=QFileDialog::getOpenFileName(this,tr("Select File"),"D://","All files (*.*);;Text Files (*.txt);; Music Files(*.mp3)");
}



void MainWindow::on_BackupDestination_clicked()
{
    destination= QFileDialog::getExistingDirectory(this, tr("Open Directory"), "E://",QFileDialog::ShowDirsOnly);
}




/*
bool copyFile(const std::string sourceFile,const std::string& destinationFile,const std::string& filename)
{
    std::string file("");
    file=destinationFile+filename;
    char data;
    std::ifstream i_file(sourceFile,std::ios::binary);
    std::ofstream o_file(file,std::ios::binary);

    while(i_file.get(data))
    {
        o_file.put(data);

    }
    i_file.close();
    o_file.close();

    return true;
}*/

int key_val(const std::string& key, const int& pos)
{
    return key[pos%key.length()];
}

void vigenere(std::fstream& file, const double& size, const std::string& key, const int& coding)
{
    char c;
    setbuf(stdout, NULL);

    for (int i=0; file.get(c); i++)
    {
        c+=coding*key_val(key, i);
        file.seekg(i, file.beg);
        file.put(c);
    }

}
double filesize(const std::string filepath)
{
    std::ifstream file(filepath.c_str(),std::ios::ate);

    double size=file.tellg();
    file.close();
    return size;
}


bool copyBinData(const std::string filepath,const std::string destinationFile)//used for binary file
{
    const int bufferSize=2048;
    std::vector<char> buffer(bufferSize);

    std::ifstream source(filepath,std::ios::binary);
    std::ofstream destination(destinationFile,std::ios::binary);

    if(!source)
        return false;
    if(!destination)
        return false;

    while(!source.eof())
    {
        source.read(buffer.data(),bufferSize); //reads data in groups
        std::streamsize bytes=source.gcount();

        if(bytes>0)
            destination.write(buffer.data(),bytes);

    }

    source.close();
    destination.close();

    return true;

}

bool ceaserCipher(std::string& content,bool val)//passing reference of string,nno need to return value
{
    int shift=val ? 5 : -5;

    for(char& ch:content)
    {
        if(isalpha(ch))
        {
            char base=isupper(ch)?'A':'a';
            ch=static_cast<char>((ch-base+shift+26)%26+base);
        }
    }

    return true;
}

bool encryptFile(std::string Filename,bool val)
{
    std::ifstream inFile(Filename);
    if(!inFile)
        return false;

    std::string content((std::istreambuf_iterator<char>(inFile)),{});
    inFile.close();

    if(ceaserCipher(content,val))
    {
        remove(Filename.c_str());

        std::ofstream out(Filename);
        out << content;
        out.close();
        return true;
        //write the data to the new file;
    }
    return false;
}

void MainWindow::on_createBackup_clicked()
{

    if(filepath=="" || destination=="")
    {
        QMessageBox::warning(this,"Error","Please select the Backup-Destination and file to be Backed-up First");
        return;
    }

    QString filename=ui->filename->text();//gets the filename

    if(filename=="")
    {
        QMessageBox::warning(this,"Error","Please Mention the Filename.");
    }



    //copyFile(filepath.toStdString(),destination.toStdString(),filename.toStdString());
/*
    if(check)
    {
        QMessageBox::warning(this,"Error","Error in Opening File.");
    }
*/
    std::string sourceFile=filepath.toStdString();
    destination=destination+"/"+filename;
    std::string destinationFile=destination.toStdString();

    int ln=sourceFile.size();

    //if file is binary we use this
    if(sourceFile.substr(ln-4).compare(".txt")!=0)
    {
        if(copyBinData(sourceFile,destinationFile))
             QMessageBox::information(this,"Completion","Task Completed");
        else
             QMessageBox::warning(this,"Error","Failed");
        return;
    }

    char data;
    std::ifstream i_file(sourceFile);
    std::ofstream o_file(destinationFile);


    while(i_file.get(data))
    {
        o_file.put(data);

    }
    i_file.close();
    o_file.close();

    QMessageBox::information(this,"Completion","Task Completed");
}


void MainWindow::on_encryptButton_clicked()
{
    std::string sourceFile=destination.toStdString();
    int ln=sourceFile.size();

    if(sourceFile.substr(ln-4).compare(".txt")==0){
        if(encryptFile(destination.toStdString(),true))
        {
            // std::string recordFile="record.txt";

            QMessageBox::information(this,"Completion","Encryption Completed");
        }
        else
        {
            QMessageBox::warning(this,"Error","Encryption Failed");
        }
        return;
    }

    if(ui->encryptionkey->text()=="")
    {
        QMessageBox::warning(this,"Error","Encryption Key not given");
        return;
    }
    QString key=ui->encryptionkey->text();

    double size=filesize(sourceFile);

    std::fstream file(sourceFile,std::ios::in | std::ios::out);
    vigenere(file,size,key.toStdString(),1);//we pass 1 when we want to encrpyt

    QMessageBox::information(this,"Sucess","Encryption Completed");
}


void MainWindow::on_DecryptSelectFile_clicked()
{
    decryptName=QFileDialog::getOpenFileName(this,tr("Select File"),"E://","All files (*.*);;Text Files (*.txt);; Music Files(*.mp3)");
}



void MainWindow::on_DecryptButton_clicked()
{
    if(decryptName=="")
    {
        QMessageBox::warning(this,"Error","First Select a file to Decrypt");
        return;
    }

    std::string sourceFile=decryptName.toStdString();
    int ln=sourceFile.size();


    if(sourceFile.substr(ln-4).compare(".txt")==0){
      if(encryptFile(decryptName.toStdString(),false))//this will decrypt our file
      {
        // std::string recordFile="record.txt";
        QMessageBox::information(this,"Completion","Decryption Completed");
      }
      else
      {
        QMessageBox::warning(this,"Error","Decryption Failed");
      }
      return;
    }
    QString key=ui->decryptionkey->text();

    double size=filesize(sourceFile);
    std::fstream file(sourceFile,std::ios::in | std::ios::out);
    vigenere(file,size,key.toStdString(),-1);//we pass 1 when we want to encrpyt

    QMessageBox::information(this,"Sucess","Decryption Completed");

}

