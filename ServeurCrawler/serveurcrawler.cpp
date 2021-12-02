#include "serveurcrawler.h"
#include "ui_serveurcrawler.h"
/**
 * @brief ServeurCrawler::ServeurCrawler
 * @param parent
 * COnstructeur du serveur
 */
ServeurCrawler::ServeurCrawler(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServeurCrawler)
{
    ui->setupUi(this);
    socketEcoute = new QTcpServer(this);
    connect(socketEcoute,&QTcpServer::newConnection,this,&ServeurCrawler::onQTcpServer_newConnection);

}
/**
 * @brief ServeurCrawler::~ServeurCrawler
 * Destructeur du serveur
 */
ServeurCrawler::~ServeurCrawler()
{
    delete ui;
    // delete socketEcoute;
}
/**
 * @brief ServeurCrawler::onQTcpSocket_readyRead
 * S'active quand on intéragie avec une donnée
 */
void ServeurCrawler::onQTcpSocket_readyRead()
{
    {
        QTcpSocket *client=qobject_cast<QTcpSocket*>(sender());

        quint16 taille = 0;
        QChar commande;
        QPoint CoordonnerJoueur;

        int index;
        if (!client)
        {
            QMessageBox msg;
            msg.setText("erreur de lecture : "+client->errorString());
            msg.exec();
        }
        else
        {
            index=listeSocketClient.indexOf(client);
            CoordonnerJoueur= listePositions.at(index);
            //si le nombre d'octets reçu est au moins égal à celui de la taille de ce que l'on doit recevoir
            if(client->bytesAvailable() >= (qint64)sizeof (taille)){
                //association de la socket au flux d'entrée
                QDataStream in(client);
                //extraire la taille de ce que l'on doit recevoir
                in >> taille;
                //si le nombre d'octets reçu est au moins égal à celui de ce que l'on doit recevoir
                if(client->bytesAvailable() >= (qint64)taille){
                    //extraire la demande du client
                    in >> commande;
                    switch (commande.toLatin1()) {
                    //haut
                    case 'U' :
                        //LA CASE EST LA CASE DE DEPART ET NON CELLE DU CLIENT
                        CoordonnerJoueur.setY(listePositions.at(index).y()+1);
                        EnvoyerDonnees(client,CoordonnerJoueur,"Start");
                        qDebug()<<"UP : "<<CoordonnerJoueur;
                        break;
                        //bas
                    case 'D' :
                        CoordonnerJoueur.setY(listePositions.at(index).y()-1);
                        EnvoyerDonnees(client,CoordonnerJoueur,"Start");
                        qDebug()<<"down : "<<CoordonnerJoueur;
                        //gauche
                    case 'L' :
                        CoordonnerJoueur.setX(listePositions.at(index).x()-1);
                        EnvoyerDonnees(client,CoordonnerJoueur,"Start");
                        qDebug()<<"left : "<<CoordonnerJoueur;
                        break;
                    case 'R':
                        //droite
                        CoordonnerJoueur.setX(listePositions.at(index).x()+1);
                        EnvoyerDonnees(client,CoordonnerJoueur,"Start");
                        qDebug()<<"right : "<<CoordonnerJoueur;
                        break;
                    }
                }
            }
        }
    }
}

/**
 * @brief ServeurCrawler::onQTcpServer_newConnection
 * S'active quand un client se connecte
 */
void ServeurCrawler::onQTcpServer_newConnection()
{
    QTcpSocket *client;
    QPoint pt;
    //ajout de la socket de communication a la liste
    client = socketEcoute->nextPendingConnection();
    listeSocketClient.append(client);
    //on donne la position
    pt =DonnerPositionUnique();
    listePositions.append(pt);
    //envoie le message
    EnvoyerDonnees(client,pt,"Start");
    //vider la grille
    // ViderGrille();
    //afficher la grille
   //  AfficherGrille();
    // on fait nos connect
    connect(client,&QTcpSocket::readyRead,this,&ServeurCrawler::onQTcpSocket_readyRead);
    connect(client,&QTcpSocket::disconnected,this,&ServeurCrawler::onQTcpSocket_disconnected);
    qDebug()<<"un Client s'est connecté";


}
/**
 * @brief ServeurCrawler::onQTcpSocket_disconnected
 * s'active quand un client se déconecte
 */

