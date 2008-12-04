#include <iostream>
#include <stdexcept>

#include <boost/regex.hpp>
#include <boost/regex/user.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/regex/icu.hpp>
#include <boost/lexical_cast.hpp>


#include "collector.h"
#include "line_group.h"
#include "line.h"
#include "station.h"
#include "config_parser.h"


namespace wm_vvo {

    static int writer(char *data, size_t size, size_t nmemb,
            std::string *writerData)
    {
        if (writerData == NULL)
            return 0;

        writerData->append(data, size*nmemb);

        return size * nmemb;
    }


    Collector::Collector(){
        curl = ::curl_easy_init();


        this->html_preplaces.push_back(std::pair<std::string, std::string>("&quot;", "\""));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#246;", "ö"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#223;", "ß"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#252;", "ü"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#228;", "ä"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#196;", "Ä"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#214;", "Ö"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#220;", "Ü"));
        this->html_preplaces.push_back(std::pair<std::string, std::string>("&#38;", "&"));
        //	this->html_preplaces.push_back(std::pair<std::string, std::string>("", ""));
    }

    Collector::~Collector(){
        ::curl_easy_cleanup(curl);
    }

    Collector& Collector::getCollector()
    {
        static Collector c;
        return c;
    }

    inline const std::string Collector::fetchData(const std::string& station){
        CURLcode code;
        static char errorBuffer[CURL_ERROR_SIZE];

        code = ::curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        if (code != CURLE_OK)
            throw std::runtime_error("cannot set ErrorBuffer");

        code = ::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        if (code != CURLE_OK)
            throw std::runtime_error("cannot set Redirect Stuff");

        code = ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        if (code != CURLE_OK)
            throw std::runtime_error("cannot set WriterFunc");


        std::string readed;
        code = ::curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readed);
        if (code != CURLE_OK)
            throw std::runtime_error("cannot set Databuffer");


        std::string ort(ConfigParser::getConfigParser().getLocation());
        std::string verz(boost::lexical_cast<std::string, int>(ConfigParser::getConfigParser().getDelay()));
        std::string url("http://widgets.vvo-online.de/abfahrtsmonitor/Abfahrten.do?ort=" + ort + "&hst=" + station + "&vz=" + verz); 
        boost::algorithm::replace_all(url, " ", "%20");
        code = ::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (code != CURLE_OK)
            throw std::runtime_error("cannot set URL");

        code = ::curl_easy_perform(curl);
        if (code != CURLE_OK)
            throw std::runtime_error("cannot perform Request");



        for (std::vector<std::pair<std::string, std::string> >::iterator it = html_preplaces.begin();
                it != html_preplaces.end(); it++) {
            boost::algorithm::replace_all(readed, (*it).first, (*it).second);
        }

        return readed;
    }

    inline void Collector::fillLine(const Line& line,  std::string data){

        std::string regex("\\[\"(" + line.getRegexp() + ")\",\"(" + line.getDirRegexp() + ")\",\"([\\d]+)\"\\]");
        boost::u32regex e(boost::make_u32regex(regex));
        boost::smatch what;

        bool first = true;

        while(boost::u32regex_search(data, what, e, boost::match_extra)){
            if (first){
                line.clearResults();
                first = false;
            }
            int minutes = boost::lexical_cast<int, std::string>(std::string(what[3]));
            line.addResult(minutes, std::string(what[2]));

            boost::algorithm::erase_first(data, std::string(what[0]));
        }
    }

    inline void Collector::fillStationResult(const Station& s)
    {

        std::string readed(fetchData(s.getUrlParam()));

        for (Station::LineIterator it = s.firstLine();
                it != s.lastLine(); it ++) 
        {
            fillLine(*it, readed);
        }
    }

    void Collector::fillLineGroup(const LineGroup& l){
        for (LineGroup::StationIterator it = l.firstStation();
                it != l.lastStation(); it++)
        {
            fillStationResult(*it);
        }

    }
}
