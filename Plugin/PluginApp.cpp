#include "PluginApp.h"
#include "Global/Global.h"
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QSharedPointer>

CPluginApp::CPluginApp()
{  
}

CPluginApp::~CPluginApp()
{
    LOG_MODEL_DEBUG("CPluginApp", "CPluginApp::~CPluginApp");
}

int CPluginApp::About()
{
    QMessageBox::information(NULL, QObject::tr("Application Infomation"),
                             QObject::tr("Application:") + Name());
    return 0;
}

QIcon CPluginApp::Icon()
{
    return QIcon(":/icon/AppIcon");
}

int CPluginApp::InitInstance(const QString &szDir)
{
    int nRet = 0;
    nRet = LoadTranslate(szDir);
    return nRet;
}

int CPluginApp::LoadTranslate(const QString &szDir)
{
    //初始化翻译  
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", QLocale::system().name()).toString();

    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }

    LOG_MODEL_DEBUG("CPluginApp", "locale language:%s", szLocale.toStdString().c_str());

    if(!m_TranslatorPlugin.isNull())
    {
        qApp->removeTranslator(m_TranslatorPlugin.data());
        m_TranslatorPlugin.clear();
    }

    LOG_MODEL_DEBUG("CPluginApp", "Translate dir:%s", qPrintable(szDir));

    m_TranslatorPlugin = QSharedPointer<QTranslator>(new QTranslator());
   
#ifdef ANDROID
    m_TranslatorPlugin->load(":/translations/Plugin_" + szLocale + ".qm");
#else
    QString szPlugin = szDir + QDir::separator() + "translations"
            + QDir::separator() + "Plugin_" + szLocale + ".qm";
    bool bRet = m_TranslatorPlugin->load(szPlugin);
    if(!bRet)
        LOG_MODEL_ERROR("CPluginApp", "load translator[%s] fail",
                        szPlugin.toStdString().c_str());

#endif
    qApp->installTranslator(m_TranslatorPlugin.data());

    return 0;
}
