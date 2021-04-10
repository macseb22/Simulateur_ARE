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
#include <QTextEdit>


class AREsimulationWindow : public QFrame
{
    Q_OBJECT

public:
    AREsimulationWindow(QFrame* parent = nullptr);
    ~AREsimulationWindow();

public slots:
    void inputTextValidation();

private:
    void miseEnPage();


    double val_CA_Brut;
    double val_prestation;
    double val_vente;
    double val_allocationJournaliere;
    double val_abbatementPresta;
    double val_abbatementVente;
    QLabel lblTitreAppli;
    QLabel lblCA_Brut;
    QLabel lblPrestation;
    QLabel lblVente;
    QLabel lblAllocJournaliere;
    QLabel lblAbbat50;
    QLabel lblAbbat71;
    QLabel lblAbbat50Result;
    QLabel lblAbbat71Result;


    QTextEdit CA_Brut;
    QTextEdit Prestation;
    QTextEdit Vente;
    QTextEdit AJ;



    QPushButton* buttonValidation;
    QTabWidget* resultTabWindow;
    QWidget* ongletARE;
    QWidget* ongletJoursIndemnises;
    QGridLayout* formulaire;
    QHBoxLayout* layoutTitle;
    QVBoxLayout* mainLayout;

};

#endif // AREsimulationWindow_H
