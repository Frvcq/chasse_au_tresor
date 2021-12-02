#ifndef SERVEURCRAWLER_H
#define SERVEURCRAWLER_H
/**
    @author : Farvacque Flavian
  Serveur qui permet de jouer à la chasse au trésor avec un client adéquat
  **/
#include <QWidget>
#include <QTcpServer>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QDateTime>
#include <QThread>
#include <QTcpSocket>
#include <QProcess>
#include <QHostInfo>
#include <QBuffer>
#include <QMessageBox>
#include <QToolButton>
#define TAILLE 20
QT_BEGIN_NAMESPACE
namespace Ui { class ServeurCrawler; }
QT_END_NAMESPACE

class ServeurCrawler : public QWidget
{
    Q_OBJECT

public:
    ServeurCrawler(QWidget *parent = nullptr);
    ~ServeurCrawler();

    void EnvoyerDonnees(QTcpSocket * client, QPoint pt, QString msg);
    void AfficherGrille();
    void ViderGrille();
    QPoint DonnerPositionUnique();
    double CalculerDistance(QPoint pos);
private slots:
    void onQTcpSocket_readyRead();
    void onQTcpServer_newConnection();
    void onQTcpSocket_disconnected();


    void on_pushButtonLancementServeur_clicked();

private:
    Ui::ServeurCrawler *ui;
    QTcpServer *socketEcoute;
    QList<QTcpSocket*> listeSocketClient;
    QList<QPoint> listePositions;
    QPoint tresor;
    QGridLayout *grille;
    QPushButton *touches[TAILLE];
};
#endif // SERVEURCRAWLER_H
