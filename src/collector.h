#include <vector>

#include <boost/utility.hpp>
#include <curl/curl.h>

namespace wm_vvo {

    class Station;
    class Collector : private boost::noncopyable {
	
	public:

	Collector();
	~Collector();
	
	static  Collector& getCollector();
	void fillStationResult(const Station& s);

	private:
	  

	CURL *curl;
	static char errorBuffer[CURL_ERROR_SIZE];

       	std::vector<std::pair<std::string, std::string> > html_preplaces;

    };

}
