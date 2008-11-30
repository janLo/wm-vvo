#include <string>
#include <vector>
#include <boost/utility.hpp>
#include <boost/algorithm/string.hpp>

#include "station.h"


namespace wm_vvo {
    class LineGroup {

	std::vector<Station> stations;
	std::string headline;

        std::vector<Station>::iterator findStation(const std::string& name);

	public:
	LineGroup(const std::string& headline);
	~LineGroup();

	void addStation(const Station& s);
	Station& getStation(const std::string& name);
	bool hasStation(const std::string& name) ;

	inline const std::string getHeadline() const {return headline;}
    };

}
