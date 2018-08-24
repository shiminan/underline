#include <QTest>
#include <QQmlApplicationEngine>
#include <QtShell>
#include <underline.h>
#include "quicktests.h"

QuickTests::QuickTests(QObject *parent) : QObject(parent)
{
    auto ref = [=]() {
        QTest::qExec(this, 0, 0); // Autotest detect available test cases of a QObject by looking for "QTest::qExec" in source code
    };
    Q_UNUSED(ref);

}

void QuickTests::test_QJSValue()
{
    QCOMPARE((bool) _::Private::is_meta_object<QJSValue>::value, true);

    QJSEngine engine;
    QJSValue value = engine.toScriptValue(QVariantMap{});

    _::Private::write(value, "value1", 1);

    QCOMPARE(value.property("value1").toInt(), 1);

    QCOMPARE(_::Private::read(value, "value1").toInt(), 1);
}

void QuickTests::test_private_is_convertible()
{
    QCOMPARE( (bool) (_::Private::is_custom_convertible<QJSValue, QVariant>::value) , true);

    QQmlApplicationEngine engine;
    QString content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
    QJSValue source = engine.evaluate(content);

    QVariant dest;
    _::Private::convertTo(source, dest);

    auto map = dest.toMap();

    QCOMPARE(map["value1"].toInt(), 10);
}

void QuickTests::test_forIn()
{
    {
        QQmlApplicationEngine engine;

        QString content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
        QJSValue value = engine.evaluate(content);

        QVariantMap map;

        _::forIn(value, [&](const QJSValue& value, const QString& key) {
            map[key] = value.toVariant();
        });

        QCOMPARE(map["value1"].toInt()  , 10);
        QCOMPARE(map["value3"].toBool() , false);
        QCOMPARE(map["value4"].toMap()["value1"].toInt() , 21);
    }

}

void QuickTests::test_assign_QJSValue()
{
    {
        QQmlApplicationEngine engine;
        engine.load(QString(SRCDIR) + "/SampleData1.qml");
        QObject* root = engine.rootObjects()[0];

        QString content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
        QJSValue value = engine.evaluate(content);

        _::assign(root, value);

        QCOMPARE(root->property("value1").toInt(), 10);
        QVERIFY(root->property("value2").toString() == "11");
        QVERIFY(root->property("value3").toBool() == false);
        QCOMPARE(root->property("value4").value<QObject*>()->property("value1").toInt(), 21);
    }

    {
        QQmlApplicationEngine engine;
        QString content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
        QJSValue value = engine.evaluate(content);

        _::assign(0, value);
    }
}

void QuickTests::test_merge_QJSValue()
{
    /* Remarks: merge(QJSValue, any_type_other_then_QJSValue) is not supported.
     */

    {
        /* QVariantMap, QJSValue */
        QQmlApplicationEngine engine;

        QString content;
        content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
        QJSValue source = engine.evaluate(content);

        QVariantMap object;
        object["value4"] = QVariantMap{{"value2", 2.0}};

        _::merge(object, source);

        QCOMPARE(object["value4"].toMap()["value1"].toInt(), 21);
        QCOMPARE(object["value4"].toMap()["value2"].toDouble(), 2.0);
    }

    {
        /* QJSValue, QJSValue */
        QQmlApplicationEngine engine;

        QString content;
        content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
        QJSValue source = engine.evaluate(content);

        content = QtShell::cat(QString(SRCDIR) + "/SampleData2.json");
        QJSValue object = engine.evaluate(content);

        _::merge(object, source);

        QCOMPARE(object.property("value4").property("value1").toInt(), 21);
        QCOMPARE(object.property("value4").property("value2").toNumber(), 2.0);

        QVariantMap map;
        _::merge(map, object);

        QCOMPARE(map["value4"].toMap()["value1"].toInt(), 21);
        QCOMPARE(map["value4"].toMap()["value2"].toDouble(), 2.0);

    }

}