void ServeurCrawler::onQTcpSocket_disconnected()
{
    qDebug()<<"un Client s'est déconnecté";
}
/**
 * @brief ServeurCrawler::EnvoyerDonnees
 * @param client le client
 * @param pt    le point du client
 * @param msg      le message a envoyé
 */
void ServeurCrawler::EnvoyerDonnees(QTcpSocket *client, QPoint pt, QString msg)
{
    quint16 taille = 0;
    QBuffer tampon;
    double distance;
    distance = CalculerDistance(pt);
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);
    //construction de la trame
    out<<taille<<pt<<msg<<distance;
    //calcul de la taille de la trame
    taille = tampon.size()-sizeof (taille);
    //placement sur la premiere position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la taille de la trame
    out << taille;
    //envoie du QByteArray du tampon via la socket
    client->write(tampon.buffer());
    //lesConnexionsClients.at(lesConnexionsClients.indexOf(client))->write(msg.toLatin1());
}
/**
 * @brief ServeurCrawler::AfficherGrille
 * Affiche la grille
 */
void ServeurCrawler::AfficherGrille()
{
    for(int ligne=0; ligne<TAILLE; ligne++)
        {
            for (int colonne=0; colonne<TAILLE; colonne++)
            {
                QToolButton *b=new QToolButton();
                grille->addWidget(b,ligne,colonne,1,1);
            }
        }
}
/**
 * @brief ServeurCrawler::ViderGrille
 * Vide la grille
 */
void ServeurCrawler::ViderGrille()
{
    /*for(int ligne=0; ligne<TAILLE; ligne++)
        {
            for (int colonne=0; colonne<TAILLE; colonne++)
            {
                QToolButton *b=new QToolButton();
                grille->removeWidget(b,ligne,colonne,1,1);
            }
        }*/
}
/**
 * @brief ServeurCrawler::DonnerPositionUnique
 * @return
 * Donner une position au point
 */
QPoint ServeurCrawler::DonnerPositionUnique()
{
    QRandomGenerator gen;
    QPoint pt;
    gen.seed(QDateTime::currentMSecsSinceEpoch());
    int ligne;
    int colonne;
    do
    {
        ligne = gen.bounded(TAILLE);
        QThread::usleep(20000);// attendre 20ms
        colonne = gen.bounded(TAILLE);
        QThread::usleep(20000);// attendre 20ms
        pt = QPoint(colonne,ligne);
    }while (listePositions.contains(pt));
    return pt;
}/**
 * @brief ServeurCrawler::CalculerDistance
 * @param pos
 * @return
 * Calculer distance du point et du trésor
 */
double ServeurCrawler::CalculerDistance(QPoint pos)
{
    double distance;
    int xVecteur = tresor.x()-pos.x();
    int yVecteur = tresor.y()-pos.y();
    distance = sqrt((xVecteur*xVecteur + yVecteur*yVecteur));
    return distance;
}


/**
 * @brief ServeurCrawler::on_pushButtonLancementServeur_clicked
 * lance le serveur
 */

void ServeurCrawler::on_pushButtonLancementServeur_clicked()
{
    if(!socketEcoute->listen(QHostAddress::Any,ui->spinBoxPortServeur->value())){

        qDebug() << "socket déjà écouté";
        QMessageBox message;
        message.setText("Erreur , la socket est déjà écouté");
        message.exec();
        this->close();
    }
    else {
        qDebug() << "le serveur est lance";
    }

}
