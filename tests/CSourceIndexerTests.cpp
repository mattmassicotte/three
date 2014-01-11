#include "compiler/CSourceIndexer.h"

#include <assert.h>
#include <gtest/gtest.h>

class CSourceIndexerTest : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(CSourceIndexerTest, ExtractPoundDefineValue) {
    Three::CSourceIndexer indexer;

    ASSERT_TRUE(indexer.indexFileAtUnresolvedPath("stdio.h"));

    ASSERT_TRUE(indexer.module()->definesConstant("EOF"));
}
