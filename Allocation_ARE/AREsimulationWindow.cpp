#include "AREsimulationWindow.h"
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QDebug>
#include <QVector>
#include <QDate>
#include <QCalendar>
#include <QFontDatabase>
#include <QDoubleValidator>
#include <QSignalMapper>



AREsimulationWindow::AREsimulationWindow(QFrame *parent) : QFrame(parent),val_CA_Brut(0),val_prestation(0),val_vente(0),val_allocationJournaliere(0),val_abbatementPresta(0),val_abbatementVente(0)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setFrameShape(QFrame::WinPanel);
    setObjectName("mainWindow");


    //fonction qui prépare la mise en page initial de l'application
    miseEnPage();

    //Connexion du formulaire vers le slot d'auto completion des données
    connect(&Prestation,&QLineEdit::editingFinished,this,&AREsimulationWindow::autoCompletion);
    connect(&Vente,&QLineEdit::editingFinished,this,&AREsimulationWindow::autoCompletion);


    //Réglage de l'ordre de tabulation des QLineEdit
    this->setTabOrder(&CA_Brut,&Prestation);
    this->setTabOrder(&Prestation,&Vente);
    this->setTabOrder(&Vente,&AJ);
    this->setTabOrder(&AJ,buttonValidation);

    //Action du bouton Quitter
    connect(buttonQuit,&QPushButton::clicked,qApp,&QApplication::quit);

    //Action des bouton simuler et reset
    connect(buttonValidation,&QPushButton::clicked,this,&AREsimulationWindow::affichageResultat);
    connect(buttonReset,&QPushButton::clicked,this,&AREsimulationWindow::resetInputValue);

    //Lors d'un input dans les QTextBox on vérifie les données
    connect(&CA_Brut,&QLineEdit::textEdited,this,&AREsimulationWindow::inputTextValidation);
    connect(&Prestation,&QLineEdit::textEdited,this,&AREsimulationWindow::inputTextValidation);
    connect(&Vente,&QLineEdit::textEdited,this,&AREsimulationWindow::inputTextValidation);
    connect(&AJ,&QLineEdit::textEdited,this,&AREsimulationWindow::inputTextValidation);


}

AREsimulationWindow::~AREsimulationWindow()
{
    delete layoutTitle;
    delete formulaire;
    delete mainLayout;
}

