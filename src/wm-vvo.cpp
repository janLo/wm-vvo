#include <iostream>
#include <string>

#include "line_group.h"
#include "collector.h"



void printLineGroup(const wm_vvo::LineGroup& l){
    
}

int main(int argc, char* argv[]){

  using namespace wm_vvo;

  LineGroup line("Testgruppe");
  Station s1("Wasaplatz", "wasaplatz");
  Station s2("Pohlandplatz", "pohlandplatz");

  s1.addLine(Line("61", "61", "Löbtau"));
  s1.addLine(Line("75", "75", "Goppeln"));
  s1.addLine(Line("13", "13", "Kaditz"));


  s2.addLine(Line("61", "61", "[\\d\\w_.: -]+"));
  s2.addLine(Line("4", "4", "[\\d\\w_.: -]+"));

  line.addStation(s1);
  line.addStation(s2);

  Collector::getCollector().fillStationResult(s1);
  Collector::getCollector().fillStationResult(s2);

}
