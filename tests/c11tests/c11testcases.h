#pragma once
#include <QObject>

class C11TestCases : public QObject
{
    Q_OBJECT
public:
    explicit C11TestCases(QObject *parent = nullptr);

private slots:

    void initTestCase();

    void validate_template_static_variable();

    void spec_QVariant();

    void spec_QVariantMap();

    void spec_QObject();

    void test_private_has();

    void test_private_is_static_collection();

    void test_private_is_map();

    void test_private_is_meta_object();

    void test_private_is_qobject();

    void test_private_is_key_value_type();

    void test_private_is_std_type();

    void test_private_is_qt_type();

    void test_private_traits();

    void test_private_is_lambda();

    void test_private_is_callable();

    void test_private_invoke();

    void test_private_invoke_by_read();

    void test_private_rebind_to_map();

    void test_private_has_static_meta_object();

    void test_private_read();

    void test_private_write();

    void test_private_cast_to_pointer();

    void test_private_cast_to_qobject();

    void test_private_GadgetContainer();

    void test_private_contains();

    void test_private_cast_to_collection();

    void test_merge();

    void test_merge_qobject();

    void test_merge_gadget();

    void test_merge_complex();

    void test_merge_arg1_QVariantMap_containing_Gadget();

    void spec_merge_arg1_QObject_should_direct_copy_in_missing_path();

    void spec_merge_args_QVariantMap_QVariantMap_should_support_list_merging();

    void spec_merge_arg1_QVariantMap_containing_list_of_QVariantMap();

    void spec_merge_arg1_Gadget_containing_list_of_Gadget();

    void spec_merge_arg2_Gadget_containing_list_of_Gadget();

    void spec_omit_support_Gadget_containing_list_of_Gadget();

    void spec_pick_support_Gadget_containing_list_of_Gadget();

    void test_some();

    void test_map();

    void test_range();

    void test_rangeQ();

    void test_isMap();

    void test_isKeyValueType();

    void test_isQtMetable();

    void test_get_should_support_path_in_qobject_dynamic_property();

    void test_keyBy();

    void test_keyBy_should_support_key_iteratee();

    void spec_filter_args1_collection();

    void test_toCollection();

    void spec_toCollection_should_support_registeredQtMetable();

    void test_first();

    void test_last();

};