void AREsimulationWindow::miseEnPage()
{
    //Label du titre de l'appli
    lblTitreAppli.setText("<h1 style='color:rgb(156,26,49);'>Simulateur:<br/><i>L'allocation d'aide à la reprise d'un emploi (ARE)</i></h1>");
    lblTitreAppli.setTextFormat(Qt::RichText);
    lblTitreAppli.setAlignment(Qt::AlignCenter);

    buttonQuit = new QPushButton(this);
    buttonQuit->setGeometry(770,20,32,32);
    buttonQuit->setStyleSheet("background: url(':/icones/Ressources/Icones/ico_quit.png') no-repeat; ");
    buttonQuit->setToolTip("Quitter l'application");
    buttonQuit->setCursor(Qt::PointingHandCursor);


    //Mise en forme du titre de l'application
    layoutTitle = new QHBoxLayout;
    layoutTitle->addWidget(&lblTitreAppli);
    layoutTitle->setAlignment(Qt::AlignCenter);
    layoutTitle->setContentsMargins(0,10,0,50);

    //Déclaration des labels du formulaire
    lblCA_Brut.setText("Chiffre d'affaire Brut:");
    lblCA_Brut.setStyleSheet("font:15px; font-weight: bold;");

    lblPrestation.setText("dont Prestation:");
    lblPrestation.setStyleSheet("font:13px; font-weight: bold;");

    lblVente.setText("dont Vente:");
    lblVente.setStyleSheet("font:13px; font-weight: bold;");

    lblAllocJournaliere.setText("Allocation Journalière:");
    lblAllocJournaliere.setStyleSheet("font:15px; font-weight: bold;");

    //Mise en forme et parmamétrage des QLineEdit
    CA_Brut.setObjectName("CA_Brut");
    CA_Brut.setFixedSize(QSize(80,25));
    CA_Brut.setText("0");
    CA_Brut.selectAll();    
    CA_Brut.setValidator(new QDoubleValidator(0,100000,2)); //Créer une validation de type de donnée pour CA_Brut

    Prestation.setObjectName("Prestation");
    Prestation.setFixedSize(QSize(80,25));
    Prestation.setText("0");
    Prestation.setValidator(new QDoubleValidator(0,100000,2)); //Créer une validation de type de donnée pour Prestation

    Vente.setObjectName("Vente");
    Vente.setFixedSize(QSize(80,25));
    Vente.setText("0");
    Vente.setValidator(new QDoubleValidator(0,100000,2)); //Créer une validation de type de donnée pour Vente

    AJ.setObjectName("lineEdit");
    AJ.setFixedSize(QSize(80,25));
    AJ.setText("29,32");
    AJ.setValidator(new QDoubleValidator(0,300,2)); //Créer une validation de type de donnée pour AJ


    //Bouton simuler
    buttonValidation = new QPushButton("Simuler");
    buttonValidation->setStyleSheet("QPushButton { background:gold; border: 2px outset yellow; } :hover { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 yellow, stop: 1 white); } ");
    buttonValidation->setCursor(Qt::PointingHandCursor);

    //Bouton Reset
    buttonReset = new QPushButton("Reset");
    buttonReset->setStyleSheet("QPushButton { background:purple; border: 2px outset purple } :hover { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 purple, stop: 1 white); } ");
    buttonReset->setCursor(Qt::PointingHandCursor);

    //Message en cas d'erreur
    lblErrorMessage.setStyleSheet("color: red; background:white; font:15px; ");
    lblErrorMessage.setVisible(false);
    lblErrorMessage.setTextFormat(Qt::RichText);


    //Création du Formulaire des données du chiffre d'affaire à déclarer
    formulaire = new QGridLayout;
    formulaire->setVerticalSpacing(0);
    formulaire->setColumnStretch(1,1);
    formulaire->setColumnStretch(2,0);
    formulaire->setColumnStretch(3,2);
    formulaire->setContentsMargins(60,0,0,30);


    formulaire->addWidget(&lblCA_Brut,0,0,Qt::AlignRight);
    formulaire->addWidget(&lblPrestation,1,0,Qt::AlignRight);
    formulaire->addWidget(&lblVente,2,0,Qt::AlignRight);

    formulaire->addWidget(&CA_Brut,0,1,Qt::AlignLeft);
    formulaire->addWidget(&Prestation,1,1,Qt::AlignLeft);
    formulaire->addWidget(&Vente,2,1,Qt::AlignLeft);

    formulaire->addWidget(&lblAllocJournaliere,0,2,Qt::AlignLeft);
    formulaire->addWidget(buttonValidation,2,2,1,1,Qt::AlignRight);
    formulaire->addWidget(buttonReset,2,3,1,1,Qt::AlignLeft);

    formulaire->addWidget(&AJ,0,3,Qt::AlignLeft);
    formulaire->addWidget(&lblErrorMessage,4,1,2,3,Qt::AlignLeft);



    //Widget conteneur d'onglets affichant les résultats
    resultTabWindow = new QTabWidget;

    //QPixmap contien l'image du logo pole emploi
    logoPoleEmploi.load(":/images/Ressources/logo_poleEmploi.jpg");

    //Préparation 1er onglet: Affiche le resultat du montant de l'ARE
    ongletARE = new QWidget;   
    ongletARE->setStyleSheet("background: #5a6868; padding:0px;");

    //Logo afficher en attente d'un resultat
    lblLogoOnletARE.setPixmap(logoPoleEmploi);

    layoutOngletARE = new QVBoxLayout(ongletARE);
    layoutOngletARE->addWidget(&lblLogoOnletARE);

    //Préparation 2e onglet: Affiche le resultat du nombre de jours indemnisés
    ongletJoursIndemnises = new QWidget;
    ongletJoursIndemnises->setStyleSheet("background: #5a6868;");

    //Logo afficher en attente d'un resultat
    lblLogoOngletJoursIndemnises.setPixmap(logoPoleEmploi);

    layoutOngletJoursIndemnises = new QVBoxLayout(ongletJoursIndemnises);
    layoutOngletJoursIndemnises->addWidget(&lblLogoOngletJoursIndemnises);

    resultTabWindow->addTab(ongletARE,"Montant de l'ARE");
    resultTabWindow->addTab(ongletJoursIndemnises,"Jours indémnisables");
    //resultTabWindow->setMinimumHeight(300);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutTitle);
    mainLayout->addLayout(formulaire);
    mainLayout->addWidget(resultTabWindow);


    setStyleSheet("#mainWindow { background: url(:/images/Ressources/background.png); }"
                  "#lineEdit { font:15px; }"
                  "QPushButton { color:black; font-weight:bold; width: 50px; padding:12px; border-radius:10px;  }");
    setLayout(mainLayout);

}












/******************************************************************
 *
 *          ----             ----   -------    ----
 *         |        |       |    |     |      |
 *          ----    |       |    |     |       ----
 *              |   |       |    |     |           |
 *          ----     -----   ----      .       ----
 *
 ******************************************************************/



