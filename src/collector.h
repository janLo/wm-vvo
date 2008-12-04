#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <vector>

#include <boost/utility.hpp>
#include <curl/curl.h>

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
            static char errorBuffer[CURL_ERROR_SIZE];
            std::vector<std::pair<std::string, std::string> > html_preplaces;

            inline void fillStationResult(const Station& s);
            inline const std::string fetchData(const std::string& station);
            inline void fillLine(const Line& line, std::string data);

    };

}

#endif
