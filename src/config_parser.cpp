#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <boost/spirit/utility/regex.hpp>
#include <boost/spirit/actor/assign_actor.hpp>
#include <boost/spirit/actor/insert_at_actor.hpp>

#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/functions.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include <boost/algorithm/string/replace.hpp>

#include <boost/lexical_cast.hpp>

#include "config_parser.h"
//#include "line.h"
//#include "station.h"
#include "line_group.h"



namespace wm_vvo {
    ConfigParser::ConfigParser() :symbols(), scope(GLOBAL), parsed(false) {
        using namespace boost::spirit;
        using namespace boost::lambda;

        symbols.push_back(std::map<std::string, std::string>());
        symbols.push_back(std::map<std::string, std::string>());
        symbols.push_back(std::map<std::string, std::string>());
        symbols.push_back(std::map<std::string, std::string>());

        assignment       = *space_p >> regex_p("[a-z]+")[assign_a(key)] >> *space_p >> 
                           ch_p('=') >> *space_p >> 
                           ch_p('"') >> regex_p("[\\w\\d._\\*\\söäüß?\\]\\[\\\\+-]+")[assign_a(val)] >> ch_p('"')[bind(&ConfigParser::pushSymbol, this)] >> 
                           *(space_p | eol_p);

        begin            = *space_p >> str_p("begin") >> *space_p ;
        end              = *space_p >> str_p("end") >> *space_p ;
        eol              = *(space_p | eol_p);

        line_begin       = begin >> str_p("line") >> eol;
        line_end         = end >> str_p("line") >> eol; 
        line             = line_begin[var(scope) = LINE] >> 
                           (*(assignment))[bind(&ConfigParser::buildLine, this)] >> 
                           line_end[var(scope) = STATION];

        station_begin    = begin >> str_p("station") >> eol;
        station_end      = end >> str_p("station") >> eol;
        station          = station_begin[var(scope) = STATION] >> 
                           (*(assignment | line))[bind(&ConfigParser::buildStation, this)] >> 
                           station_end[var(scope) = LINEGROUP];

        linegroup_begin  = begin >> str_p("linegroup")  >> eol ;
        linegroup_end    = end >> str_p("linegroup") >> eol;
        linegroup        = linegroup_begin[var(scope) = LINEGROUP] >> 
                           (*(assignment | station))[bind(&ConfigParser::buildLineGroup, this)] >> 
                           linegroup_end[var(scope) = GLOBAL];

        global           = eol >> *(assignment | linegroup) >> eol >> end_p[bind(&ConfigParser::setGlobalCfg, this)];

    }

    void ConfigParser::pushSymbol(){
        boost::algorithm::replace_all(val, ".*", "[^\"]*"); 
        boost::algorithm::replace_all(val, ".+", "[^\"]+");
        boost::algorithm::replace_all(val, ".?", "[^\"]?");
        symbols[scope].insert(std::pair<std::string,std::string>(key,val));
    }

    void ConfigParser::buildLine(){
        if (0 == symbols[LINE].count(std::string("name")))
            throw new ParamNotFoundError("Line: name");
        if (0 == symbols[LINE].count(std::string("dir")))
            throw new ParamNotFoundError("Line: dir");

        if (0 == symbols[LINE].count(std::string("regex")))
            line_tmp.push_back(Line(symbols[LINE]["name"], symbols[LINE]["dir"]));
        else
            line_tmp.push_back(Line(symbols[LINE]["name"], symbols[LINE]["regex"], symbols[LINE]["dir"]));


        symbols[LINE].clear();
    }

    void ConfigParser::buildStation(){
        if (0 == symbols[STATION].count(std::string("name")))
            throw new ParamNotFoundError("Station: name");

        if (0 == symbols[STATION].count(std::string("urlparam")))
            station_tmp.push_back(Station(symbols[STATION]["name"]));
        else 
            station_tmp.push_back(Station(symbols[STATION]["name"], symbols[STATION]["urlparam"]));

        Station& tmp = station_tmp.back();

        {
            using namespace boost::lambda;
            std::for_each(line_tmp.begin(), line_tmp.end(), bind(&Station::addLine, &tmp, _1));
        }

        line_tmp.clear();
        symbols[STATION].clear();
    }

    void ConfigParser::buildLineGroup(){
        if (0 == symbols[LINEGROUP].count(std::string("name")))
            throw new ParamNotFoundError("Linegroup: name");

        LineGroup tmp(symbols[LINEGROUP]["name"]);

        {
            using namespace boost::lambda;
            std::for_each(station_tmp.begin(), station_tmp.end(), bind(&LineGroup::addStation, &tmp, _1));
        }

        linegroup_tmp.push_back(tmp);
        station_tmp.clear();
        symbols[LINEGROUP].clear();
    }

    void ConfigParser::setGlobalCfg(){

        if (0 == symbols[GLOBAL].count(std::string("location")))
            throw new ParamNotFoundError("location");
        if (0 == symbols[GLOBAL].count(std::string("interval")))
            throw new ParamNotFoundError("interval");
        if (0 == symbols[GLOBAL].count(std::string("delay")))
            throw new ParamNotFoundError("delay");

        location = symbols[GLOBAL]["location"];
        delay    = boost::lexical_cast<int,std::string>(symbols[GLOBAL]["delay"]);
        interval = boost::lexical_cast<int,std::string>(symbols[GLOBAL]["interval"]);

    }

    void ConfigParser::parseConfig(const std::string filename){
        
        std::fstream file(filename.c_str(), std::ios_base::in);

        std::string tmp, result;
        while ( getline(file, tmp) ) result.append(tmp );

        if(parse(result.c_str(), global, boost::spirit::space_p).hit){
            parsed = true;
        }
    }

      int ConfigParser::getDelay() const {
          if (!parsed)
              throw new std::runtime_error("not yet parsed!");
          return delay;
      }
      int ConfigParser::getInterval() const {
          if (!parsed)
              throw new std::runtime_error("not yet parsed!");
          return interval;
      }
      const std::string& ConfigParser::getLocation() const {
          if (!parsed)
              throw new std::runtime_error("not yet parsed!");
          return location;
      }
      const std::vector<LineGroup> ConfigParser::getLineGroups() const {
          if (!parsed)
              throw new std::runtime_error("not yet parsed!");
          return linegroup_tmp;
      }
}
