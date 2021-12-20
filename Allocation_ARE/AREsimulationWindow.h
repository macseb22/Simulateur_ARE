#ifndef AREsimulationWindow_H
#define AREsimulationWindow_H

#include <QTabWidget>
#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>


class AREsimulationWindow : public QFrame
{
    Q_OBJECT

public:
    AREsimulationWindow(QFrame* parent = nullptr);
    ~AREsimulationWindow();

public slots:
    void inputTextValidation();
    void affichageResultat();
    int calcul_CA_Net();
    int calcul_Allocation();
    void resetInputValue();
    void autoCompletion();

private:
    void miseEnPage();
    bool validUserData();

    //Eléments de calcul
    double val_CA_Brut;
    double val_CA_net;
    double val_Allocation_Brut;
    double val_Allocation_Net;
    double val_prestation;
    double val_vente;
    double val_allocationJournaliere;
    double val_abbatementPresta;
    double val_abbatementVente;


    //Titre de l'apli
    QLabel lblTitreAppli;
    QHBoxLayout* layoutTitle;

    //Partie formulaire
    QLabel lblCA_Brut;
    QLabel lblPrestation;
    QLabel lblVente;
    QLabel lblAllocJournaliere;
    QLabel lblErrorMessage;

    QLineEdit CA_Brut;
    QLineEdit Prestation;
    QLineEdit Vente;
    QLineEdit AJ;

    QPushButton* buttonValidation;
    QPushButton* buttonReset;
    QPushButton* buttonQuit;

    QGridLayout* formulaire;

    //Partie QTabWidget (Onglets résultat)
    QTabWidget* resultTabWindow;

    QWidget* ongletARE;
    QVBoxLayout* layoutOngletARE;

    QWidget* ongletJoursIndemnises;
    QVBoxLayout* layoutOngletJoursIndemnises;

    //Logo avant affichage des resultats
    QLabel lblLogoOnletARE;
    QLabel lblLogoOngletJoursIndemnises;
    QPixmap logoPoleEmploi;


    //Label des resultats
    QLabel lblAREResult;
    QLabel lblJoursIndemnisesResult;





    QVBoxLayout* mainLayout;

};

#endif // AREsimulationWindow_H
