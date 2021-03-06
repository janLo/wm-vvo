#include <iostream>

#include <boost/lexical_cast.hpp>

#include <string.h>
#include <time.h>

#include "dockapp.h"
#include "line_group.h"
#include "station.h"
#include "line.h"
#include "config_parser.h"

namespace wm_vvo {


    inline gboolean updateHelper(gpointer data){
        Dockapp* d = static_cast<Dockapp*>(data);
        d->updateGui();
        return TRUE;
    }

    Dockapp::Dockapp(const std::vector<LineGroup>& g, int *argc, char ***argv) 
        : groups(g), first(true)
    {

        int line_num = 0;
        int station_num = 0;
        int linegroup_num = groups.size(); 
        
        for (std::vector<LineGroup>::const_iterator it = groups.begin(); it != groups.end(); it++){
            station_num += (*it).getStationCount();
            for (std::vector<Station>::const_iterator sit = (*it).firstStation(); sit != (*it).lastStation(); sit++){
                line_num += (*sit).getLineCount();
            }
        }

        height = (8*line_num) + (13*station_num) + (16*linegroup_num) + 5 ;
        
        applet.name        = "wm-vvo";
        applet.version     = "0.1";
        applet.nice_name   = "Wm - Vvo";
        applet.author      = "Jan Losinski";
        applet.license     = "GPLv2";
        applet.description = "VVO Abfahrtsmonitor";
        applet.icon        = "";
        applet.image_path  = ".";

        ::gai_init2(&applet, argc, argv);
        ::gai_background_set(64,height,64,TRUE);

        gai_signal_on_update(updateHelper,ConfigParser::getConfigParser().getRefresh(),this);
        
        interval = ConfigParser::getConfigParser().getInterval();

        int pos = 5;
        for (std::vector<LineGroup>::const_iterator it = groups.begin(); it != groups.end(); it++){
            const LineGroup& l = *it;
            linegroup_strings.push_back(new DrawString(4,pos, 57, l.getHeadline(), 5, "sans", 230, 230, 60, GAI_TEXT_BOLD));
            pos += 11;

            l.refreshContent(false);

            for (std::vector<Station>::const_iterator sit = l.firstStation(); sit != l.lastStation(); sit++){
                const Station& s = *sit;
                station_strings.push_back(new DrawString(5,pos, 50, s.getName(), 5, "sans", 200, 200, 50, GAI_TEXT_NORMAL));
                pos += 9;
                for (std::vector<Line>::const_iterator lit = s.firstLine(); lit != s.lastLine(); lit++) {
                    const Line& l = *lit;
                    line_strings.push_back(new LineString(l.getName(), "A", 0, pos));
                    pos += 8;
                }
                pos += 4;
            }
            pos += 5;
        }
    }

    Dockapp::~Dockapp() {
    }

    inline void Dockapp::drawLinegroup(const LineGroup& l){

    }

    void Dockapp::start(){
        ::gai_start();
    }


    void Dockapp::updateGui(){

        static time_t last_data_update;
        static time_t last_image_update;

        if(first){
            GdkPixbuf * tmp = ::gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 64, height);
            ::gdk_pixbuf_fill(tmp, 0x090209FF);


            ::gai_draw_bg(tmp, 0,0, 60, height-3, 1, 1);

            ::gdk_pixbuf_unref(tmp);
            ::gai_draw_update_bg();
            last_data_update = ::time(0) - interval - 1;
            last_image_update = ::time(0) - interval - 1;
        } 

        time_t now = time(0);
        int div1 = now - last_data_update;
        int div2 = now - last_image_update;


        int pos = 3;
        int lg_count = 0;
        int st_count = 0;
        int li_count = 0;

        for (std::vector<LineGroup>::const_iterator it = groups.begin(); it != groups.end(); it++){
            const LineGroup& l = *it;

            if (div1 > interval){
                l.refreshContent(true);
                last_data_update = ::time(0);
            }

            linegroup_strings[lg_count]->draw();
            for (std::vector<Station>::const_iterator sit = l.firstStation(); sit != l.lastStation(); sit++){
                const Station& s = *sit;
                station_strings[st_count]->draw();
                pos = station_strings[st_count]->getPosY() + 9;
                for (std::vector<Line>::const_iterator lit = s.firstLine(); lit != s.lastLine(); lit++) {
                    const Line& l = *lit;
                    try {
                        const Line::Result& r = l.getValidResult();
                        if (div2 > 30 || first){
                            line_strings[li_count]->update(r.getDirection(), r.getMinutes());
                            last_image_update = ::time(0);
                        }
                        line_strings[li_count]->draw(pos);
                        pos += 8;
                    } catch (Line::NoValidDataError e) {
                        if (div2 > 30 || first)
                            std::cout << "No valid value for Line " << l.getName() << std::endl;
                    }
                    li_count++;
                }
                st_count++;
            }
            lg_count++;
        }

