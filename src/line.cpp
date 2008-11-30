#include "line.h"

namespace wm_vvo {
    Line::Line(const std::string id, const std::string& id_regexp, const std::string& line_regexp)
	: id_name(id), id_regexp(id_regexp), direction_regexp(line_regexp)
    {
    }
    Line::~Line(){}
}
