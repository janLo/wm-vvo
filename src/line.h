#include <string>
#include <vector>
#include <boost/utility.hpp>

#include <time.h>

namespace wm_vvo {

    class Line  {
	

	public: 
	Line(const std::string id, const std::string& id_regexp, const std::string& line_regexp);
	~Line();



	inline const std::string getName() const {return id_name;}
	inline const std::string getRegexp() const {return id_regexp;}
	inline const std::string getDirRegexp() const {return direction_regexp;}

	

	    class Result {

                public:

                    Result(int minutes, const std::string direction);
                    int getMinutes() const;

                private:
                std::string direction;
                int minutes;
                time_t timestamp;
	    };

	typedef std::vector<Line::Result>::iterator result_iter;

	void clearResults() const;
	void addResult(int minutes, const std::string& direction) const;
	inline result_iter getResultBegin() const  {return results.begin();}
	inline result_iter getResultEnd() const {return results.end();}

	private:

	std::string id_name;
	std::string id_regexp;
	std::string direction_regexp;
	mutable std::vector<Line::Result> results;

    };
    bool operator< (const Line::Result& lhs,  const Line::Result& rhs);
}
