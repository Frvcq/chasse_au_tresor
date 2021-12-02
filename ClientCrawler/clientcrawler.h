#ifndef CLIENTCRAWLER_H
#define CLIENTCRAWLER_H

/**
  @author : Farvacque Flavian
  CLient qui permet de jouer à la chasse au trésor avec un serveur adéquat
  **/
#include <QWidget>
#include <QGridLayout>

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QBuffer>
#include <QToolButton>

#define TAILLE 20
QT_BEGIN_NAMESPACE
namespace Ui { class ClientCrawler; }
QT_END_NAMESPACE

class ClientCrawler : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief ClientCrawler
     * @param parent
     * COnstructeur de la classe
     */
    ClientCrawler(QWidget *parent = nullptr);
    /**
      Destructeur de la classe
      **/
    ~ClientCrawler();

private:
    Ui::ClientCrawler *ui;
    QGridLayout *grille;
    QTcpSocket *socketClient;
    QPoint point;

private slots:
    void onQtcpSocket_connected();
    void onQtcpSocket_disconnected();
    void onQtcpSocket_error(QAbstractSocket::SocketError socketError);
    void onQtcpSocket_readyRead();
    void on_pushButtonConnexion_clicked();
    void on_pushButtonUp_clicked();
    void on_pushButtonLeft_clicked();
    void on_pushButtonRight_clicked();
    void on_pushButtonDown_clicked();
};
#endif // CLIENTCRAWLER_H
