#ifndef STATION_H
#define STATION_H

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

#include "line.h"


namespace wm_vvo {

    class Station {
        std::string name;
        std::string url_param;

        std::vector<Line> lines;


        public:
        Station(const std::string& name, const std::string& url_param);
        Station(const std::string& name);
        ~Station();

        void addLine(const Line& l);
        const Line& getLine(const std::string& name) const ;
        bool hasLine(const std::string& name) const ;

        typedef std::vector<Line>::const_iterator LineIterator;

        inline const std::string& getName() const {return name;}
        inline const std::string& getUrlParam() const {return url_param;}
        inline LineIterator firstLine() const {return lines.begin();}
        inline LineIterator lastLine() const {return lines.end();}
        inline int getLineCount() const {return lines.size(); }

        private:

        LineIterator findLine(const std::string& name) const; 
    };
}

#endif
