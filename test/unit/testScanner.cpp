#include "../src/scanner.hpp"
#include "../src/token.hpp"

#define BOOST_TEST_MODULE VsidCommonTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(lexer_tests)

BOOST_AUTO_TEST_CASE(number) {

  Scanner scanner{""};
  std::vector<Token> tokens = scanner.scanTokens();
  int res = tokens.size();
  BOOST_CHECK_EQUAL(res, 1);
  Token token = tokens[0];
  Token expected = Token{TokenType::t_EOF, "", 0,0};
  BOOST_CHECK(token == expected);
}

// BOOST_AUTO_TEST_CASE( simple2 )
//{
// int x = 10;

// int res = plus_one(x);

// BOOST_CHECK_EQUAL( res, 12 );

//}
BOOST_AUTO_TEST_SUITE_END()
