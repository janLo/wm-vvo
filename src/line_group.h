#ifndef LINE_GROUP_H
#define LINE_GROUP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>
#ifndef NOT_USE_THREADS
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#endif

#include "station.h"


namespace wm_vvo {
    class LineGroup {

        std::vector<Station> stations;
        std::string headline;
#ifndef NOT_USE_THREADS
        mutable bool work_request;
        mutable bool working;
        boost::shared_ptr<boost::mutex> request_mutex;
        boost::shared_ptr<boost::condition_variable> wait_cond;
        mutable boost::shared_ptr<boost::thread> worker;
#endif

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
        void refreshContent(bool async) const;

        private:

        StationIterator findStation(const std::string& name) const;
#ifndef NOT_USE_THREADS
        void workLoop() const;
#endif

    };

}

#endif
