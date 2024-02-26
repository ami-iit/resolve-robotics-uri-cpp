

#include <catch2/catch_test_macros.hpp>

#include <ResolveRoboticsURICpp.h>

TEST_CASE("FileDoesNotExist")
{
    CHECK_FALSE(ResolveRoboticsURICpp::resolveRoboticsURI("package://this/package/and/file/does/not.exist").has_value());
}
