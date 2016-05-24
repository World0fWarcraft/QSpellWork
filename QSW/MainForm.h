#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QSettings>
#include <QFutureWatcher>
#include <QMainWindow>
#include <QToolButton>
#include <QIcon>
#include <QStringListModel>

#include "SWEnums.h"
#include "SWObject.h"

#include "ui_main.h"
#include "ui_advancedFilter.h"

class SWObject;
class SpellListSortedModel;

class Enums : public QObject
{
    Q_OBJECT
public:

    enum LocalesDBC
    {
        enUS,
        koKR,
        frFR,
        deDE,
        zhCN,
        zhTW,
        esES,
        esMX
    };

    Q_ENUMS(LocalesDBC)
};

class AdvancedFilterWidget : public QWidget, public Ui::advancedFilter
{
    Q_OBJECT

    public:
        AdvancedFilterWidget(QWidget* parent = NULL);
        ~AdvancedFilterWidget() {}

        void addBookmark(QString str) {
            str =  str.trimmed();
            if (!m_bookmarks.contains(str))
                m_bookmarks << str;
            m_model->setStringList(m_bookmarks);
        }

        void removeBookmark(int index) {
            if (index < m_bookmarks.size())
                m_bookmarks.removeAt(index);
            m_model->setStringList(m_bookmarks);
        }

        void clearBookmarks() {
            m_bookmarks.clear();
            m_model->setStringList(m_bookmarks);
        }

        QStringList getBookmarks() const { return m_bookmarks; }

    public slots:
        void slotAdd();
        void slotRemove();
        void slotClear();

    private:
        QStringList m_bookmarks;
        QStringListModel* m_model;
};

class MainForm : public QMainWindow, public Ui::main
{
    Q_OBJECT

    public:
        MainForm(QWidget* parent = NULL);
        ~MainForm();

        void saveSettings();
        void loadSettings();

        bool isRegExp() const { return m_actionRegExp->isChecked(); }
        void setRegExp(bool enable) { m_actionRegExp->setChecked(enable); }
        SWEnums* getEnums() const { return m_enums; }
        QString getFilterText() const { return m_advancedFilterWidget->textEdit->toPlainText(); }
        void setFilterText(QString str) { m_advancedFilterWidget->textEdit->setText(str); }

        QWebEngineView* getBrowser(quint8 num) const
        {
            switch (num)
            {
                case 1: return webView1;
                case 2: return webView2;
                case 3: return webView3;
                default: return webView1;
            }
        }

    signals:
        void signalSearch(quint8 type);

    private slots:
        void slotAbout();
        void slotAdvancedFilter();
        void slotAdvancedApply();
        void slotFilterSearch();
        void slotButtonSearch();
        void slotCompareSearch();
        void slotSearch(quint8 type);
        void slotSearchResult();
        void slotSearchFromList(const QModelIndex &index);
        void slotLinkClicked(const QUrl &url);
        void slotRegExp();
        void slotWov();
        void slotSettings();
        void slotModeShow();
        void slotModeCompare();
        void slotPrevRow();
        void slotNextRow();
        void slotCopyAll();

        bool event(QEvent* ev);

    private:
        void loadComboBoxes();
        void detectLocale();
        void createModeButton();
        void initializeCompleter();

        SWEnums* m_enums;

        SpellListSortedModel* m_sortedModel;
        Ui::main m_ui;
        SWObject* m_sw;
        QToolButton* m_modeButton;
        QAction* m_actionRegExp;
        QAction* m_actionAbout;
        QAction* m_actionSettings;
        QAction* m_actionWov;

        typedef QList<QEvent*> EventList;
        typedef QFutureWatcher<EventList> SearchResultWatcher;
        SearchResultWatcher* m_watcher;

        AdvancedFilterWidget* m_advancedFilterWidget;
};

class QSWPage : public QWebEnginePage
{
    Q_OBJECT
    public:
        QSWPage(QObject* parent = 0) : QWebEnginePage(parent){}

        bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool)
        {
            if (type == QWebEnginePage::NavigationTypeLinkClicked)
            {
                emit linkClicked(url);
                return false;
            }
            return true;
        }

    signals:
        void linkClicked(const QUrl&);

};

#endif // MAIN_FORM_H