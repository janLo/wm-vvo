#include <iostream>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "dockapp.h"
#include "line_group.h"
#include "station.h"
#include "line.h"

namespace wm_vvo {

    inline gboolean updateHelper(gpointer data){
        Dockapp* d = static_cast<Dockapp*>(data);
        d->updateGui();
        return TRUE;
    }

    Dockapp::Dockapp(const std::vector<LineGroup>& g, int *argc, char ***argv) 
        :groups(g), first(true)
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

        height = (5*line_num) + (7*station_num) + (8*linegroup_num) + 10;
        

        applet.name        = "wm-vvo";
        applet.version     = "0.1";
        applet.nice_name   = "Wm - Vvo";
        applet.author      = "Jan Losinski";
        applet.license     = "GPLv2";
        applet.description = "VVO Abfahrtsmonitor";
        applet.icon        = "";
        applet.image_path  = ".";

        std::cout << height << std::endl;

        ::gai_init2(&applet, argc, argv);
        ::gai_background_set(64,height,64,TRUE);

        bg = gai_load_image("bg.xpm");

        gai_signal_on_update(updateHelper,100,this);
        
            
       ::gai_start();
    }
    Dockapp::~Dockapp() {
      free(bg); 
    }



    gboolean Dockapp::updateGui(){
    
        DrawString* ss;
        if(first){
            first = false;
            int bg_height = ::gdk_pixbuf_get_height(bg);

            int num = height/bg_height;

            for (int i = 0; i < num; i++){
                ::gai_draw_bg(bg, 0,0, 60, bg_height, 1, i*bg_height);
            }

            ::gai_draw_update_bg();

            for (std::vector<LineGroup>::const_iterator it = groups.begin(); it != groups.end(); it++){

                const LineGroup& l = *it;

                linegroup_strings.push_back(new DrawString(4,5,38,l.getHeadline(), 5, "sans", 200,200,50, GAI_TEXT_BOLD));

                for (std::vector<Station>::const_iterator sit = l.firstStation(); sit != l.lastStation(); sit++){

                    const Station& s = *sit;

                    for (std::vector<Line>::const_iterator lit = s.firstLine(); lit != s.lastLine(); lit++) {
                        const Line& l = *lit;
                    }
                }
            }

        }

        int pos = 0;

                linegroup_strings[0]->draw();


        ::gai_draw_update();
        return TRUE;
    }



    Dockapp::DrawString::DrawString(int posx, int posy, int maxwidth, const std::string& text, int fontsize, const std::string& font, int r, int g, int b, int feature)
        :posx(posx), posy(posy), maxwidth(maxwidth), text(text), fontsize(fontsize), font(font), red(r), green(g), blue(b), feature(feature), roate(false), roate_state(0), pixmap(0)
    {
        pixmap = ::gai_text_create(text.c_str(), font.c_str(), fontsize, feature, red, green, blue);
        calcRoate();
    }
    
    Dockapp::DrawString::~DrawString() {
     //   if (0 != pixmap)
     //       free(pixmap);
    }

    void Dockapp::DrawString::updatePosY(int newposy) {
        posy = newposy;
    }

    void Dockapp::DrawString::draw(){

        if(roate){
            if(roate_state <= pixmap_length + 8) {
                roate_state ++;
            } else {
                roate_state = 1;
            }
            ::gai_draw(pixmap, roate_state, 0, maxwidth, pixmap_height, posx , posy);
        } else {
            ::gai_draw(pixmap, 0, 0, pixmap_length, pixmap_height, posx, posy);
        }
    }

    void Dockapp::DrawString::setText(const std::string& new_text) {
        text = new_text;
        if (0 != pixmap)
            free(pixmap);
        pixmap = ::gai_text_create(text.c_str(), font.c_str(), fontsize, feature, red, green, blue);
        calcRoate();
    }

    void Dockapp::DrawString::calcRoate(){
        pixmap_length = ::gdk_pixbuf_get_width(pixmap);
        pixmap_height = ::gdk_pixbuf_get_height(pixmap);
        roate = maxwidth < pixmap_length;
        roate_state = 0;
        if (roate) {
            GdkPixbuf * tmp = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, pixmap_length*2 + 8, pixmap_height);
            gdk_pixbuf_copy_area(pixmap, 0, 0, pixmap_length, pixmap_height, tmp, 0, 0);
            gdk_pixbuf_copy_area(pixmap, 0, 0, pixmap_length, pixmap_height, tmp, pixmap_length+8, 0);
            gdk_pixbuf_unref(pixmap);
            pixmap = tmp;
        }
    }
}
