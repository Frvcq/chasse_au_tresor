#include "clientcrawler.h"
#include "ui_clientcrawler.h"

/**
 * @brief ClientCrawler::ClientCrawler
 * @param parent
 * Le constructeur construit la grille et met en place les widgets
 */

ClientCrawler::ClientCrawler(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientCrawler)
{
    ui->setupUi(this);
    grille = new QGridLayout();
    socketClient = new QTcpSocket();
    for(int ligne=0; ligne<TAILLE; ligne++)
    {
        for (int colonne=0; colonne<TAILLE; colonne++)
        {
            QToolButton *b=new QToolButton();
            grille->addWidget(b,ligne,colonne,1,1);
        }
    }
    // Placement sur la grille des objets présents sur ui
    // les labels
    grille->addWidget(ui->labelAdresseServeur,TAILLE,0,1,5);
    grille->addWidget(ui->labelNumeroPort,TAILLE,6,1,5);
    grille->addWidget(ui->labelDistance,TAILLE+3,0,1,5);
    grille->addWidget(ui->labelInformations,TAILLE+3,12,1,5);
    // les QLineEdit adresse et port
    grille->addWidget(ui->lineEditAdresseServeur,TAILLE+1,0,1,5);
    grille->addWidget(ui->spinBoxPortServeur,TAILLE+1,6,1,5);
    // les QPushButton connexion et quitter
    grille->addWidget(ui->pushButtonConnexion,TAILLE+2,0,1,5);
    grille->addWidget(ui->pushButtonQuitter,TAILLE+2,6,1,5);
    // les QPushButton fleches
    grille->addWidget(ui->pushButtonUp,TAILLE,15,1,1);
    grille->addWidget(ui->pushButtonLeft,TAILLE+1,14,1,1);
    grille->addWidget(ui->pushButtonRight,TAILLE+1,16,1,1);
    grille->addWidget(ui->pushButtonDown,TAILLE+2,15,1,1);
    //distance et informations
    grille->addWidget(ui->lcdNumberDistance,TAILLE+3,6,1,5);
    this->setLayout(grille);
    connect(socketClient,&QTcpSocket::connected,this,&ClientCrawler::onQtcpSocket_connected);
    connect(socketClient,&QTcpSocket::disconnected,this,&ClientCrawler::onQtcpSocket_disconnected);
    connect(socketClient,&QTcpSocket::readyRead,this,&ClientCrawler::onQtcpSocket_readyRead);
    connect(socketClient,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&ClientCrawler::onQtcpSocket_error);
}
/**
 * @brief ClientCrawler::~ClientCrawler
 * Détruit l'interface visuelle
 */
ClientCrawler::~ClientCrawler()
{
    delete ui;
}
/**
 * @brief ClientCrawler::onQtcpSocket_connected
 * lorsque qque l'on se connecte
 */
void ClientCrawler::onQtcpSocket_connected()
{

}
/**
 * @brief ClientCrawler::onQtcpSocket_disconnected
 * lorsque l'on se déconnecte
 */
void ClientCrawler::onQtcpSocket_disconnected()
{

}
/**
 * @brief ClientCrawler::onQtcpSocket_error
 * @param socketError
 * Une erreur avec la socket
 */
void ClientCrawler::onQtcpSocket_error(QAbstractSocket::SocketError socketError)
{
    ui->labelInformations->setText(socketClient->errorString());
}
/**
 * @brief ClientCrawler::onQtcpSocket_readyRead
 * Lors d'une réception de donnée on met a jour nos données
 */
void ClientCrawler::onQtcpSocket_readyRead()
{
    quint16 taille=0;
    QString message;
    QPoint pts;
    double distance;
    //si le nombre d'octers recu est au moin egal a celui de la taille de que l'on doit recevoir
    if(socketClient->bytesAvailable()>=(quint64)sizeof (taille)){
        //association de la socket au flux d'entree
        QDataStream in(socketClient);
        //extraire la taille de ce que l'on doit recevoir
        in >> taille;
        //si le nombre d'octets recu est au moin egal a celui  de ce que l'on foit recevoir
        if(socketClient->bytesAvailable()>=(quint64)taille){
            //extraire le reste
            in >>pts>>message>>distance;
        }

        ui->labelInformations->setText(message);
        ui->lcdNumberDistance->display(distance);
        point=pts;
        // ne pas oublier a effacer

       // grille->itemAtPosition(point.x(),point.y())->widget()->setStyleSheet("background-color : black");


    }


}

