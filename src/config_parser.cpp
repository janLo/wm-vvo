#include <string>
#include <iostream>
#include <fstream>

#include <boost/spirit/utility/regex.hpp>
#include <boost/spirit/actor/assign_actor.hpp>
#include <boost/spirit/actor/insert_at_actor.hpp>

#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/functions.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "config_parser.h"



namespace wm_vvo {
    ConfigParser::ConfigParser() :symbols(), scope(GLOBAL) {
        using namespace boost::spirit;
        using namespace boost::lambda;

        symbols.push_back(std::map<std::string, std::string>());
        symbols.push_back(std::map<std::string, std::string>());
        symbols.push_back(std::map<std::string, std::string>());
        symbols.push_back(std::map<std::string, std::string>());

        assignment       = *space_p >> regex_p("[a-z]+")[assign_a(key)] >> *space_p >> 
                           ch_p('=') >> *space_p >> 
                           ch_p('"') >> regex_p("[\\w\\d._\\*\\söäüß?-]+")[assign_a(val)] >> ch_p('"')[bind(&ConfigParser::pushSymbol, this)] >> 
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

    }

    void ConfigParser::pushSymbol(){
        symbols[scope].insert(std::pair<std::string,std::string>(key,val));
    }

    void ConfigParser::buildLine(){
        std::cout << "Line: "<< std::endl;
        for (std::map<std::string, std::string>::iterator it = symbols[LINE].begin();
                it != symbols[LINE].end(); it++) {
            std::cout << (*it).first << " " << (*it).second << std::endl;
        }
        symbols[LINE].clear();
        std::cout <<  std::endl;
        /*       for (std::map<std::string, std::string>::iterator it = symbols[GLOBAL].begin();
                 it != symbols[GLOBAL].end(); it++) {
                 std::cout << (*it).first << " " << (*it).second << std::endl;
                 }*/
    }
    void ConfigParser::buildStation(){

        std::cout << "Station: "<< std::endl;
        for (std::map<std::string, std::string>::iterator it = symbols[STATION].begin();
                it != symbols[STATION].end(); it++) {
            std::cout << (*it).first << " " << (*it).second << std::endl;
        }
        symbols[STATION].clear();
        std::cout <<  std::endl;
    }
    void ConfigParser::buildLineGroup(){
        std::cout << "LineGroup: "<< std::endl;
        for (std::map<std::string, std::string>::iterator it = symbols[LINEGROUP].begin();
                it != symbols[LINEGROUP].end(); it++) {
            std::cout << (*it).first << " " << (*it).second << std::endl;
        }
        symbols[LINEGROUP].clear();
        std::cout <<  std::endl;
    }

    void ConfigParser::parseConfig(const std::string filename){
        
        std::fstream file(filename.c_str(), std::ios_base::in);

        std::string tmp, result;

        while ( getline(file, tmp) ) result.append(tmp );

        if(parse(result.c_str(), linegroup, boost::spirit::space_p).hit){
            std::cout << "FOO" << std::endl;
        }

    }
}
