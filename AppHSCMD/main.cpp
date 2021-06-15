#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("Приложение для выполнения избирательной односторонней синхронизации файлов и каталогов");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption createPage = {{"c", "create"},
                                     QCoreApplication::translate("main", "Создает новую вкладку синхронизации с указанным именем, которое используется для дальнейшего обращения к ней."), "Имя вкладки"};
    QCommandLineOption removePage = {{"r", "remove"},
                                     QCoreApplication::translate("main", "Удаляет существующую вкладку синхронизации."), "Имя вкладки"};
    QCommandLineOption setSrcDir({"s", "src"},
                                    QCoreApplication::translate("main", "Устанавливает ведущий каталог на указанной вкладке синхронизации"), "Имя страницы, Путь до ведущего каталога");
    QCommandLineOption setDstDir({"d", "dst"},
                                    QCoreApplication::translate("main", "Устанавливает ведомый каталог на указанной вкладке синхронизации"), "Имя страницы, Путь до ведомого каталога");
    QCommandLineOption savePages("S",
                                    QCoreApplication::translate("main", "Сохраняет все вкладки синхронизаций в указанный файл"),
                                    QCoreApplication::translate("main", "Путь до файла сохранения всех вкладок синхронизаций"));
    QCommandLineOption loadPages("L",
                                    QCoreApplication::translate("main", "Загружает вкладки синхронизации из указанного файла"),
                                    QCoreApplication::translate("main", "Путь до файла сохранения всех вкладок синхронизаций"));
    QCommandLineOption setLocalFilter({"f", "filter"},
                                    QCoreApplication::translate("main", "Настраивает фильтр для выбранной вкладки синхронизации"),
                                    QCoreApplication::translate("main", "Имя вкладки, Строка с именами игнорируемых файлов, разделенных точкой с запятой"));
    QCommandLineOption setGlobalFilter("filterall",
                                    QCoreApplication::translate("main", "Настраивает фильтр для всех вкладок"),
                                    QCoreApplication::translate("main", "Строка с именами игнорируемых файлов, разделенных точкой с запятой"));
    QCommandLineOption startSync("start",
                                    QCoreApplication::translate("main", "Запускает синхронизацию вкладки"),
                                    QCoreApplication::translate("main", "Имя вкладки"));
    QCommandLineOption startCheck("check",
                                    QCoreApplication::translate("main", "Запускает проверку файлов вкладки"),
                                    QCoreApplication::translate("main", "Имя вкладки"));
    QCommandLineOption startAll("startall",
                                    QCoreApplication::translate("main", "Запускает синхронизацию всех вкладок"));
    QCommandLineOption showList("startall",
                                    QCoreApplication::translate("main", "Выводит список существующих вкладок"));
    QCommandLineOption removeAll("removeall",
                                    QCoreApplication::translate("main", "Удаляет все существующие вкладки"));

    parser.addOption(createPage);
    parser.addOption(removePage);
    parser.addOption(setSrcDir);
    parser.addOption(setDstDir);
//    parser.addOption(savePages);
//    parser.addOption(loadPages);
//    parser.addOption(savePages);
//    parser.addOption(setLocalFilter);
//    parser.addOption(setGlobalFilter);
//    parser.addOption(startSync);
//    parser.addOption(startCheck);
//    parser.addOption(startAll);
//    parser.addOption(showList);
//    parser.addOption(removeAll);
    parser.process(a);

    if (parser.value(createPage) != "")
    {
        QFile pageFile(parser.value(createPage) + ".json");

        if (QFile::exists(parser.value(createPage) + ".json"))
        {
            qDebug() << QString("Вкладка с именем [%1] уже существует!").arg(parser.value(createPage));
        }

        if (!pageFile.open((QIODevice::WriteOnly)))
        {
            return -1;
        }
        pageFile.close();
    }

    qDebug() << "[setSrcDir] " << parser.values(setSrcDir);
    qDebug() << parser.value(createPage);
    qDebug() << parser.value(removePage);


    return 0;
}
