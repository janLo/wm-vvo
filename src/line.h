#include <string>
#include <vector>
#include <boost/utility.hpp>

namespace wm_vvo {

    class Line  {
	

	public: 
	Line(const std::string id, const std::string& id_regexp, const std::string& line_regexp);
	~Line();

	void clearResults();
	void addResult();


	inline const std::string getName() const {return id_name;}
	inline const std::string getRegexp() const {return id_regexp;}

	

	    class Result {

	    };

	typedef std::vector<Line::Result>::iterator result_iter;

	inline result_iter getResultBegin() {return results.begin();}
	inline result_iter getResultEnd() {return results.end();}

	private:

	std::string id_name;
	std::string id_regexp;
	std::string direction_regexp;
	std::vector<Line::Result> results;

    };
}
