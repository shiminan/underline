#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include <QtShell>
#include <functional>
#include "c14testcases.h"
#include "underline.h"
#include "dataobject.h"

static bool isOdd(int value) {
    return value % 2 == 1;
}

static QObject* createMockObject(QObject* parent) {
    QObject* ret = new DataObject(parent);
    ret->setProperty("value1", 1);
    ret->setProperty("value2", 2.0);
    ret->setProperty("value3", "3");

    return ret;
}

C14TestCases::C14TestCases(QObject *parent) : QObject(parent)
{
    auto ref = [=]() {
        QTest::qExec(this, 0, 0); // Autotest detect available test cases of a QObject by looking for "QTest::qExec" in source code
    };
    Q_UNUSED(ref);
}

template <typename F, typename T>
auto wrapper(F functor, T t) -> typename _::Private::ret_func<F,T>::type {
    return functor(t);
}

void C14TestCases::test_private_traits()
{
    {
        auto func = [](auto i) {
            Q_UNUSED(i);
        };

        QCOMPARE((std::is_convertible<std::function<void(int)>, decltype(func)>::value), false);

        QCOMPARE((std::is_convertible<decltype(func), std::function<void(int)>>::value), true);
    }

    {
        // rebind
        const std::type_info& ti1 = typeid(_::Private::rebind<QList<int>, QString>::type);
        const std::type_info& ti2 = typeid(QList<QString>);

        QVERIFY(ti1 == ti2);
    }

    {

        // container_value_type

        const std::type_info& ti1 = typeid(_::Private::container_value_type<QList<int>>::type);
        const std::type_info& ti2 = typeid(int);

        QVERIFY(ti1 == ti2);

    }

    {
        // rebind
        const std::type_info& ti1 = typeid(_::Private::rebind<QList<int>, QString>::type);
        const std::type_info& ti2 = typeid(QList<QString>);

        QVERIFY(ti1 == ti2);

    }

    {
        const std::type_info& ti1 = typeid(_::Private::rebind<QStringList, int>::type);
        const std::type_info& ti2 = typeid(QList<int>);

        QVERIFY(ti1 == ti2);

    }
}

void C14TestCases::test_private_invoke()
{
    auto myFunc0 = []() {
        return -1;
    };

    auto myFunc1 = [](auto i) {
        return i;
    };

    auto myFunc2 = [](auto i, int j) {
        Q_UNUSED(j);
        return i;
    };

    auto myFunc2_intToString = [](auto i, int j) {
        Q_UNUSED(j);
        return i.toInt();
    };

    QCOMPARE((int) (_::Private::is_args_compatible<decltype(myFunc0)>::value), 1);
    QCOMPARE((int) (_::Private::is_args_compatible<decltype(myFunc0), int>::value), 0);

    QCOMPARE((int) (_::Private::is_args_compatible<decltype(myFunc1),int>::value), 1);
    QCOMPARE((int) (_::Private::is_args_compatible<decltype(myFunc1),QString>::value), 1);
    QCOMPARE((int) (_::Private::is_args_compatible<decltype(myFunc1),QString, int>::value), 0);
    QCOMPARE((int) (_::Private::is_args_compatible<decltype(myFunc1),QString, int>::value), 0);

    QCOMPARE((std::is_same<decltype(_::Private::decl_func0<decltype(myFunc0)>()), int>::value), true);

    QCOMPARE((std::is_same<decltype(_::Private::decl_func0<decltype(myFunc1),QString>()), QString>::value), true);

    QCOMPARE((std::is_same<_::Private::ret_func<decltype(myFunc0)>::type,int>::value), true);

    QCOMPARE((std::is_same<_::Private::ret_func<decltype(myFunc1),int>::type,int>::value), true);
    QCOMPARE((std::is_same<_::Private::ret_func<decltype(myFunc2),int, int>::type,int>::value), true);
    QCOMPARE((std::is_same<_::Private::ret_func<decltype(myFunc2),QString, int>::type,QString>::value), true);
    QCOMPARE((std::is_same<_::Private::ret_func<decltype(myFunc2_intToString),QString, int>::type , int>::value), true);

    QCOMPARE(_::Private::invoke(myFunc0) , -1);
    QCOMPARE(_::Private::invoke(myFunc0, 1) , -1);
    QCOMPARE(_::Private::invoke(myFunc0, 2,3) , -1);

    QCOMPARE(_::Private::invoke(myFunc1, 1) , 1);
    QCOMPARE(_::Private::invoke(myFunc1, 2,3) , 2);

    QCOMPARE(_::Private::invoke(myFunc2, 2,3) , 2);

    QCOMPARE((std::is_same<decltype(_::Private::decl_invoke0<decltype(myFunc0)>()) , int>::value), true);

    QCOMPARE((std::is_same<decltype(_::Private::decl_invoke0<decltype(myFunc2), QString, int>()) , QString>::value), true);

    QCOMPARE((std::is_same<_::Private::ret_invoke<decltype(myFunc0),QString, int>::type , int>::value), true);
}

