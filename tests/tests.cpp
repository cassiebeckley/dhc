#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <graft/scanner.hpp>

TEST_CASE("Test scanner state")
{
    dhc::graft::scanner scanner("abcdef");

    auto state = scanner.state;

    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 0);
    REQUIRE(scanner.index() == 0);

    char data = scanner.next()->data;
    REQUIRE(data == 'a');
    REQUIRE(scanner.finished() == false);
    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 1);
    REQUIRE(scanner.index() == 1);

    data = scanner.next()->data;
    REQUIRE(data == 'b');
    REQUIRE(scanner.finished() == false);
    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 2);
    REQUIRE(scanner.index() == 2);

    data = scanner.next()->data;
    REQUIRE(data == 'c');
    REQUIRE(scanner.finished() == false);
    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 3);
    REQUIRE(scanner.index() == 3);

    data = scanner.next()->data;
    REQUIRE(data == 'd');
    REQUIRE(scanner.finished() == false);
    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 4);
    REQUIRE(scanner.index() == 4);

    scanner.newline();

    REQUIRE(scanner.lineno() == 1);
    REQUIRE(scanner.charno() == 0);
    REQUIRE(scanner.index() == 4);

    data = scanner.next()->data;
    REQUIRE(data == 'e');
    REQUIRE(scanner.finished() == false);
    REQUIRE(scanner.lineno() == 1);
    REQUIRE(scanner.charno() == 1);
    REQUIRE(scanner.index() == 5);

    data = scanner.next()->data;
    REQUIRE(data == 'f');
    REQUIRE(scanner.finished() == true);
    REQUIRE(scanner.lineno() == 1);
    REQUIRE(scanner.charno() == 2);
    REQUIRE(scanner.index() == 6);

    auto match = scanner.next();
    REQUIRE(match == nullptr);
    REQUIRE(scanner.finished() == true);
    REQUIRE(scanner.lineno() == 1);
    REQUIRE(scanner.charno() == 2);
    REQUIRE(scanner.index() == 6);

    scanner.state = state;

    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 0);
    REQUIRE(scanner.index() == 0);

    data = scanner.next()->data;
    REQUIRE(data == 'a');
    REQUIRE(scanner.finished() == false);
    REQUIRE(scanner.lineno() == 0);
    REQUIRE(scanner.charno() == 1);
    REQUIRE(scanner.index() == 1);
}
