#include <iostream>
#include <stdexcept>

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "collector.h"
#include "station.h"


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

    void Collector::fillStationResult(const Station& s)
    {

	CURLcode code;
	static char errorBuffer[CURL_ERROR_SIZE];

	code = ::curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	    throw new std::runtime_error("cannot set ErrorBuffer");

	code = ::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK)
	    throw new std::runtime_error("cannot set Redirect Stuff");

	code = ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	    throw new std::runtime_error("cannot set WriterFunc");


	std::string readed;
	code = ::curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readed);
	if (code != CURLE_OK)
	    throw new std::runtime_error("cannot set Databuffer");


	std::string ort("Dresden");
	std::string verz("1");
	std::string url("http://widgets.vvo-online.de/abfahrtsmonitor/Abfahrten.do?ort=" + ort + "&hst=" + s.getUrlParam() + "&vz=" + verz); 
	code = ::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	if (code != CURLE_OK)
	    throw new std::runtime_error("cannot set URL");

	code = ::curl_easy_perform(curl);
	if (code != CURLE_OK)
	    throw new std::runtime_error("cannot perform Request");



	for (std::vector<std::pair<std::string, std::string> >::iterator it = html_preplaces.begin();
		it != html_preplaces.end(); it++) {
	    boost::algorithm::replace_all(readed, (*it).first, (*it).second);
	}

	std::cout << readed << std::endl;

        std::string regex("\\[(\\[(.*)\\],?)+\\]");
        boost::regex e(regex);

    }


}