void C14TestCases::test_some()
{
    {
        /// Standard lambda method
        auto func = [](int item, int index) {
            Q_UNUSED(index);
            return item % 2 == 1;
        };

        QCOMPARE(_::some(QList<int>() << 0 << 2 << 4, func), false);
        QCOMPARE(_::some(QList<int>() << 0 << 3 << 4, func), true);
    }

    {
        /// Standard lambda method without the index parameter
        auto func = [](int item) {
            return item % 2 == 1;
        };

        QCOMPARE(_::some(QList<int>() << 0 << 2 << 4, func), false);
        QCOMPARE(_::some(QList<int>() << 0 << 3 << 4, func), true);
    }

    {
        /// Standard function
        QCOMPARE(_::some(QList<int>() << 0 << 2 << 4, isOdd), false);
        QCOMPARE(_::some(QList<int>() << 0 << 3 << 4, isOdd), true);
    }


    {
        auto func = [](auto item, auto index) {
            Q_UNUSED(index);
            return item % 2 == 1;
        };

        QCOMPARE(_::some(QList<int>() << 0 << 2 << 4, func), false);
        QCOMPARE(_::some(QList<int>() << 0 << 3 << 4, func), true);
    }

    {
        auto func = [](auto item) {
            return item % 2 == 1;
        };

        QCOMPARE(_::some(QList<int>() << 0 << 2 << 4, func), false);
        QCOMPARE(_::some(QList<int>() << 0 << 3 << 4, func), true);
    }

    {
        /// test non-int function
        auto func = [](QString item) {
            return item.size() % 2 == 1;
        };

        QCOMPARE(_::some(QStringList() << "0" << "2" << "4", func), true);
        QCOMPARE(_::some(QStringList() << "01" << "21" << "42", func), false);

    }

}

void C14TestCases::test_map()
{
    {
        auto func = [](QString item) {
            return item.toInt();
        };

        QCOMPARE(_::map(QList<QString>() << "1" << "2" << "3", func), QList<int>() << 1 << 2 << 3);
        QCOMPARE(_::map(QVector<QString>() << "1" << "2" << "3", func), QVector<int>() << 1 << 2 << 3);
    }

    {
        // generic lambda
        auto func = [](auto item) {
            return item.toInt();
        };

        QCOMPARE(_::map(QList<QString>() << "1" << "2" << "3", func), QList<int>() << 1 << 2 << 3);
        QCOMPARE(_::map(QVector<QString>() << "1" << "2" << "3", func), QVector<int>() << 1 << 2 << 3);
    }

    {
        // generic lambda with index parameter
        auto func = [](auto item, int index) {
            Q_UNUSED(index);
            return item.toInt();
        };

        QCOMPARE(_::map(QStringList() << "1" << "2" << "3", func), QList<int>() << 1 << 2 << 3);
        QCOMPARE(_::map(QVector<QString>() << "1" << "2" << "3", func), QVector<int>() << 1 << 2 << 3);
    }
}

