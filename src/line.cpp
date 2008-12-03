#include <algorithm>
#include "line.h"

namespace wm_vvo {
    Line::Line(const std::string id, const std::string& id_regexp, const std::string& line_regexp)
        : id_name(id), id_regexp(id_regexp), direction_regexp(line_regexp)
    {
    }
    Line::Line(const std::string id, const std::string& line_regexp)
        : id_name(id), id_regexp(id), direction_regexp(line_regexp)
    {
    }
    Line::~Line(){}

    void Line::clearResults() const {
        results.clear();
    }
    void Line::addResult(int minutes, const std::string& direction) const {
        results.push_back(Line::Result(minutes, direction));
        std::sort(results.begin(), results.end());
    }

    const Line::Result&  Line::getValidResult() const {
        for (ResultIterator it = results.begin(); it != results.end(); it++){
            if ((*it).isValid())
                return *it;
        }
        throw NoValidDataError();
    }

    Line::Result::Result(int minutes, const std::string direction)
        : minutes(minutes), direction(direction), timestamp(::time(0))
    {}

    int Line::Result::getMinutes() const {
        time_t now = time(0);
        time_t offset = now - timestamp;
        int offset_min = ((offset - (offset%60)) / 60);
        int ret = minutes - offset_min;
        return (ret >= 0 ? ret : 0);
    }

    bool Line::Result::isValid() const {
        return (0 <= getMinutes());
    }

    bool operator< (const Line::Result& lhs,  const Line::Result& rhs){
        return lhs.getMinutes() < rhs.getMinutes();
    }

}
