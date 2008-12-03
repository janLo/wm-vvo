#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <vector>
#include <map>
#include <stdexcept>

#include <boost/spirit/core.hpp>

namespace wm_vvo {


    class LineGroup;
    class Station;
    class Line;

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
        boost::spirit::rule<> global;

        std::vector<std::map<std::string, std::string > > symbols;
        std::string key;
        std::string val;

        std::vector<LineGroup> linegroup_tmp;
        std::vector<Station> station_tmp;
        std::vector<Line> line_tmp;

        std::string location;
        int interval;
        int delay;
        int roate;

        void pushSymbol();
        void buildLine();
        void buildStation();
        void buildLineGroup();
        void setGlobalCfg();

        bool parsed;

        ConfigParser();

        static ConfigParser* instance;
        void parseConfig(const std::string filename);

        public:

        static ConfigParser& getConfigParser(){
            if (0 == instance)
                throw new std::runtime_error("no instance now!");
            return *instance;
        }

        static ConfigParser& getConfigParser(const std::string filename){
            if (0 == instance){
                instance = new ConfigParser();
                instance->parseConfig(filename);
            }
            return *instance;
        }

        int getDelay() const;
        int getRoate() const;
        int getInterval() const;
        const std::string& getLocation() const;
        const std::vector<LineGroup> getLineGroups() const;

        class ParamNotFoundError : public std::runtime_error { 
            public:
                ParamNotFoundError(const std::string& s)
                    : std::runtime_error(s) 
                {}
        };
    };
}

#endif
