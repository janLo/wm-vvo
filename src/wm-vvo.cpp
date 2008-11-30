#include <iostream>
#include <string>

#include "line_group.h"
#include "collector.h"




int main(int argc, char* argv[]){

  using namespace wm_vvo;

  LineGroup line("Testgruppe");
  Station s1("Wasaplatz", "wasaplatz");
  Station s2("Pohlandplatz", "pohlandplatz");

  s1.addLine(Line("76", ".*76.*", ".*"));
  s1.addLine(Line("13", ".*13.*", ".*"));


  s2.addLine(Line("61", ".*61.*", ".*"));
  s2.addLine(Line("4", ".*4.*", ".*"));

  line.addStation(s1);
  line.addStation(s2);

  Collector::getCollector().fillStationResult(s1);
  Collector::getCollector().fillStationResult(s2);

}
