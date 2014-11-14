#include "compiler/CSourceIndexer.h"
#include "compiler/Parser/ParseContext.h"

#include <assert.h>
#include <gtest/gtest.h>
#include <fstream>

class CSourceIndexerTest : public testing::Test {
protected:
    virtual void SetUp() {
        _context = new Three::ParseContext();
        _indexer = new Three::CSourceIndexer();
    }

    virtual void TearDown() {
        delete _indexer;
        delete _context;
    }

    Three::CSourceIndexer* indexCSource(const char* input) {
        std::ofstream sourceFile("/tmp/test_output.h");

        sourceFile << input << std::endl;

        assert(_context);

        if (!_indexer->indexFileAtPath("/tmp/test_output.h", _context)) {
            std::cout << "Indexing failed" << std::endl;
            return nullptr;
        }

        return _indexer;
    }

    Three::ParseContext* context() const {
        return _context;
    }

private:
    Three::ParseContext* _context;
    Three::CSourceIndexer*  _indexer;
};

TEST_F(CSourceIndexerTest, HandleTypedef) {
    Three::CSourceIndexer* idx = this->indexCSource("typedef int MyInt;");

    ASSERT_TRUE(idx != nullptr);

    DataType type = this->context()->dataTypeForName(QualifiedName("MyInt"));
    EXPECT_EQ(DataType::Kind::CInt, type.kind());
}

TEST_F(CSourceIndexerTest, HandleTypedefForPointer) {
    Three::CSourceIndexer* idx = this->indexCSource("typedef int* MyInt;");

    ASSERT_TRUE(idx != nullptr);

    DataType type = this->context()->dataTypeForName(QualifiedName("MyInt"));
    ASSERT_EQ(DataType::Kind::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::CInt, type.subtypeAtIndex(0).kind());
}
