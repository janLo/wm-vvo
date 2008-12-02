#include <iostream>
#include <string>
#include <vector>

#include "line_group.h"
#include "collector.h"
#include "config_parser.h"



void printLineGroup(const wm_vvo::LineGroup& l){
  using namespace wm_vvo;
  for (LineGroup::StationIterator it = l.firstStation(); 
      it != l.lastStation(); it++)
  {
        const Station& s = *it;
        std::cout << "Station: " << s.getName() << std::endl;
        for (Station::LineIterator lit = s.firstLine();
                lit != s.lastLine(); lit++)
        {
            const Line& l = *lit;
            std::cout << "  Line " << l.getName() << ":" << std::endl;
            for (Line::ResultIterator rit = l.firstResult();
                    rit != l.lastResult(); rit++)
            {
                std::cout << "    " << (*rit).getMinutes() << "  " << (*rit).getDirection() << std::endl;

            }
        }
  }
}

int main(int argc, char* argv[]){

  using namespace wm_vvo;

/*  LineGroup line("Testgruppe");
  Station s1("Wasaplatz", "wasaplatz");
  Station s2("Pohlandplatz", "pohlandplatz");

  s1.addLine(Line("61", "61", "Löbtau"));
  s1.addLine(Line("75", "75", "Goppeln"));
  s1.addLine(Line("13", "13", "Kaditz"));


  s2.addLine(Line("61", "61", "[\\d\\w_.: -]+"));
  s2.addLine(Line("4", "4", "[\\d\\w_.: -]+"));

  Collector::getCollector().fillStationResult(s1);
  Collector::getCollector().fillStationResult(s2);

  line.addStation(s1);
  line.addStation(s2);


  printLineGroup(line);
*/
  ConfigParser p;
  p.parseConfig("vm-vvo.rc");

  const std::vector<LineGroup>& groups = p.getLineGroups();

  for (std::vector<LineGroup>::const_iterator it = groups.begin(); it != groups.end(); it++){
    Collector::getCollector().fillLineGroup(*it);
  printLineGroup(*it);

  }



}
