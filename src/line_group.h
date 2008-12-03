#ifndef LINE_GROUP_H
#define LINE_GROUP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>

#include "station.h"


namespace wm_vvo {
    class LineGroup {

        std::vector<Station> stations;
        std::string headline;


        public:
        LineGroup(const std::string& headline);
        ~LineGroup();

        void addStation(const Station& s);
        const Station& getStation(const std::string& name) const;
        bool hasStation(const std::string& name) const;

        inline const std::string getHeadline() const {return headline;}

        typedef std::vector<Station>::const_iterator StationIterator;

        StationIterator firstStation() const;
        StationIterator lastStation() const;
        int getStationCount() const;

        private:

        StationIterator findStation(const std::string& name) const;
    };

}

#endif
