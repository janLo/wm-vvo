#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <vector>

#include <boost/utility.hpp>
#include <curl/curl.h>
#ifndef NOT_USE_THREADS
#include <boost/thread.hpp>
#endif

namespace wm_vvo {

    class Station;
    class Line;
    class LineGroup;
    class Collector : private boost::noncopyable {

        public:

            Collector();
            ~Collector();

            static  Collector& getCollector();
            void fillLineGroup(const LineGroup& l);

        private:

            CURL *curl;
            std::vector<std::pair<std::string, std::string> > html_preplaces;
#ifndef NOT_USE_THREADS
            boost::mutex fetch_mutex;
#endif

            inline void fillStationResult(const Station& s) ;
            inline const std::string fetchData(const std::string& station);
            inline void fillLine(const Line& line, std::string data);

    };

}

#endif
