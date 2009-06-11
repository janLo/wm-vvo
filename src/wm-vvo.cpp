#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <stdlib.h>

#include "line_group.h"
#include "station.h"
#include "line.h"
#include "collector.h"
#include "config_parser.h"
#include "dockapp.h"


void printLineGroups(const std::vector<wm_vvo::LineGroup>& lgs){
    using namespace wm_vvo;

    for (std::vector<LineGroup>::const_iterator it = lgs.begin(); it != lgs.end(); it++){
        Collector::getCollector().fillLineGroup(*it);
        const wm_vvo::LineGroup& l = *it;

        std::cout << "Group: " << l.getHeadline() << std::endl;

        for (LineGroup::StationIterator it = l.firstStation(); 
                it != l.lastStation(); it++)
        {
            const Station& s = *it;
            std::cout << "+-- Station: " << s.getName() << std::endl;
            for (Station::LineIterator lit = s.firstLine();
                    lit != s.lastLine(); lit++)
            {
                const Line& l = *lit;
                std::cout << "|    +-- Line " << l.getName() << ":" << std::endl;
                for (Line::ResultIterator rit = l.firstResult();
                        rit != l.lastResult(); rit++)
                {
                    std::cout << "|    |    +-- " << (*rit).getMinutes() << "  " << (*rit).getDirection() << std::endl;

                }
            }
                std::cout << "|    |" << std::endl;
        }
                std::cout << std::endl;
    }
}

int main(int argc, char* argv[]){

    using namespace wm_vvo;
    bool cmdline = false;

    try {
        
        char * home = getenv("HOME");
        std::string filename(std::string(home) + "/.wm-vvo.rc");

        char c;
        while ((c = getopt (argc, argv, "c:x")) != -1){
            if (c == 'c') {
                filename = optarg;
                break;
            }
            if (c == 'x') {
                cmdline = true;
            }
        }

        ConfigParser& p = ConfigParser::getConfigParser(filename);

        const std::vector<LineGroup>& groups = p.getLineGroups();

        if (cmdline){
            printLineGroups(groups);
        } else {
            Dockapp d(groups, &argc, &argv);
            d.start();
        }

    } catch (std::runtime_error e){
        std::cerr << "Error:" << std::endl;
        std::cerr << "  " << e.what() << std::endl;
        std::cerr << "Abort!" << std::endl;
        abort();
    }
}
