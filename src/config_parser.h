#include <vector>
#include <map>
#include <boost/spirit/core.hpp>

namespace wm_vvo {

  
  class LineGroup;

  class ConfigParser {

    enum scope {
      GLOBAL = 0,
      LINEGROUP = 1,
      STATION = 2,
      LINE = 3,
    };

    int scope;


    boost::spirit::rule<> linegroup;
    boost::spirit::rule<> linegroup_begin;
    boost::spirit::rule<> linegroup_end;
    boost::spirit::rule<> station;
    boost::spirit::rule<> station_begin;
    boost::spirit::rule<> station_end;
    boost::spirit::rule<> line;
    boost::spirit::rule<> line_begin;
    boost::spirit::rule<> line_end;
    boost::spirit::rule<> assignment;
    boost::spirit::rule<> begin;
    boost::spirit::rule<> end;
    boost::spirit::rule<> eol;

    std::vector<std::map<std::string, std::string > > symbols;
    std::string key;
    std::string val;

    void pushSymbol();
    void buildLine();
    void buildStation();
    void buildLineGroup();

    public:
      ConfigParser();
      void parseConfig(const std::string filename);
  };
}
