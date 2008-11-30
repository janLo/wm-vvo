#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

#include "line.h"


namespace wm_vvo {

    class Station {
	std::string name;
	std::string url_param;

	std::vector<Line> lines;

        std::vector<Line>::iterator findLine(const std::string& name); 

	public:
	Station(const std::string& name, const std::string& url_param);
	~Station();

	void addLine(const Line& l);
	Line& getLine(const std::string& name);
	bool hasLine(const std::string& name);

        inline const std::string& getName() const {return name;}
	inline const std::string& getUrlParam() const {return url_param;}

    };
}
