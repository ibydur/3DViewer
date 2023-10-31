#include <QtTest/QtTest>

#include "CgalApi.h"

class CgalApiTest : public QObject
{
    Q_OBJECT

private slots:
    static void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
        if (type != QtDebugMsg) {
            QTextStream(stdout) << msg << "\n";
        }
    }
    void initTestCase() {
        qInstallMessageHandler(customMessageHandler);
    }
    void cleanupTestCase() {
        qInstallMessageHandler(nullptr);
    }
    void testConstructMeshFromInvalidObj() {
        const auto& result = CGAL_API::constructMeshFromObj("abc.obj");
        QVERIFY(nullptr == result);
    }
    void testConstructMeshFromEmptyObj() {
        const auto& result = CGAL_API::constructMeshFromObj("tmp.obj");
        QVERIFY(nullptr == result);
    }
    void testConstructValidMeshFromObj() {
        const auto& result = CGAL_API::constructMeshFromObj("FinalBaseMesh.obj");
        QVERIFY(nullptr != result);
        QVERIFY(!result->is_empty() == true);
        QVERIFY(result->is_valid() == true);
        // compare vertices
        QCOMPARE(result->number_of_vertices(), 24461);
        // compare faces
        QCOMPARE(result->number_of_faces(), 48918);
        // compare edges
        QCOMPARE(result->number_of_edges(), 73377);
    }
};

QTEST_APPLESS_MAIN(CgalApiTest)
#include "Cgal_api_test.moc"