void AREsimulationWindow::inputTextValidation()
{
    /****************************************************************************************************/
                        //Vérification de l'acceptabilité des données dans les QLineEdit
                        //en cas de défaut: on efface le dernier caractère saisie
    /****************************************************************************************************/
    QString textEdited;
    int lengthText;

    //Si un caractère n'est pas acceptable (autre qu'un double ex: différent de 152.23) on efface le dernier caractere saisi
    if(!CA_Brut.hasAcceptableInput())
    {
       textEdited = CA_Brut.text(); //Texte saisi dans CA_Brut
       lengthText = textEdited.length()-1; //On récupère la longueur du texte - 1

       CA_Brut.setText(textEdited.left(lengthText)); //On réedite le texte afin de supprimer le dernier caractère inacceptable par QDoubleValidator
    }
    else if(!Prestation.hasAcceptableInput())
    {
        textEdited = Prestation.text();
        lengthText = textEdited.length()-1;

        Prestation.setText(textEdited.left(lengthText));
    }
    else if(!Vente.hasAcceptableInput())
    {
        textEdited = Vente.text();
        lengthText = textEdited.length()-1;

        Vente.setText(textEdited.left(lengthText));
    }
    else if(!AJ.hasAcceptableInput())
    {
        textEdited = AJ.text();
        lengthText = textEdited.length()-1;

        AJ.setText(textEdited.left(lengthText));
    }

}

void AREsimulationWindow::autoCompletion()
{
    /***************************************************************************************************************************/
    /************   A l'actualisation des données de Prestation ou de Vente on auto complète les autres cases.  ****************/
    /***************************************************************************************************************************/

    QLocale french(QLocale::French); //Permet d'utiliser la notation standard française des nombres à virgule ex: 150,26 et pas 150.26

    double resultat;

    //Si le signal issu des QLineEdit vient de Prestation et CA Brut >= à préstation on auto complete la case Vente
    if(sender()->objectName() == "Prestation" and (french.toDouble(CA_Brut.text()) >= french.toDouble(Prestation.text())))
    {
        resultat = french.toDouble(CA_Brut.text()) - french.toDouble(Prestation.text());
        Vente.setText(french.toString(resultat));
    }

    //Sinon si le signal issu des QLineEdit vient de Vente et CA Brut >= à Vente on auto complete la case Prestation
    else if(sender()->objectName() == "Vente" and (french.toDouble(CA_Brut.text()) >= french.toDouble(Vente.text())))
    {
        resultat = french.toDouble(CA_Brut.text()) - french.toDouble(Vente.text());
        Prestation.setText(french.toString(resultat));
    }

    //Sinon cela signifie que CA brut est inférieur à la valeur de Vente ou Presatation donc on auto complete la case CA_Brut
    else
    {
        resultat = french.toDouble(Prestation.text()) + french.toDouble(Vente.text());
        CA_Brut.setText(french.toString(resultat));
    }
}

int AREsimulationWindow::calcul_CA_Net()
{
    QLocale french(QLocale::French);

    //Récupère les données de l'utilisateur:
    val_CA_Brut = french.toDouble(CA_Brut.text());
    val_prestation = french.toDouble(Prestation.text());
    val_vente = french.toDouble(Vente.text());


    //Calcul des sommes après abattement: (50% sur les prestations et 71% sur les ventes)
    val_abbatementPresta = val_prestation*0.5;
    val_abbatementVente = val_vente*0.71;

    return val_CA_net = val_abbatementPresta + val_abbatementVente;

}

int AREsimulationWindow::calcul_Allocation()
{
    //Utilise un QLocale pour manipuler les nombres au format francais (ex: 159,56 au lieu de 159.56)
    QLocale french(QLocale::French);

    //Récupère les données de l'utilisateur:
    val_allocationJournaliere = french.toDouble(AJ.text());


    //Calcul de l'allocation Brut et net
    int nbJourDuMois = QDate::currentDate().daysInMonth();

    val_Allocation_Brut = val_allocationJournaliere * nbJourDuMois;
    val_Allocation_Net = val_Allocation_Brut - (0.7 * val_CA_net);

    if(val_Allocation_Net < 0) { val_Allocation_Net = 0; }

    return val_Allocation_Net;
}

