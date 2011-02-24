#include "SWForm.h"
#include "AboutForm.h"

SWForm::SWForm(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    sw = new SWObject(this);

    LoadComboBoxes();
    LoadToolButtons();

    // List search connection
    connect(SpellList, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSearchFromList(QModelIndex)));

    // Main search connections
    connect(findLine_e1, SIGNAL(returnPressed()), this, SLOT(SlotButtonSearch()));
    connect(findLine_e2, SIGNAL(returnPressed()), this, SLOT(SlotButtonSearch()));
    connect(findLine_e3, SIGNAL(returnPressed()), this, SLOT(SlotButtonSearch()));
    connect(findButton, SIGNAL(clicked()), this, SLOT(SlotButtonSearch()));

    // Menu connections
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(SlotAbout()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    // RegExp connection
    connect(regexpButton, SIGNAL(clicked()), this, SLOT(SlotRegExp()));

    // Filter search connections
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotFilterSearch()));
    connect(comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotFilterSearch()));
    connect(comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotFilterSearch()));
    connect(comboBox_4, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotFilterSearch()));
    connect(comboBox_5, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotFilterSearch()));
    connect(adLine1, SIGNAL(returnPressed()), this, SLOT(SlotFilterSearch()));
    connect(adLine2, SIGNAL(returnPressed()), this, SLOT(SlotFilterSearch()));

    // Search connection
    connect(this, SIGNAL(SignalSearch(bool)), this, SLOT(SlotSearch(bool)));
}

SWForm::~SWForm()
{
}

void SWForm::LoadToolButtons()
{
    for (quint8 i = 0; i < 3; i++)
    {
        actionA[i] = new QAction(this);
        actionB[i] = new QAction(this);
    }

    actionA[0]->setText(QString("None"));
    actionA[1]->setText(QString("Contain"));
    actionA[2]->setText(QString("Not Contain"));

    actionB[0]->setText(QString("None"));
    actionB[1]->setText(QString("Contain"));
    actionB[2]->setText(QString("Not Contain"));

    for (quint8 i = 0; i < 3; i++)
    {
        actionA[i]->setCheckable(true);
        actionB[i]->setCheckable(true);
        toolButton->addAction(actionA[i]);
        toolButton_2->addAction(actionB[i]);
    }

    actionA[0]->setChecked(true);
    actionB[0]->setChecked(true);

    connect(toolButton, SIGNAL(triggered(QAction*)), this, SLOT(SlotSetCheckedA(QAction*)));
    connect(toolButton_2, SIGNAL(triggered(QAction*)), this, SLOT(SlotSetCheckedB(QAction*)));
}

void SWForm::SlotSetCheckedA(QAction *ac)
{
    for (quint8 i = 0; i < 3; i++)
        actionA[i]->setChecked(false);

    ac->setChecked(true);
}

void SWForm::SlotSetCheckedB(QAction *ac)
{
    for (quint8 i = 0; i < 3; i++)
        actionB[i]->setChecked(false);

    ac->setChecked(true);
}

void SWForm::LoadComboBoxes()
{
    comboBox->clear();
    comboBox->insertItem(-1, "SpellFamilyName");
    sw->SetMetaEnum("SpellFamilyNames");
    for (quint16 i = 0; i < sw->me.keyCount(); i++)
        comboBox->insertItem(i, QString("(%0) %1").arg(i, 3, 10, QChar('0')).arg(sw->me.valueToKey(sw->me.value(i))));

    comboBox_2->clear();
    comboBox_2->insertItem(-1, "Aura");
    sw->SetMetaEnum("AuraType");
    for (quint16 i = 0; i < sw->me.keyCount(); i++)
        comboBox_2->insertItem(i, QString("(%0) %1").arg(i, 3, 10, QChar('0')).arg(sw->me.valueToKey(sw->me.value(i))));

    comboBox_3->clear();
    comboBox_3->insertItem(-1, "Effect");
    sw->SetMetaEnum("Effects");
    for (quint16 i = 0; i < sw->me.keyCount(); i++)
        comboBox_3->insertItem(i, QString("(%0) %1").arg(i, 3, 10, QChar('0')).arg(sw->me.valueToKey(sw->me.value(i))));

    comboBox_4->clear();
    sw->SetMetaEnum("Targets");
    comboBox_4->insertItem(-1, "Target A");
    for (quint16 i = 0; i < sw->me.keyCount(); i++)
        comboBox_4->insertItem(i, QString("(%0) %1").arg(i, 3, 10, QChar('0')).arg(sw->me.key(i)));

    comboBox_5->clear();
    comboBox_5->insertItem(-1, "Target B");
    for (quint16 i = 0; i < sw->me.keyCount(); i++)
        comboBox_5->insertItem(i, QString("(%0) %1").arg(i, 3, 10, QChar('0')).arg(sw->me.key(i)));

    adBox1->clear();
    adBox2->clear();
    for (quint16 i = 0; i < MAX_STRUCT; i++)
    {
        adBox1->insertItem(i, QString("%0").arg(SpellStruct[i]));
        adBox2->insertItem(i, QString("%0").arg(SpellStruct[i]));
    }
}