/**
 * @brief ClientCrawler::on_pushButtonConnexion_clicked
 * Initialisation de la connexion
 */
void ClientCrawler::on_pushButtonConnexion_clicked()
{
    if(ui->pushButtonConnexion->text()=="Connexion"){
        socketClient->connectToHost(ui->lineEditAdresseServeur->text(),ui->spinBoxPortServeur->text().toInt());
        ui->pushButtonConnexion->setText("Deconnexion");

    }
    else {
        ui->pushButtonConnexion->setText("Connexion");
        socketClient->disconnectFromHost();

    }
}
/**
 * @brief ClientCrawler::on_pushButtonUp_clicked
 * bouge en haut
 */

void ClientCrawler::on_pushButtonUp_clicked()
{
    quint16 taille=0;
    QChar commande ='U';
    QBuffer tampon;
    //association du tampon au flux de sortie
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);

    //construction de la trame
    out << taille << commande;
    //calcul de la taille de la trame
    taille = tampon.size()-sizeof(taille);
    //placement sur la première position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille réel de la trame
    out << taille;
    //envoi du QByteArray du tampon via la socket
    socketClient->write(tampon.buffer());
   //grille->itemAtPosition(point.y(),point.x())->widget()->setStyleSheet("background-color : black");
  // grille->itemAtPosition(point.y(),point.x())->widget()->setStyleSheet("background-color : white");

}
/**
 * @brief ClientCrawler::on_pushButtonLeft_clicked
 * bouge a gauche
 */

void ClientCrawler::on_pushButtonLeft_clicked()
{
    quint16 taille=0;
    QChar commande ='L';
    QBuffer tampon;
    //association du tampon au flux de sortie
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);

    //construction de la trame
    out << taille << commande;
    //calcul de la taille de la trame
    taille = tampon.size()-sizeof(taille);
    //placement sur la première position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille réel de la trame
    out << taille;
    //envoi du QByteArray du tampon via la socket
    socketClient->write(tampon.buffer());
    grille->itemAtPosition(point.y(),point.x())->widget()->setStyleSheet("background-color : black");
   // grille->itemAtPosition(point.y(),point.x()+1)->widget()->setStyleSheet("background-color : white");
}
/**
 * @brief ClientCrawler::on_pushButtonRight_clicked
 * bouge a droite
 */
void ClientCrawler::on_pushButtonRight_clicked()
{
    quint16 taille=0;
    QChar commande ='R';
    QBuffer tampon;
    //association du tampon au flux de sortie
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);

    //construction de la trame
    out << taille << commande;
    //calcul de la taille de la trame
    taille = tampon.size()-sizeof(taille);
    //placement sur la première position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille réel de la trame
    out << taille;
    //envoi du QByteArray du tampon via la socket
    socketClient->write(tampon.buffer());
    grille->itemAtPosition(point.y(),point.x())->widget()->setStyleSheet("background-color : black");
     // grille->itemAtPosition(point.y(),point.x()-1)->widget()->setStyleSheet("background-color : white");
}
/**
 * @brief ClientCrawler::on_pushButtonDown_clicked
 * Bouge en bas
 */

void ClientCrawler::on_pushButtonDown_clicked()
{
    quint16 taille=0;
    QChar commande ='D';
    QBuffer tampon;
    //association du tampon au flux de sortie
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);

    //construction de la trame
    out << taille << commande;
    //calcul de la taille de la trame
    taille = tampon.size()-sizeof(taille);
    //placement sur la première position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille réel de la trame
    out << taille;
    //envoi du QByteArray du tampon via la socket
    socketClient->write(tampon.buffer());
    grille->itemAtPosition(point.y(),point.x())->widget()->setStyleSheet("background-color : black");
    //grille->itemAtPosition(point.y()-1,point.x())->widget()->setStyleSheet("background-color : white");
}