void C14TestCases::test_assign()
{
    QObject* root = createMockObject(this);

    QVERIFY(root);

    /* assign(map, QObject) */

    QVariantMap data;
    _::assign(data, root);

    QVERIFY(data["objectName"] == "Root");
    QVERIFY(data["value1"].toInt() == 1);
    QVERIFY(data["value2"].toString() == "2");
    QVERIFY(data["value3"].toBool());

    QVERIFY(data["value4"].type() == QVariant::Map);
    QVERIFY(data["value4"].toMap()["value1"].toInt() == 5);

    /* assign(QObject, map) */
    data.clear();
    data["value1"] = 99;
    QVariantMap value4;
    value4["value1"] = 32;
    data["value4"] = value4;

    _::assign(root, data);
    QVERIFY(root->property("value1").toInt() == 99);
    QVERIFY(root->property("value4").value<QObject*>()->property("value1").toInt() == 32);

    /* assign(QObject, QJSvalue)*/
    /*
    QString content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
    QJSValue value = engine.evaluate(content);

    _::assign(root, value);

    QCOMPARE(root->property("value1").toInt(), 10);
    QVERIFY(root->property("value2").toString() == "11");
    QVERIFY(root->property("value3").toBool() == false);
    QCOMPARE(root->property("value4").value<QObject*>()->property("value1").toInt(), 21);
    */

    /* assign(QObject = null, QJSValue) */
    /*
    {
        QString content = QtShell::cat(QString(SRCDIR) + "/SampleData1.json");
        QJSValue value = engine.evaluate(content);

        _::assign(0, value);
    }
    */
}

void C14TestCases::test_get()
{
    QObject* root = createMockObject(this);
    QVERIFY(root);

    /* get(QObject*, QString) */
    QVariant value = _::get(root, "value4.value1");
    QCOMPARE(value.toInt(), 5);

    value = _::get(root,"value4.valueX", QString("Not Found"));
    QVERIFY(value.toString() == "Not Found");

    /* get(QVarnaintMap, QString) */

    QVariantMap source;
    _::assign(source, root);
    value = _::get(source, "value2");
    QVERIFY(value.toString() == "2");

    value = _::get(source, "valueX");
    QVERIFY(value.isNull());
}

void C14TestCases::test_set()
{
    QVariantMap data;
    _::set(data,"value1", 1);
    QVERIFY(data.contains("value1"));
    QVERIFY(data["value1"].toInt() == 1);

    _::set(data,"value2","value2");
    QVERIFY(data.contains("value1"));
    QVERIFY(data["value1"].toInt() == 1);
    QVERIFY(data.contains("value2"));
    QVERIFY(data["value2"].toString() == "value2");

    _::set(data,"value3.value1",2);

    QVariantMap value3 = data["value3"].toMap();
    QVERIFY(value3["value1"].toInt() == 2);

    /* Override value */
    data["value4"] = true;
    _::set(data,"value4.value1",3);
    QVariant value4 = data["value4"];
    QVERIFY(value4.canConvert<QVariantMap>());

    QVERIFY(value4.toMap()["value1"].toInt() == 3);
}

void C14TestCases::test_pick()
{
    QObject* root = createMockObject(this);
    QVERIFY(root);

    /* _::pick(QObject*, paths) */

    QVariantMap data = _::pick(root, QStringList()
                                       << "value1"
                                       << "value4.value1");

    QCOMPARE(data.size(), 2);
    QVERIFY(data.contains("value1"));
    QVERIFY(!data.contains("value2"));
    QVERIFY(data.contains("value4"));

    QVERIFY(data["value4"].toMap()["value1"].toInt() == 5);

    // Pick an QObject
    data = _::pick(root, QStringList() << "value4");
    QVERIFY(data["value4"].type() == QVariant::Map);

    /* _::pick(QVariant, paths) */
    QVariantMap source;
    _::assign(source, root);

    data = _::pick(source, QStringList() << "value1" << "value4.value1");

    QCOMPARE(data.size(), 2);
    QVERIFY(data.contains("value1"));
    QVERIFY(!data.contains("value2"));
    QVERIFY(data.contains("value4"));

    QVERIFY(data["value4"].toMap()["value1"].toInt() == 5);

}

void C14TestCases::test_omit()
{
    QObject* root = createMockObject(this);

    QVERIFY(root);

    QVariantMap data1;

    _::assign(data1, root);
    QCOMPARE(data1.contains("value2"), true);

    QVariantMap properties;
    properties["value1"] = true;
    properties["value3"] = false; // omit do not care the content

    QVariantMap data2 = _::omit(data1, properties);

    QVERIFY(!data2.contains("value1"));
    QVERIFY(data2.contains("value2"));
    QVERIFY(!data2.contains("value3"));
    QVERIFY(data2.contains("value4"));
    QVERIFY(data2["value4"].type() == QVariant::Map);
}
