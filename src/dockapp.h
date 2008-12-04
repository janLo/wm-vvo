#include <vector>

#include <boost/utility.hpp>

#include <gai/gai.h>



namespace wm_vvo {

    class LineGroup;

    class Dockapp : private boost::noncopyable { 

        const std::vector<LineGroup>& groups;
        GaiApplet applet;
        int height;
        int interval;


        bool first;

        class DrawString {
            std::string text;
            int posx;
            int posy;
            int maxwidth;
            int fontsize; 
            char * font;
            int red;
            int green;
            int blue;
            int feature;

            bool rotate;
            int rotate_state;

            GdkPixbuf* pixmap;
            int pixmap_length;
            int pixmap_height;

            void calcRoate();

            public:

            DrawString(int posx, int posy, int maxwidth, const std::string& text, int fontsize, const std::string& font, int r, int g, int b, int feature);
            ~DrawString();

            inline void updatePosY(int posy);
            inline void draw();
            inline void setText(const std::string& s);
            inline int getPosY() const {return posy;}

        };

        class LineString {
            DrawString * line;
            DrawString * dir;
            DrawString * time;
            int pos;

            public:
            LineString(const std::string& line, const std::string& dir, int time, int pos);
            ~LineString();

            inline void update(const std::string& dir, int time);
            inline void draw();
            inline void draw(int i);
        };

        std::vector<DrawString *> linegroup_strings;
        std::vector<DrawString *> station_strings;
        std::vector<LineString *> line_strings;

        inline void drawLinegroup(const LineGroup& l);

        public: 
        Dockapp(const std::vector<LineGroup>& g, int *argc, char ***argv);
        ~Dockapp();

        void start();
        void updateGui();

        friend gboolean updateHelper(gpointer data);
    };
    gboolean updateHelper(gpointer data);
}
