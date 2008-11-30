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

    std::vector<Station>::iterator LineGroup::findStation(const std::string& name) {
	using namespace boost::lambda;
	return std::find_if(stations.begin(), stations.end(), name == bind<std::string>(&Station::getName, _1));
    }

    Station& LineGroup::getStation(const std::string& name){
        std::vector<Station>::iterator it = findStation(name);
	if (it == stations.end())
	    throw new std::runtime_error("No Station found!");
	return *it;
    }

    bool LineGroup::hasStation(const std::string& name) {
        std::vector<Station>::iterator it = findStation(name);
	return (it == stations.end() ? false : true);
    }

}
