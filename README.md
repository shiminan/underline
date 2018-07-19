## Underline

A C++ utility library provides useful functional programming helpers like lodash.js

Features:
-----

1. It is a C++ 11 library but supports C++14's generic lambda function (using auto as parameter type)

2. Single Header Library

3. All the helper functions are pure, reentrant, and thead-safe

Use-cases
-------

 1) Serialize a QObject
```C++
#include <underline.h>
  QVariantMap dest;
  QObject* source;

  // Non-deep copy from object to output
  _::assign(output, object);

  // Serialize but ignore the parent field
  output = _::omit(object, QStringList(){"parent"});
```

2) Avoid using chaining method to query for a property value.

```C++
// Obtain the objectName property from object's parent.
// It is equivalent to object->parent()->objectName() but _::get will take care null pointer checking
QVariant property = _::get(object, "parent.objectName");
```

```C++

QList<int> output1 = _::map(QList<QString>(){"1","2","3"},  [](auto item) { return item.toInt();});

QVector<int> output2 = _::map(QVector<QString>() {"1","2","3"}, [](auto item, int index) { return item.toInt();});

QVector<int> output3 = _::map(std::vector<QString>(){"1","2","3"}, [](auto item, int index, auto collection) { return item.toInt();});

```

Installation
-----------

```
wget https://raw.githubusercontent.com/benlau/underline/master/src/cpp/underline.h
```

API
===

assign
-----

```C++
_:assign(QVariantMap& dest, QObject source, ...)
_:assign(QObject* dest, QObject* source, ...)
_:assign(QVariantMap& dest, QVariantMap source, ...)
_:assign(QObject* dest, QJSValue source, ...) // Available only if `quick` is set in the qmake project file.
```

Assigns the string-keyed properties from the source object to the destination object. The source objects are applied from left to right. Subsequent sources overwrite property assignments of previous sources.

Arguments:

 * dest: The destination object
 * source && ... : The source object(s) to be assigned to the destination object

Returns:

 * None

Example

```C++
_::assign(object, QVariantMap{{"objectName", "Test"}});
_::assign(map, object, QVariantMap{{"objectName", "Test"}});
```

get
---

```C++
QVariant _::get(const QObject* source, QString path,QVariant defaultValue = QVariant())
QVariant _::get(const QVariantMap source, QString path, QVariant defaultValue = QVariant())
```

Obtain the value from the source object at the given path. If the path does not exist, it will return the default value.

Example:

```
// Obtain the objectName property from object's parent
QVariant property = _::get(object, "parent.objectName");
```

map
----

```C++
_::map(collection, iteratee)
```

Creates an array of values by running each element in collection throught iteratee. The iteratee is invoked with three arguments: (value, index, collection). The index and collection parameters are optional.


Arguments:

 * collection: The input collection. QList/QVector/std::vector are proven to work.
 * iteratee: The function invoked per iteration


Returns:

 * (Array) The new mapped array. The container type should be same as the input collection.

Example:

```C++
QList<int> output1 = _::map(QList<QString>(){"1","2","3"},  [](auto item) { return item.toInt();});

QVector<int> output2 = _::map(QVector<QString>() {"1","2","3"}, [](auto item, int index) { return item.toInt();});

QVector<int> output3 = _::map(std::vector<QString>(){"1","2","3"}, [](auto item, int index, auto collection) { return item.toInt();});
```

omit
----

pick
---

set
---

some
----
