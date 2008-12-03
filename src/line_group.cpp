#include <algorithm>
#include <stdexcept>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "line_group.h"

namespace wm_vvo {

    int foobar(std::string, std::string){return 1;}

    LineGroup::LineGroup(const std::string& headline)
        : headline(headline) 
    {}

    LineGroup::~LineGroup(){}

    void LineGroup::addStation(const Station& s){
        stations.push_back(s);
    }

    LineGroup::StationIterator LineGroup::findStation(const std::string& name) const {
        using namespace boost::lambda;
        return std::find_if(stations.begin(), stations.end(), name == bind<std::string>(&Station::getName, _1));
    }

    const Station& LineGroup::getStation(const std::string& name) const {
        LineGroup::StationIterator it = findStation(name);
        if (it == stations.end())
            throw new std::runtime_error("No Station found!");
        return *it;
    }

    bool LineGroup::hasStation(const std::string& name) const {
        LineGroup::StationIterator it = findStation(name);
        return (it == stations.end() ? false : true);
    }
    LineGroup::StationIterator LineGroup::firstStation() const {
        return stations.begin();
    }
    LineGroup::StationIterator LineGroup::lastStation() const {
        return stations.end();
    }
    int LineGroup::getStationCount() const {
        return stations.size();
    }

}
