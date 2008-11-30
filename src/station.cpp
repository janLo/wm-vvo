#include <algorithm>
#include <stdexcept>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "station.h"

namespace wm_vvo {
  
    Station::Station(const std::string& name, const std::string& url_param)
	    : name(name), url_param(url_param) 
	{}
    Station::~Station(){}

   void Station::addLine(const Line& l){
       lines.push_back(l);
   }

   std::vector<Line>::iterator Station::findLine(const std::string& name) {
     using namespace boost::lambda;
     return std::find_if(lines.begin(), lines.end(),
         name == bind<std::string>(&Line::getName, _1));
   }

    Line& Station::getLine(const std::string& name){
	std::vector<Line>::iterator it = findLine(name);
	if (it == lines.end())
	    throw new std::runtime_error("No Line found!");
	return *it;
    }

    bool Station::hasLine(const std::string& name) {
	std::vector<Line>::iterator it = findLine(name);
	return (it == lines.end() ? false : true);
    }
}
