#include "AREsimulationWindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QRegExp>
#include <QTextEdit>
#include <QDebug>


AREsimulationWindow::AREsimulationWindow(QFrame *parent) : QFrame(parent),val_CA_Brut(0),val_prestation(0),val_vente(0),val_allocationJournaliere(0),val_abbatementPresta(0),val_abbatementVente(0)
{
    setGeometry(700,150,600,600);
    setWindowFlag(Qt::FramelessWindowHint);
    setFrameShape(QFrame::WinPanel);

    //fonction qui prépare la mise en page de l'application
    miseEnPage();


    connect(&CA_Brut,&QTextEdit::textChanged,this,&AREsimulationWindow::inputTextValidation);


}

AREsimulationWindow::~AREsimulationWindow()
{
    delete layoutTitle;
    delete formulaire;
    delete mainLayout;
}

void AREsimulationWindow::miseEnPage()
{
    //Titre de l'appli
    lblTitreAppli.setText("<h1 style='color:rgb(156,26,49);'>Simulateur:<br/><i>L'allocation d'aide à la reprise d'un emploi (ARE)</i></h1>");
    lblTitreAppli.setTextFormat(Qt::RichText);
    lblTitreAppli.setAlignment(Qt::AlignCenter);

    //Mise en forme du tire
    layoutTitle = new QHBoxLayout;
    layoutTitle->addWidget(&lblTitreAppli);
    layoutTitle->setAlignment(Qt::AlignCenter);

    //Déclaration des labels du formulaire
    lblCA_Brut.setText("Chiffre d'affaire Brut:");
    lblPrestation.setText("dont Prestation:");
    lblVente.setText("dont Vente:");
    lblAllocJournaliere.setText("Allocation Journalière:");
    lblAbbat50.setText("Abbattement 50%");
    lblAbbat50Result.setText("");
    lblAbbat71.setText("Abbattement 71%");
    lblAbbat71Result.setText("");

    //Mise en forme des QTextEdit
    CA_Brut.setFixedSize(QSize(80,25));
    //CA_Brut.set
    Prestation.setFixedSize(QSize(80,25));
    Vente.setFixedSize(QSize(80,25));
    AJ.setFixedSize(QSize(80,25));

    //Création du Formulaire des données du chiffre d'affaire à déclarer
    formulaire = new QGridLayout;
    formulaire->setVerticalSpacing(0);
    formulaire->addWidget(&lblCA_Brut,0,0,Qt::AlignRight);
    formulaire->addWidget(&lblPrestation,1,0,Qt::AlignRight);
    formulaire->addWidget(&lblVente,2,0,Qt::AlignRight);


    formulaire->addWidget(&CA_Brut,0,1,Qt::AlignLeft);
    formulaire->addWidget(&Prestation,1,1,Qt::AlignLeft);
    formulaire->addWidget(&Vente,2,1,Qt::AlignLeft);

    formulaire->addWidget(&lblAllocJournaliere,0,2,Qt::AlignRight);
    formulaire->addWidget(&lblAbbat50,1,2,Qt::AlignRight);
    formulaire->addWidget(&lblAbbat71,2,2,Qt::AlignRight);

    formulaire->addWidget(&AJ,0,3,Qt::AlignLeft);
    formulaire->addWidget(&lblAbbat50Result,1,3,Qt::AlignLeft);
    formulaire->addWidget(&lblAbbat71Result,2,3,Qt::AlignLeft);

    //Création du widget d'onglet
    resultTabWindow = new QTabWidget;

    ongletARE = new QWidget;
    ongletJoursIndemnises = new QWidget;

    resultTabWindow->addTab(ongletARE,"Montant de l'ARE");
    resultTabWindow->addTab(ongletJoursIndemnises,"Jours indémnisables");
    resultTabWindow->setMinimumHeight(300);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutTitle);
    mainLayout->addLayout(formulaire);
    mainLayout->addWidget(resultTabWindow);
    //mainLayout->addSpacerItem(new QSpacerItem(0,350));
    setLayout(mainLayout);

}


void AREsimulationWindow::inputTextValidation()
{
    QRegExp rex1("\\D");
    QRegExp rex2("^\\d+,\\d?");

    QString lastInput;
    QString input;

    input = CA_Brut.toPlainText();
    lastInput = input.right(1);

    if(rex1.exactMatch(lastInput) and lastInput != "")
    {
        if(!rex2.exactMatch(input))
        {
            CA_Brut.setText(input.left(input.length()-1));
            CA_Brut.moveCursor(QTextCursor::End);
        }


    }
}
