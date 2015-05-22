#include "compiler/Constructs/SymbolContainer.h"

#include <assert.h>
#include <gtest/gtest.h>

class SymbolContainerTest : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(SymbolContainerTest, SimpleIdentifierStorage) {
    SymbolContainer<int> container(0);
    int value = 45;

    ASSERT_TRUE(container.defineSymbolForName(value, QualifiedName("id")));
    ASSERT_EQ(45, container.symbolForName(QualifiedName("id")));
}

TEST_F(SymbolContainerTest, DuplicateIdentifier) {
    SymbolContainer<int> container(0);
    int value = 45;

    ASSERT_TRUE(container.defineSymbolForName(value, QualifiedName("id")));
    ASSERT_FALSE(container.defineSymbolForName(value, QualifiedName("id")));
    ASSERT_FALSE(container.defineSymbolForName(value, QualifiedName("id")));
}

TEST_F(SymbolContainerTest, NamespacedIdentifierStorage) {
    SymbolContainer<int> container(-1);
    int value = 45;
    QualifiedName name("id");
    QualifiedName space;

    space.appendComponent("namespace");
    name.prependName(space);

    ASSERT_TRUE(container.defineSymbolForName(value, name));
    ASSERT_EQ(-1, container.symbolForName(QualifiedName("id"), QualifiedName()));
    ASSERT_EQ(45, container.symbolForName(QualifiedName("id"), space));
    ASSERT_EQ(45, container.symbolForName(name));
}