void SWForm::SlotRegExp()
{
    if (!sw->IsRegExp())
    {
        sw->SetRegExp(true);
        regexpButton->setStyleSheet(QString(
            "QPushButton {"
                "border: 1px solid #000000;"
                "border-radius: 10px;"
                "background-color: qlineargradient(x1:0, y1:0, x2: 0, y2: 1, "
                "stop:0 #00FF1C, stop: 0.5 #70FC83,"
                "stop: 0.51 #00FF1C, stop: 1 #70FC83);"
                "color: #000000;"
                "font: bold 12px;"
                "min-width: 80px;"
            "}"));
    }
    else
    {
        sw->SetRegExp(false);
        regexpButton->setStyleSheet(QString(
            "QPushButton {"
                "border: 1px solid #000000;"
                "border-radius: 10px;"
	            "background-color: qlineargradient(x1:0, y1:0, x2: 0, y2: 1, "
                "stop:0 #FF0000, stop: 0.5 #FF7070,"
                "stop: 0.51 #FF0000, stop: 1 #FF7070);"
                "color: #000000;"
                "font: bold 12px;"
                "min-width: 80px;"
            "}"));
    }

    if (!SpellList->model())
        return;

    if (SpellEntry const *spellInfo = sSpellStore.LookupEntry(SpellList->model()->data(SpellList->model()->index(SpellList->currentIndex().row(), 0)).toInt()))
        sw->ShowInfo(spellInfo);
}

void SWForm::SlotAbout()
{
    AboutForm *aform = new AboutForm;
}

void SWForm::SlotButtonSearch()
{
    emit SignalSearch(false);
}

void SWForm::SlotFilterSearch()
{
    emit SignalSearch(true);
}

void SWForm::SlotSearch(bool filter)
{
    sw->SetFilter(filter);
    delete SpellList->model();
    sw->ThreadBegin(THREAD_SEARCH);
}

void SWForm::SlotSearchFromList(const QModelIndex &index)
{
    QVariant var = SpellList->model()->data(SpellList->model()->index(index.row(), 0));

    if (SpellEntry const *spellInfo = sSpellStore.LookupEntry(var.toInt()))
        sw->ShowInfo(spellInfo);
}

bool SWForm::event(QEvent *ev)
{
    switch (ev->type())
    {
        case SendModel::TypeId:
        {
            SendModel* m_ev = (SendModel*)ev;
            SpellList->setModel(m_ev->GetObject());
            return true;
        }
        break;
        case SendSpell::TypeId:
        {
            SendSpell* m_ev = (SendSpell*)ev;
            sw->ShowInfo(m_ev->GetObject());
            return true;
        }
        break;
    }

    return QWidget::event(ev);
}

quint8 SWForm::GetToolState(quint8 Tool)
{
    if (Tool == TOOL_ONE)
    {
        if (actionA[STATE_NONE]->isChecked())
            return STATE_NONE;
        else if (actionA[STATE_CONTAIN]->isChecked())
            return STATE_CONTAIN;
        else if (actionA[STATE_NOT_CONTAIN]->isChecked())
            return STATE_NOT_CONTAIN;
    }
    else if (Tool == TOOL_TWO)
    {
        if (actionB[STATE_NONE]->isChecked())
            return STATE_NONE;
        else if (actionB[STATE_CONTAIN]->isChecked())
            return STATE_CONTAIN;
        else if (actionB[STATE_NOT_CONTAIN]->isChecked())
            return STATE_NOT_CONTAIN;
    }

    return STATE_NONE;
}