        ::gai_draw_update();
        first = false;
    }



    Dockapp::DrawString::DrawString(int posx, int posy, int maxwidth, const std::string& text, int fontsize, const std::string& newfont, int r, int g, int b, int feature)
        :posx(posx), posy(posy), maxwidth(maxwidth), text(text), fontsize(fontsize), font(0), red(r), green(g), blue(b), feature(feature), rotate(false), rotate_state(0), pixmap(0)
    {
        font = new char[newfont.size() + 1];
        ::strcpy(font, newfont.c_str());
        pixmap = ::gai_text_create(text.c_str(), font, fontsize, feature, red, green, blue);
        calcRoate();
    }

    Dockapp::DrawString::~DrawString() {
        if (0 != pixmap)
            gdk_pixbuf_unref(pixmap);
        delete[] font;
    }

    inline void Dockapp::DrawString::updatePosY(int newposy) {
        posy = newposy;
    }

    inline void Dockapp::DrawString::draw(){
        if(rotate){
            if(rotate_state <= pixmap_length + 8) {
                rotate_state ++;
            } else {
                rotate_state = 1;
            }
            ::gai_draw(pixmap, rotate_state, 0, maxwidth, pixmap_height, posx , posy);
        } else {
            ::gai_draw(pixmap, 0, 0, pixmap_length, pixmap_height, posx, posy);
        }
    }

    inline void Dockapp::DrawString::setText(const std::string& new_text) {
        text = new_text;
        if (0 != pixmap)
            gdk_pixbuf_unref(pixmap);
        pixmap = ::gai_text_create(text.c_str(), font, fontsize, feature, red, green, blue);
        calcRoate();
    }

    void Dockapp::DrawString::calcRoate(){
        pixmap_length = ::gdk_pixbuf_get_width(pixmap);
        pixmap_height = ::gdk_pixbuf_get_height(pixmap);
        rotate = (maxwidth < pixmap_length) && (1 == ConfigParser::getConfigParser().getRoate());
        rotate_state = 0;
        if (rotate) {
            GdkPixbuf * tmp = ::gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, pixmap_length*2 + 8, pixmap_height);
            ::gdk_pixbuf_fill(tmp, 0x00000000);
            ::gdk_pixbuf_copy_area(pixmap, 0, 0, pixmap_length, pixmap_height, tmp, 0, 0);
            ::gdk_pixbuf_copy_area(pixmap, 0, 0, pixmap_length, pixmap_height, tmp, pixmap_length+8, 0);
            ::gdk_pixbuf_unref(pixmap);
            pixmap = tmp;
        } else {
            pixmap_length = (pixmap_length <= maxwidth ? pixmap_length : maxwidth);
        }
    }

    Dockapp::LineString::LineString(const std::string& newline, const std::string& newdir, int newtime, int newpos)
        : line(0), dir(0), time(0), pos(newpos)
    { 
        std::string t = boost::lexical_cast<std::string, int>(newtime);
        line = new DrawString(5, newpos, 10, newline, 5, "sans", 180, 180, 70, GAI_TEXT_NORMAL);
        dir  = new DrawString(20,newpos, 26, newdir,  5, "sans", 180, 180, 70, GAI_TEXT_NORMAL);
        time = new DrawString(49,newpos, 10, t,       5, "sans", 180, 180, 70, GAI_TEXT_NORMAL);

    }

    Dockapp::LineString::~LineString() {
        delete line;
        delete dir;
        delete time;
    }

    inline void Dockapp::LineString::update(const std::string& newdir, int newtime) {
        std::string t = boost::lexical_cast<std::string, int>(newtime);
        dir->setText(newdir);
        time->setText(t);
    }

    inline void Dockapp::LineString::draw(){
        line->draw();
        dir->draw();
        time->draw();
    }

    inline void Dockapp::LineString::draw(int newpos){
        pos = newpos;
        line->updatePosY(pos);
        dir->updatePosY(pos);
        time->updatePosY(pos);
        draw();
    }
}