void AREsimulationWindow::affichageResultat()
{

    if(!validUserData())
    {
        lblAREResult.setVisible(false);
        lblLogoOnletARE.show();
        lblLogoOngletJoursIndemnises.show();
        return;
    }

    lblAREResult.setVisible(true);
    lblLogoOnletARE.close();
    lblLogoOngletJoursIndemnises.close();

    //On récupère les données calculées:
    double CA_Net = calcul_CA_Net();
    double Alloc_Net = calcul_Allocation();


    QString Allocation_NetToString;
    QString CA_NetToString;

    //On convertit les valeurs double en string
    Allocation_NetToString = Allocation_NetToString.setNum(Alloc_Net);
    CA_NetToString = CA_NetToString.setNum(CA_Net);

    //Chargement d'un style de police d'écriture pour l'écriture du résultat
    QFontDatabase::addApplicationFont(":/fonts/Ressources/Fonts/Harsey DEMO.ttf");

    lblAREResult.setFont(QFont("Arial Black",12));


    QString resultat;


    //x1 et x2 contiennent le détail numerique des calculs à afficher dans le label resultat
    QString x1 = QString::number(val_allocationJournaliere * QDate::currentDate().daysInMonth());
    QString x2 = QString::number(val_CA_net);

    resultat =
            "<h1 style='color:rgb(156,26,49);  text-align:center;'>Resultat de la simulation:</h1>"
            "<div style='font: 13px; color:DodgerBlue;'>"
                "<div style='color:white; font-style:italic;'><h3 style='margin:0 0 0 0;'>Détails calcul:</h3>"
                    "<p style='margin-top:0;'><em style='color:#49f57f;'>Allocation due</em> = <em style='color:deepblue;'>Allocation mensuelle</em> - (<em style='color:purple;'>Chiffre d'affaire net</em>  X 70%)<br/>"
                    "<span style='color:deepblue;'>========></span><em style='color:deepblue;'> Allocation mensuelle</em> = <em style='color:deepblue;'>Allocation journalière</em> X <em style='color:deepblue;'>nb de jour dans le mois</em><br/>"
                    "<span style='color:purple;'>========></span><em style='color:purple;'> Chiffre d'affaire net</em> = (<em style='color:purple;'>somme des prestations x 50%</em>) + (<em style='color:purple;'>somme des Ventes x 71%</em>)"
                    "</p>"

                    "<p><em style='color:#49f57f;'>Allocation due</em> = (<span style='color:deepblue;'>" + AJ.text() + "€ X " + QString::number(QDate::currentDate().daysInMonth())  + "j </span>) - (<span style='color:purple;'>" + Prestation.text() + "€ X 0,5 + " + Vente.text() + "€ X 0,71 </span>) X 0,7<br/>"
                    "<em style='color:#49f57f;'>Allocation due</em> = <span style='color:deepblue;'>" + x1 + "</span> - (<span style='color:purple;'>" + x2 + "</span> X 0,7)<br/>"
                    "<em style='color:#49f57f;'>Allocation due</em> = <em style='color:#49f57f;'>"+ Allocation_NetToString + "€</em>"
                    "</p>"
                "</div>"
                "<div style='text-align:center; color:rgb(156,26,49); font:30px;'>"
                     "<p style='font:20px;'>----------------------------------------------------------</p>"
                     "<p><strong>Somme à déclarer: "+ CA_NetToString + "€</strong><br/>"
                     "<strong style='color:#49f57f;'>Montant de votre allocation: " + Allocation_NetToString + "€</strong>"
                     "</p>"
                "</div>"
            "</div>";

    lblAREResult.setText(resultat);
    lblAREResult.setStyleSheet("background: grey; padding:10px;");
    layoutOngletARE->addWidget(&lblAREResult,0,Qt::AlignTop);

}

bool AREsimulationWindow::validUserData()
{
    QLocale french(QLocale::French);

    double sommeA_B, A, B;
    sommeA_B = french.toDouble(CA_Brut.text());
    A = french.toDouble(Prestation.text());
    B = french.toDouble(Vente.text());

    //Vérification que les champs ne soient pas vide
    if(CA_Brut.text() == ""  || Prestation.text() == "" || Vente.text() == "" || AJ.text() == "")
    {
        lblErrorMessage.setVisible(true);
        lblErrorMessage.setText("Attention: Remplissez toutes les cases du formulaire!");
        return false;
    }
    //Vérification que la somme Prestation + Vente = Chiffre d'affaire
    else if(sommeA_B != A + B)
    {
        lblErrorMessage.setVisible(true);
        lblErrorMessage.setText("Attention: Le CA brut est différent de la somme</br> \"Prestations\" + \"Ventes\"");
        return false;
    }

    lblErrorMessage.setVisible(false);
    return true;
}


void AREsimulationWindow::resetInputValue()
{
    /*********************************************************************************************************************/
    /******     L'appui sur reset réinitialise l'affichage de la fenetre résultat et les données du formulaire      ******/
    /*********************************************************************************************************************/


    //Réinitialise l'affichage
    lblAREResult.setVisible(false);
    lblLogoOnletARE.show();
    lblLogoOngletJoursIndemnises.show();

    //Réinitialise le formulaire
    CA_Brut.setText("0");
    Prestation.setText("0");
    Vente.setText("0");
    CA_Brut.setFocus();
    CA_Brut.selectAll();
}

