#include <algorithm>
#include <stdexcept>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/bind.hpp>

#include "line_group.h"
#include "collector.h"

namespace wm_vvo {

    int foobar(std::string, std::string){return 1;}

    LineGroup::LineGroup(const std::string& headline)
        : headline(headline)
#ifndef NOT_USE_THREADS
        , work_request(false),
          working(false),
          request_mutex(new boost::mutex),
          wait_cond(new boost::condition_variable), 
          worker()
#endif
    {}

    LineGroup::~LineGroup(){}

    void LineGroup::addStation(const Station& s){
        stations.push_back(s);
    }

    LineGroup::StationIterator LineGroup::findStation(const std::string& name) const {
        using namespace boost;
        return std::find_if(stations.begin(), stations.end(), name == lambda::bind<std::string>(&Station::getName, lambda::_1));
    }

    const Station& LineGroup::getStation(const std::string& name) const {
        LineGroup::StationIterator it = findStation(name);
        if (it == stations.end())
            throw std::runtime_error("No Station found!");
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

#ifndef NOT_USE_THREADS
    void LineGroup::workLoop() const{
        while (true) {
            {
                boost::unique_lock<boost::mutex> request_lock(*request_mutex);
                while (!work_request) {
                    wait_cond->wait(request_lock);
                }
                work_request = false;
                working = true;
            }
            Collector::getCollector().fillLineGroup(*this);
            {
                boost::lock_guard<boost::mutex> lock(*request_mutex);
                working = false;
                wait_cond->notify_all();
            }
        }

    }

    void LineGroup::refreshContent(bool async) const {
        boost::unique_lock<boost::mutex> lock(*request_mutex);
        if (0 == worker.get()) {
            worker.reset(new boost::thread( boost::bind(&LineGroup::workLoop, this) ));
        }
        if (async) {
            if (! working) {
                work_request = true;
                wait_cond->notify_all();
            }
        } else {
            while (working) {
                wait_cond->wait(lock);
            }
            working = true;
            Collector::getCollector().fillLineGroup(*this);
            working = false;
        }
    }
#else 
    void LineGroup::refreshContent(bool async) const {
        Collector::getCollector().fillLineGroup(*this);
    }
#endif


}
